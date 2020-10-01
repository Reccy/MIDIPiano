#ifndef UNICODE
#define UNICODE
#endif

// Platform Headers
#include <windows.h>
#include <wingdi.h>

// Third Party Headers
#include <glad.h>
#include <win32_window.h>
#include <win32_midi.h>
#include <win32_note.cpp>

// STD Headers
#include <sstream>
#include <string>
#include <set>
#include <algorithm>
#include <iostream>

// PSC - Refactor this to try and remove any global references
static struct MIDI_PIANO {
	HGLRC openGlRenderingContext;
	Win32Midi* midi;
} MIDI_PIANO;

// ASC - Add a way to create a window generically
// Also move these down to where they're used, should not be global
static int globalWindowWidth = 300;
static int globalWindowHeight = 100;

struct OpenGlInitException : public std::exception
{
	const char* what() const throw()
	{
		return "OpenGlInitException";
	}
};

// PSC - We should move OpenGL code to the platform specific layer for the moment
static bool loadWithLoadGLLoader()
{
	if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress))
	{
		OutputDebugString(L"Failed to load OpenGL library using gladLoadGLLoader\n");
		return false;
	}

	OutputDebugString(L"Loaded OpenGL library using gladLoadGLLoader\n");
	return true;
}

static bool loadWithGladLoadGL()
{
	if (!gladLoadGL())
	{
		OutputDebugString(L"Failed to load OpenGL library using gladLoadGL\n");
		return false;
	}

	OutputDebugString(L"Loaded OpenGL library using gladLoadGL\n");
	return true;
}

// Might investigate difference between these two loaders again later
static void loadGL()
{
	if (loadWithGladLoadGL()) return;

	if (loadWithLoadGLLoader()) return;

	throw(OpenGlInitException());
}

PIXELFORMATDESCRIPTOR buildPixelFormatDescriptor()
{
	// TODO(aaron.meaney): document this properly later
	return PIXELFORMATDESCRIPTOR {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24, // Depth Buffer
		8,  // Stencil Buffer
		0,  // Aux Buffer
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
}

void setPixelFormat(HDC deviceContext, PIXELFORMATDESCRIPTOR* pixelFormatDescriptor)
{
	int pixelFormat = ChoosePixelFormat(deviceContext, pixelFormatDescriptor);
	SetPixelFormat(deviceContext, pixelFormat, pixelFormatDescriptor);
}

HGLRC buildOpenGlRenderingContext(HDC deviceContext)
{
	HGLRC openGlRenderingContext = wglCreateContext(deviceContext);
	
	if (openGlRenderingContext == NULL) throw(OpenGlInitException()); 

	return openGlRenderingContext;
}

void setOpenGlRenderingContext(HDC deviceContext, HGLRC* openGlRenderingContext)
{
	if (wglMakeCurrent(deviceContext, *openGlRenderingContext) == FALSE) throw OpenGlInitException();
}

class OpenGlVersion
{
private:
	int _major;
	int _minor;
public:
	OpenGlVersion()
	{
		glGetIntegerv(GL_MAJOR_VERSION, &_major);
		glGetIntegerv(GL_MINOR_VERSION, &_minor);
	}

	int major()
	{
		return _major;
	}

	int minor()
	{
		return _minor;
	}
};

// Win32OpenGlContext object?
static bool initOpenGl(HWND windowHandle)
{
	// Logger
	OutputDebugString(L"Initializing OpenGL context\n");

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = buildPixelFormatDescriptor();
	
	HDC deviceContext = GetDC(windowHandle);

	setPixelFormat(deviceContext, &pixelFormatDescriptor);

	HGLRC openGlRenderingContext;
	try
	{
		openGlRenderingContext = buildOpenGlRenderingContext(deviceContext);
	}
	catch (OpenGlInitException& e)
	{
		// Logger
		OutputDebugString(L"Failed to create OpenGL context:");
		OutputDebugStringA(e.what());
		OutputDebugString(L"\n");
		throw(OpenGlInitException());
	}

	// Logger
	OutputDebugString(L"OpenGL context created\n");

	try
	{
		setOpenGlRenderingContext(deviceContext, &openGlRenderingContext);
	}
	catch (OpenGlInitException& e)
	{
		// Logger
		OutputDebugString(L"Failed to set OpenGL context:");
		OutputDebugStringA(e.what());
		OutputDebugString(L"\n");
		throw(OpenGlInitException());
	}
	
	// Logger
	OutputDebugString(L"OpenGL context set\n");

	loadGL();

	OpenGlVersion version;

	// Logger
	std::wstringstream wss;
	wss << L"Successfully initialized OpenGL v" << version.major() << L"." << version.minor() << L"" << std::endl;
	OutputDebugStringW(wss.str().c_str());

	// Win32Window.SetOpenGlRenderingContext(...)
	MIDI_PIANO.openGlRenderingContext = openGlRenderingContext;

	glViewport(0, 0, globalWindowHeight, globalWindowWidth);

	return true;
}

// PSC - Win32RunMessageLoop
static void win32PollMessages()
{
	// Logger
	OutputDebugString(L"Running Message Loop\n");

	MSG message = {};
	while (GetMessage(&message, 0, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

// PSC - Globals, move to win32_code_defines.h ?
static HDC wtDeviceContext = 0;
static RECT* wtRectangle = 0;
static int wtLeftPadding = 0;
static int wtTopPadding = 0;
static int wtTopStep = 20;

// DEL - We don't need this for the current feature
void initWriteText(HDC deviceContext, RECT* rectangle, int leftPadding)
{
	wtDeviceContext = deviceContext;
	wtRectangle = rectangle;
	wtLeftPadding = leftPadding;
	wtTopPadding = 0;
}

// DEL - We don't need this for the current feature
void writeText(std::wstring string)
{
	ExtTextOut(wtDeviceContext, wtLeftPadding, wtTopPadding + wtTopStep, ETO_OPAQUE, wtRectangle, string.c_str(), (UINT)string.size(), NULL);
	wtTopPadding += wtTopStep;
}

// PSC
void onKeyDown(LPCSTR character)
{
	Note* note = charToNote(character);

	if (note == nullptr) return;
	
	MIDI_PIANO.midi->playNote(note);
}

// PSC
void onKeyUp(LPCSTR character)
{
	Note* note = charToNote(character);

	if (note == nullptr) return;

	MIDI_PIANO.midi->stopNote(note);
}

void win32CallbackCreate(HWND windowHandle)
{
	initOpenGl(windowHandle);
}

void win32CallbackPaint(HWND windowHandle)
{
	// This is all temporary drawing code until I implement a proper GUI system
	RECT rectangle = {};
	PAINTSTRUCT paint = {};
	HDC deviceContext = BeginPaint(windowHandle, &paint);

	SetBkMode(deviceContext, TRANSPARENT);

	// Remove text code, implement it properly at a later date
	initWriteText(deviceContext, &rectangle, 10);
	writeText(L"MIDI Piano");

	EndPaint(windowHandle, &paint);
}

void win32CallbackKeydown(HWND windowHandle, WPARAM wParam)
{
	LPCSTR character = (LPCSTR)&wParam;

	// Win32Window.emitEvent(KEY_DOWN, character)
	// Might need to create an event system?
	// The emit methods should be private, so no outside code can emit events
	onKeyDown(character);

	// Win32Window.redraw - private method
	RedrawWindow(windowHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
}

void win32CallbackKeyup(HWND windowHandle, WPARAM wParam)
{
	LPCSTR character = (LPCSTR)&wParam;

	// Win32Window.emitEvent(KEY_UP, character)
	onKeyUp(character);

	// Win32Window.redraw - private method
	RedrawWindow(windowHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
}

void win32CallbackClose()
{
	PostQuitMessage(0);
}

void win32CallbackSize(LPARAM lParam)
{
	// iirc this was temp code so we wouldn't handle it until the window was initialized
	// Maybe add a guard statement to start of the message handler for
	// return unless windowInitialized?
	if (MIDI_PIANO.openGlRenderingContext == NULL) {
		return;
	}

	// Yeah move this to another private func
	INT width = LOWORD(lParam);
	INT height = HIWORD(lParam);

	glViewport(0, 0, width, height);
}

int WINAPI wWinMain(_In_ HINSTANCE appInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	Win32Window window(appInstance, L"MIDI Piano", globalWindowWidth, globalWindowHeight);
	window.callbackCreate = win32CallbackCreate;
	window.callbackPaint = win32CallbackPaint;
	window.callbackSize = win32CallbackSize;
	window.callbackClose = win32CallbackClose;

	Win32Midi midi(window.getWindowHandle());
	MIDI_PIANO.midi = &midi; // remove MIDI_PIANO struct and replace with MidiPianoApp object that gets passed the IMidi adapter

	// Instead make this emit events on the window event systems
	// E.g.
	// appWindow.emitEvent(KEYDOWN, char)
	// appWinodw.emitEvent(KEYUP, char)
	// 
	// The MidiPiano class would then listen to this
	// appWindow.listenEvent(KEYDOWN, onKeydown)
	// appWindow.listenEvent(KEYUP, onKeyUp)
	//
	// Inside those events we can then handle the generic midi playback
	// without any platform independence
	//
	// void onKeydown(const char* character)
	// {
	//    midi->playNote(character); <-- Application layer / adapter pattern, doesn't care about platform
	// }
	window.callbackKeydown = win32CallbackKeydown;
	window.callbackKeyup = win32CallbackKeyup;

	window.show();

	win32PollMessages();

	PostQuitMessage(0);
	return 0;
}
