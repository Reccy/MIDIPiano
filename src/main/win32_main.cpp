// PSC - Platform Specific Code, move to Win32/64 module
// ASC - App Specific Code, move to app logic core module
#ifndef UNICODE
#define UNICODE
#endif

// Platform Headers
#include <windows.h>
#include <wingdi.h>

// Third Party Headers
#include <glad.h>
#include <win32_driver_info.h>

// STD Headers
#include <sstream>
#include <string>
#include <set>
#include <algorithm>
#include <iostream>


// PSC - Refactor this to try and remove any global references
static struct MIDI_PIANO {
	HINSTANCE appInstance;
	HGLRC openGlRenderingContext;
	HWND windowHandle;
	HMIDIOUT midiOutHandle;
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

// window(private)->getInitialWindowPos.x
int getInitialWindowPosX(int windowWidth)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	return screenWidth / 2 - windowWidth;
}

// window(private)->getInitialWindowPos.y
int getInitialWindowPosY(int windowHeight)
{
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	return screenHeight / 2 - windowHeight;
}

// Win32Window::CreateWindowException
struct CreateWindowException : public std::exception
{
	const char* what() const throw()
	{
		return "CreateWindowException";
	}
};

// Win32Window::getWindowStyle
DWORD getWindowStyle()
{
	// WS_OVERLAPPED: Gives the window a title bar and a border
	// WS_MINIMIZEBOX: Gives the window a minimize button
	// WS_SYSMENU: Gives the window a menu for minimize and close buttons
	return WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
}

HWND buildWindowHandle(WNDCLASS windowClass)
{
	LPCWSTR windowClassName = windowClass.lpszClassName;
	LPCWSTR windowName = L"MIDI Piano";
	DWORD windowStyle = getWindowStyle();
	int initialPosX = getInitialWindowPosX(globalWindowWidth);
	int initialPosY = getInitialWindowPosY(globalWindowHeight);
	int initialWidth = globalWindowWidth;
	int initialHeight = globalWindowHeight;
	HWND windowParent = NULL;
	HMENU menu = NULL;
	HINSTANCE windowInstance = windowClass.hInstance;
	LPVOID lParam = NULL;

	HWND windowHandle = CreateWindowW(
		windowClassName,
		windowName,
		windowStyle,
		initialPosX,
		initialPosY,
		initialWidth,
		initialHeight,
		windowParent,
		menu,
		windowInstance,
		lParam
	);

	MIDI_PIANO.windowHandle = windowHandle;

	return windowHandle;
}

// PSC - Code to create the window
// Win32Window::Win32Window -> Constructor
static HWND createWindow(WNDCLASS windowClass)
{
	if (!RegisterClass(&windowClass))
	{
		throw CreateWindowException();
	}

	HWND windowHandle = buildWindowHandle(windowClass);

	if (!windowHandle) {
		throw CreateWindowException();
	}

	return windowHandle;
}

// PSC - Win32RunMessageLoop
static void runLoop()
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

// DEL - We don't need this for the current feature
void writeBlank()
{
	writeText(L"");
}

// ASC - Move to midi.h ?
// Also consider changing this to array or other data structure
// Don't know if it'll ever be relevant to directory access a note by the name
// Could just add a mapping function?
struct Note
{
	byte id;
	const char name[8];
};

Note C0{ 0, "C0" };
Note CSharp0{ 1, "C#0" };
Note D0{ 2, "D0" };
Note DSharp0{ 3, "D#0" };
Note E0{ 4, "E0" };
Note F0{ 5, "F0" };
Note FSharp0{ 6, "F#0" };
Note G0{ 7, "G0" };
Note GSharp0{ 8, "G#0" };
Note A1{ 9, "A1" };
Note ASharp1{ 10, "A#1" };
Note B1{ 11, "B1" };
Note C1{ 12, "C1" };
Note CSharp1{ 13, "C#1" };
Note D1{ 14, "D1" };
Note DSharp1{ 15, "D#1" };
Note E1{ 16, "E1" };
Note F1{ 17, "F1" };
Note FSharp1{ 18, "F#1" };
Note G1{ 19, "G1" };
Note GSharp1{ 20, "G#1" };
Note A2{ 21, "A2" };
Note ASharp2{ 22, "A#2" };
Note B2{ 23, "B2" };
Note C2{ 24, "C2" };
Note CSharp2{ 25, "C#2" };
Note D2{ 26, "D2" };
Note DSharp2{ 27, "D#2" };
Note E2{ 28, "E2" };
Note F2{ 29, "F2" };
Note FSharp2{ 30, "F#2" };
Note G2{ 31, "G2" };
Note GSharp2{ 32, "G#2" };
Note A3{ 33, "A3" };
Note ASharp3{ 34, "A#3" };
Note B3{ 35, "B3" };
Note C3{ 36, "C3" };
Note CSharp3{ 37, "C#3" };
Note D3{ 38, "D3" };
Note DSharp3{ 39, "D#3" };
Note E3{ 40, "E3" };
Note F3{ 41, "F3" };
Note FSharp3{ 42, "F#3" };
Note G3{ 43, "G3" };
Note GSharp3{ 44, "G#3" };
Note A4{ 45, "A4" };
Note ASharp4{ 46, "A#4" };
Note B4{ 47, "B4" };
Note C4{ 48, "C4" };
Note CSharp4{ 49, "C#4" };
Note D4{ 50, "D4" };
Note DSharp4{ 51, "D#4" };
Note E4{ 52, "E4" };
Note F4{ 53, "F4" };
Note FSharp4{ 54, "F#4" };
Note G4{ 55, "G4" };
Note GSharp4{ 56, "G#4" };
Note A5{ 57, "A5" };
Note ASharp5{ 58, "A#5" };
Note B5{ 59, "B5" };
Note C5{ 60, "C5" };
Note CSharp5{ 61, "C#5" };
Note D5{ 62, "D5" };
Note DSharp5{ 63, "D#5" };
Note E5{ 64, "E5" };
Note F5{ 65, "F5" };
Note FSharp5{ 66, "F#5" };
Note G5{ 67, "G5" };
Note GSharp5{ 68, "G#5" };
Note A6{ 69, "A6" };
Note ASharp6{ 70, "A#6" };
Note B6{ 71, "B6" };
Note C6{ 72, "C6" };
Note CSharp6{ 73, "C#6" };
Note D6{ 74, "D6" };
Note DSharp6{ 75, "D#6" };
Note E6{ 76, "E6" };
Note F6{ 77, "F6" };
Note FSharp6{ 78, "F#6" };
Note G6{ 79, "G6" };
Note GSharp6{ 80, "G#6" };
Note A7{ 81, "A7" };
Note ASharp7{ 82, "A#7" };
Note B7{ 83, "B7" };
Note C7{ 84, "C7" };
Note CSharp7{ 85, "C#7" };
Note D7{ 86, "D7" };
Note DSharp7{ 87, "D#7" };
Note E7{ 88, "E7" };
Note F7{ 89, "F7" };
Note FSharp7{ 90, "F#7" };
Note G7{ 91, "G7" };
Note GSharp7{ 92, "G#7" };
Note A8{ 93, "A8" };
Note ASharp8{ 94, "A#8" };
Note B8{ 95, "B8" };
Note C8{ 96, "C8" };
Note CSharp8{ 97, "C#8" };
Note D8{ 98, "D8" };
Note DSharp8{ 99, "D#8" };
Note E8{ 100, "E8" };
Note F8{ 101, "F8" };
Note FSharp8{ 102, "F#8" };
Note G8{ 103, "G8" };
Note GSharp8{ 104, "G#8" };
Note A9{ 105, "A9" };
Note ASharp9{ 106, "A#9" };
Note B9{ 107, "B9" };
Note C9{ 108, "C9" };
Note CSharp9{ 109, "C#9" };
Note D9{ 110, "D9" };
Note DSharp9{ 111, "D#9" };
Note E9{ 112, "E9" };
Note F9{ 113, "F9" };
Note FSharp9{ 114, "F#9" };
Note G9{ 115, "G9" };
Note GSharp9{ 116, "G#9" };
Note A10{ 117, "A10" };
Note ASharp10{ 118, "A#10" };
Note B10{ 119, "B10" };
Note C10{ 120, "C10" };
Note CSharp10{ 121, "C#10" };
Note D10{ 122, "D10" };
Note DSharp10{ 123, "D#10" };
Note E10{ 124, "E10" };
Note F10{ 125, "F10" };
Note FSharp10{ 126, "F#10" };
Note G10{ 127, "G10" };

// ASC - Make sure the PSC sends in generic info to this function, so it can be handled by other platforms - define interface
// NOTE(aaron.meaney): This will essentially be the keymapper, it is hard coded at the moment
Note* stringToNote(std::string character)
{
	std::transform(character.begin(), character.end(), character.begin(), [](char c) { return static_cast<char>(std::toupper(c)); });

	Note* note{};

	// NOTE(aaron.meaney): This is terrible, terrible code - replace this once I get to the proper keymapper feature
	if (character == "Z") { note = &C3; }
	if (character == "S") { note = &CSharp3; }
	if (character == "X") { note = &D3; }
	if (character == "D") { note = &DSharp3; }
	if (character == "C") { note = &E3; }
	if (character == "V") { note = &F3; }
	if (character == "G") { note = &FSharp3; }
	if (character == "B") { note = &G3; }
	if (character == "H") { note = &GSharp3; }
	if (character == "N") { note = &A4; }
	if (character == "J") { note = &ASharp4; }
	if (character == "M") { note = &B4; }

	if (note == nullptr) {
		return nullptr;
	}

	return note;
}

// ASC - Move this also to app code layer
const int MIDI_STATUS_NOTE_ON = 0x90;
const int MIDI_STATUS_NOTE_OFF = 0x80;

// PSC - May need to be refactored too
void sendMidiMessage(DWORD message)
{
	MMRESULT response = midiOutShortMsg(MIDI_PIANO.midiOutHandle, message);

	if (response != MMSYSERR_NOERROR)
	{
		std::wstringstream ss;
		ss << L"Could not send message to MIDI device\n";
		ss << L"Error code: " << std::to_wstring(response) << std::endl;

		// Logger
		OutputDebugString(ss.str().c_str());

		// Dialog
		MessageBox(MIDI_PIANO.windowHandle, ss.str().c_str(), L"FATAL ERROR", MB_OK | MB_ICONERROR);
		exit(1);
	}
}

// PSC - Should make this win32 play MIDI?
void playNote(Note* note)
{
	// Keep this :D
	union {
		DWORD word;
		BYTE bytes[4];
	} midiMessage;

	midiMessage.bytes[0] = MIDI_STATUS_NOTE_ON;
	midiMessage.bytes[1] = note->id;
	midiMessage.bytes[2] = 127;
	midiMessage.bytes[3] = 0;

	// Consider using a MIDI Stream instead?
	// Slight latency with current method
	// https://docs.microsoft.com/en-us/windows/win32/multimedia/managing-midi-data-blocks
	// Might need to use a double buffer and use it this way to prevent the audio latency
	sendMidiMessage(midiMessage.word);
}

// PSC - Should make this win32 stop MIDI?
void stopNote(Note* note)
{
	// Keep this :D
	union {
		DWORD word;
		BYTE bytes[4];
	} midiMessage;

	midiMessage.bytes[0] = MIDI_STATUS_NOTE_OFF;
	midiMessage.bytes[1] = note->id;
	midiMessage.bytes[2] = 127;
	midiMessage.bytes[3] = 0;

	sendMidiMessage(midiMessage.word);
}

// PSC
void onKeyDown(LPCSTR character)
{
	Note* note = stringToNote(character);

	if (note == nullptr) return;
	
	if (MIDI_PIANO.midiOutHandle != NULL)
	{
		playNote(note);
	}
}

// PSC
void onKeyUp(LPCSTR character)
{
	Note* note = stringToNote(character);

	if (note == nullptr) return;

	if (MIDI_PIANO.midiOutHandle != NULL)
	{
		stopNote(note);
	}
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

void win32CallbackGetMinMaxInfo(LPARAM lParam)
{
	LPMINMAXINFO minMaxInfo = (LPMINMAXINFO)lParam;

	minMaxInfo->ptMinTrackSize.x = globalWindowWidth;
	minMaxInfo->ptMinTrackSize.y = globalWindowHeight;

	minMaxInfo->ptMaxTrackSize.x = globalWindowWidth;
	minMaxInfo->ptMaxTrackSize.y = globalWindowHeight;
}

// PSC - Send to win32 main?
// Should also make smaller functions in each case statement - inline?
LRESULT CALLBACK MidiPianoMainWindowCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Create a dispatchMessage function - maybe there's a design pattern for this?
	// return dispatchMessage(windowHandle, message... etc
	switch (message) {
	case WM_CREATE:
	{
		// Logger
		OutputDebugString(L"WM_CREATE\n");

		win32CallbackCreate(windowHandle);

		break;
	}
	case WM_PAINT:
	{
		// Logger
		OutputDebugString(L"WM_PAINT\n");

		win32CallbackPaint(windowHandle);

		break;
	}
	case WM_KEYDOWN:
	{
		// Logger
		OutputDebugString(L"WM_KEYDOWN\n");

		win32CallbackKeydown(windowHandle, wParam);

		break;
	}
	case WM_KEYUP:
	{
		// Logger
		OutputDebugString(L"WM_KEYUP\n");

		win32CallbackKeyup(windowHandle, wParam);

		break;
	}
	case WM_CLOSE:
	{
		// Logger
		OutputDebugString(L"WM_CLOSE\n");

		win32CallbackClose();

		break;
	}
	case WM_SIZE:
	{
		// Logger
		OutputDebugString(L"WM_SIZE\n");

		win32CallbackSize(lParam);

		break;
	}
	case WM_GETMINMAXINFO:
	{
		// Logger
		OutputDebugString(L"WM_GETMINMAXINFO");

		win32CallbackGetMinMaxInfo(lParam);
		
		break;
	}
	}

	return DefWindowProc(windowHandle, message, wParam, lParam);
}

std::string getDriverVersion(MMVERSION version)
{
	// Create inline function to get high and low words?
	int minor = HIWORD(version);
	int major = LOWORD(version);

	std::stringstream ss;
	ss << "v" << major << "." << minor;

	return ss.str();
}

// Win32Window::NoMidiDevicesFoundException
struct NoMidiDevicesFoundException : public std::exception
{
	const char* what() const throw()
	{
		return "NoMidiDevicesFoundException";
	}
};

bool isMidiDeviceWin32Generic(MIDIOUTCAPS midi)
{
	return midi.wPid == MM_MSFT_GENERIC_MIDISYNTH && midi.wTechnology == MOD_SWSYNTH;
}

// Remove last param when moved to a class
void logMidiDevice(MIDIOUTCAPS midi, unsigned int midiDeviceIndex, int midiDeviceCount)
{
	std::string manufacturer = getManufacturerName(midi.wMid);
	std::string product = getProductName(midi.wPid);
	std::string driverVersion = getDriverVersion(midi.vDriverVersion);
	std::wstring productName = midi.szPname;
	std::string technology = getTechnology(midi.wTechnology);

	// This should be moved into like a print function or something
	std::wstringstream ss;
	ss << L"==========\nFound device " << std::to_wstring(midiDeviceIndex + 1) << L" of " << std::to_wstring(midiDeviceCount) << "\n";
	ss << "Manufacturer: " << manufacturer.c_str() << "\n";
	ss << "Product: " << product.c_str() << "\n";
	ss << "Driver Version: " << driverVersion.c_str() << "\n";
	ss << "Product Name: " << productName << "\n";
	ss << "Technology: " << technology.c_str() << "\n";
	ss << "Voices: " << midi.wVoices << "\n";
	ss << "Channel Mask: " << midi.wChannelMask << "\n";
	ss << "Support: " << midi.dwSupport << "\n";
	ss << std::endl;

	// Logger
	OutputDebugString(ss.str().c_str());
}

int findWin32MidiDevice()
{
	const unsigned int midiDeviceCount = midiOutGetNumDevs();

	for (unsigned int midiDeviceIndex = 0; midiDeviceIndex < midiDeviceCount; midiDeviceIndex++)
	{
		MIDIOUTCAPS midiOutCaps = {};
		midiOutGetDevCaps(midiDeviceIndex, &midiOutCaps, sizeof(midiOutCaps));

		logMidiDevice(midiOutCaps, midiDeviceIndex, midiDeviceCount);

		if (isMidiDeviceWin32Generic(midiOutCaps))
		{
			return midiDeviceIndex;
		}
	}

	throw(NoMidiDevicesFoundException());
}

void openMidiDevice(HWND windowHandle, unsigned int win32MidiDeviceId)
{
	// begin func openMidiDevice
	HMIDIOUT midiOutHandle = {};
	MMRESULT midiOpenResult = midiOutOpen(&midiOutHandle, win32MidiDeviceId, NULL, NULL, CALLBACK_NULL); // Change NULL to MidiOutProc later - is this needed?

	std::wstringstream ss;

	if (midiOpenResult != MMSYSERR_NOERROR)
	{
		// func raiseMidiDeviceException ?
		ss.str(L"");
		ss << L"Could not open MIDI device \n";
		ss << L"Error code: " << std::to_wstring(midiOpenResult) << std::endl;

		// Logger
		OutputDebugString(ss.str().c_str());
		// Dialog box
		MessageBox(windowHandle, ss.str().c_str(), L"FATAL ERROR", MB_OK | MB_ICONERROR);

		// Should exit be handled by a singleton class?
		// E.g. MIDI_PIANO_FATAL();
		exit(1);
	}

	MIDI_PIANO.midiOutHandle = midiOutHandle;
}

//PSC - Send to Win32 Main
// Should break a lot of this into smaller functions
// Could also make it an object? 
// Win32GenericMidi
void loadMidi(HWND windowHandle)
{
	int win32MidiDeviceId = findWin32MidiDevice();
	openMidiDevice(windowHandle, win32MidiDeviceId);
}

const HBRUSH getWindowBackground()
{
	// We use the COLOR_BACKGROUND because it is the default color for a window
	return (HBRUSH)COLOR_BACKGROUND;
}

const LPCWSTR getWindowClassName()
{
	return L"MidiPianoWindowClass";
}

const UINT getWindowClassStyles()
{
	// CS_OWNDC is used because the same DC needs to exist while the OpenGL context exists
	return CS_OWNDC;
}

WNDCLASS buildWindowClass(HINSTANCE appInstance)
{
	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = MidiPianoMainWindowCallback;
	windowClass.hInstance = appInstance;
	windowClass.hbrBackground = getWindowBackground();
	windowClass.lpszClassName = getWindowClassName();
	windowClass.style = getWindowClassStyles();
	return windowClass;
};

// PSC - Win main
// Move more of this logic into smaller functions
// Could also have winMain just delegate to the app main
// wWinMain() -> MIDIPiano::main()
int WINAPI wWinMain(_In_ HINSTANCE appInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	MIDI_PIANO.appInstance = appInstance;

	WNDCLASS windowClass = buildWindowClass(appInstance);
	
	// PSC - All OutputDebugString calls should be wrapped in a logger class outside of platform code
	OutputDebugString(L"Acquiring window handle...\n");

	// PSC - Window creation logic should be wrapped in a generic platform OO layer
	// Something like midiWindow.create() or windowFactory.platform(WIN_32).title('...') ???
	HWND windowHandle;
	try
	{
		windowHandle = createWindow(windowClass);
	}
	catch (CreateWindowException& e)
	{
		// Logger
		OutputDebugString(L"Failed to acquire a window handle: ");
		OutputDebugStringA(e.what());
		OutputDebugString(L"\n");
		exit(1);
	}
	
	// Logger
	OutputDebugString(L"Window handle acquired\n");

	// Rename to initMidi?
	// Could also have in private function inside the win32Window class?
	// Win32Window(private)::initMidi();

	try
	{
		loadMidi(windowHandle);
	}
	catch (NoMidiDevicesFoundException& e)
	{
		// Logger
		OutputDebugString(L"Could not find valid MIDI Output Device: ");
		OutputDebugStringA(e.what());
		OutputDebugString(L"\n");

		// Make a dialog box class?
		// win32Dialog.show(L"Could not find...")
		// also make them constant vars?
		MessageBox(windowHandle, L"Could not find valid MIDI Output Device.\nThis program will now exit.", L"FATAL ERROR", MB_OK | MB_ICONERROR);
		exit(1);
	}

	// Logger
	OutputDebugString(L"Midi handle acquired\n");

	// Show window should be handled by app logic intead?
	// win32Window.show();
	ShowWindow(windowHandle, SW_SHOW);

	// Logger
	OutputDebugString(L"Window displayed\n");

	// Rename to something like pollSystemMessages
	runLoop();

	// Put this in destructor?
	PostQuitMessage(0);
	return 0;
}
