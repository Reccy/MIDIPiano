#ifndef UNICODE
#define UNICODE
#endif

#pragma warning(push)
#pragma warning(disable: 4244)
#include <windows.h>
#include <wingdi.h>

#include <cassert>

#include <sstream>
#include <string>
#include <set>
#include <algorithm>
#include <iostream>

#include "glad.h"
#pragma warning(pop)

#define global static
#define internal static

global struct MIDI_PIANO {
	HINSTANCE appInstance;
	HGLRC openGlRenderingContext;
} MIDI_PIANO;

global int globalWindowWidth = 400;
global int globalWindowHeight = 300;

internal bool loadWithLoadGLLoader()
{
	if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress))
	{
		OutputDebugString(L"Failed to load OpenGL library using gladLoadGLLoader\n");
		return false;
	}

	OutputDebugString(L"Loaded OpenGL library using gladLoadGLLoader\n");
	return true;
}

internal bool loadWithGladLoadGL()
{
	if (!gladLoadGL())
	{
		OutputDebugString(L"Failed to load OpenGL library using gladLoadGL\n");
		return false;
	}

	OutputDebugString(L"Loaded OpenGL library using gladLoadGL\n");
	return true;
}

internal int loadGL()
{
	if (loadWithGladLoadGL())
	{
		return true;
	}

	if (loadWithLoadGLLoader())
	{
		return true;
	}

	return false;
}

internal bool initOpenGl(HWND windowHandle)
{
	OutputDebugString(L"Initializing OpenGL context\n");
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor =
	{
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

	HDC deviceContext = GetDC(windowHandle);

	int pixelFormat = ChoosePixelFormat(deviceContext, &pixelFormatDescriptor);
	SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDescriptor);

	HGLRC openGlRenderingContext = wglCreateContext(deviceContext);
	if (openGlRenderingContext == NULL)
	{
		OutputDebugString(L"Failed to create OpenGL context\n");
		return false;
	}
	OutputDebugString(L"OpenGL context created\n");

	if (wglMakeCurrent(deviceContext, openGlRenderingContext) == FALSE)
	{
		OutputDebugString(L"Failed to set OpenGL context\n");
		return false;
	}
	OutputDebugString(L"OpenGL context set\n");

	if (!loadGL())
	{
		return false;
	}

	int versionMajor;
	int versionMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
	glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

	std::wstringstream wss;
	wss << L"Successfully initialized OpenGL v" << versionMajor << L"." << versionMinor << L"" << std::endl;
	OutputDebugStringW(wss.str().c_str());

	MIDI_PIANO.openGlRenderingContext = openGlRenderingContext;

	glViewport(0, 0, globalWindowHeight, globalWindowWidth);

	ReleaseDC(windowHandle, deviceContext);

	ShowWindow(windowHandle, SW_SHOW);

	return true;
}

internal HWND createWindow(WNDCLASS windowClass)
{
	if (!RegisterClass(&windowClass))
	{
		return NULL;
	}

	HWND windowHandle = CreateWindowW(
		windowClass.lpszClassName,
		L"MIDI Piano",
		WS_OVERLAPPED | WS_CAPTION | WS_VISIBLE | WS_MINIMIZEBOX | WS_SYSMENU,
		NULL,
		NULL,
		globalWindowWidth,
		globalWindowHeight,
		NULL,
		NULL,
		windowClass.hInstance,
		NULL
	);

	if (!windowHandle) {
		return NULL;
	}

	return windowHandle;
}

internal void runLoop()
{
	OutputDebugString(L"Running Message Loop");

	MSG message = {};
	while (GetMessage(&message, 0, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

global HDC wtDeviceContext = 0;
global RECT* wtRectangle = 0;
global int wtLeftPadding = 0;
global int wtTopPadding = 0;
global int wtTopStep = 20;

void initWriteText(HDC deviceContext, RECT* rectangle, int leftPadding)
{
	wtDeviceContext = deviceContext;
	wtRectangle = rectangle;
	wtLeftPadding = leftPadding;
	wtTopPadding = 0;
}

void writeText(std::wstring string)
{
	ExtTextOut(wtDeviceContext, wtLeftPadding, wtTopPadding + wtTopStep, ETO_OPAQUE, wtRectangle, string.c_str(), (UINT)string.size(), NULL);
	wtTopPadding += wtTopStep;
}

void writeBlank()
{
	writeText(L"");
}

struct Note
{
	int id;			// 32 bits (4-byte aligned)
	char name[4];	// 32 bits (4-byte aligned)
};

Note A0{ 0, "A0" };
Note ASharp0{ 1, "A#0" };
Note B0{ 2, "B0" };
Note C1{ 3, "C1" };
Note CSharp1{ 4, "C#1" };
Note D1{ 5, "D1" };
Note DSharp1{ 6, "D#1" };
Note E1{ 7, "E1" };
Note F1{ 8, "F1" };
Note FSharp1{ 9, "F#1" };
Note G1{ 10, "G1" };
Note GSharp1{ 11, "G#1" };
Note A1{ 12, "A1" };
Note ASharp1{ 13, "A#1" };
Note B1{ 14, "B1" };
Note C2{ 15, "C2" };
Note CSharp2{ 16, "C#2" };
Note D2{ 17, "D2" };
Note DSharp2{ 18, "D#2" };
Note E2{ 19, "E2" };
Note F2{ 20, "F2" };
Note FSharp2{ 21, "F#2" };
Note G2{ 22, "G2" };
Note GSharp2{ 23, "G#2" };
Note A2{ 24, "A2" };
Note ASharp2{ 25, "A#2" };
Note B2{ 26, "B2" };
Note C3{ 27, "C3" };
Note CSharp3{ 28, "C#3" };
Note D3{ 29, "D3" };
Note DSharp3{ 30, "D#3" };
Note E3{ 31, "E3" };
Note F3{ 32, "F3" };
Note FSharp3{ 33, "F#3" };
Note G3{ 34, "G3" };
Note GSharp3{ 35, "G#3" };
Note A3{ 36, "A3" };
Note ASharp3{ 37, "A#3" };
Note B3{ 38, "B3" };
Note C4{ 39, "C4" };
Note CSharp4{ 40, "C#4" };
Note D4{ 41, "D4" };
Note DSharp4{ 42, "D#4" };
Note E4{ 43, "E4" };
Note F4{ 44, "F4" };
Note FSharp4{ 45, "F#4" };
Note G4{ 46, "G4" };
Note GSharp4{ 47, "G#4" };
Note A4{ 48, "A4" };
Note ASharp4{ 49, "A#4" };
Note B4{ 50, "B4" };
Note C5{ 51, "C5" };
Note CSharp5{ 52, "C#5" };
Note D5{ 53, "D5" };
Note DSharp5{ 54, "D#5" };
Note E5{ 55, "E5" };
Note F5{ 56, "F5" };
Note FSharp5{ 57, "F#5" };
Note G5{ 58, "G5" };
Note GSharp5{ 59, "G#5" };
Note A5{ 60, "A5" };
Note ASharp5{ 61, "A#5" };
Note B5{ 62, "B5" };
Note C6{ 63, "C6" };
Note CSharp6{ 64, "C#6" };
Note D6{ 65, "D6" };
Note DSharp6{ 66, "D#6" };
Note E6{ 67, "E6" };
Note F6{ 68, "F6" };
Note FSharp6{ 69, "F#6" };
Note G6{ 70, "G6" };
Note GSharp6{ 71, "G#6" };
Note A6{ 72, "A6" };
Note ASharp6{ 73, "A#6" };
Note B6{ 74, "B6" };
Note C7{ 75, "C7" };
Note CSharp7{ 76, "C#7" };
Note D7{ 77, "D7" };
Note DSharp7{ 78, "D#7" };
Note E7{ 79, "E7" };
Note F7{ 80, "F7" };
Note FSharp7{ 81, "F#7" };
Note G7{ 82, "G7" };
Note GSharp7{ 83, "G#7" };
Note A7{ 84, "A7" };
Note ASharp7{ 85, "A#7" };
Note B7{ 86, "B7" };
Note C8{ 87, "C8" };

std::set<Note*> sustained_notes {};

void playNote(Note*)
{
	return;
}

// NOTE(aaron.meaney): This will essentially be the keymapper, it is hard coded at the moment
Note* stringToNote(std::string character)
{
	std::transform(character.begin(), character.end(), character.begin(), std::toupper);

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
	if (character == "N") { note = &A3; }
	if (character == "J") { note = &ASharp3; }
	if (character == "M") { note = &B3; }

	if (note == nullptr) {
		return nullptr;
	}

	return note;
}

LRESULT CALLBACK MidiPianoMainWindowCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (message) {
		case WM_CREATE:
		{
			OutputDebugString(L"WM_CREATE\n");

			if (!initOpenGl(windowHandle))
			{
				return -1;
			}

			break;
		}
		case WM_PAINT:
		{
			OutputDebugString(L"WM_PAINT\n");

			// This is all temporary drawing code until I implement a proper GUI system

			RECT rectangle = {};
			PAINTSTRUCT paint = {};
			HDC deviceContext = BeginPaint(windowHandle, &paint);

			SetBkMode(deviceContext, TRANSPARENT);

			initWriteText(deviceContext, &rectangle, 10);
			writeText(L"MIDI Piano");
			writeText(L"Version 0.0.1 ALPHA");
			writeBlank();
			writeText(L"By Aaron Meaney");
			writeText(L"13/09/2020");

			std::wstringstream ss;

			for (auto it = sustained_notes.begin(); it != sustained_notes.end(); ++it)
			{
				Note* note = *it;
				ss << note->name;
				ss << " ";
			}

			writeText(ss.str().c_str());

			EndPaint(windowHandle, &paint);
			break;
		}
		case WM_KEYDOWN:
		{
			LPCSTR character = (LPCSTR)&wParam;
			Note* note = stringToNote(character);

			if (note != nullptr)
			{
				sustained_notes.insert(note);
			}

			RedrawWindow(windowHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);

			break;
		}
		case WM_KEYUP:
		{
			LPCSTR character = (LPCSTR)&wParam;
			Note* note = stringToNote(character);

			if (note != nullptr)
			{
				sustained_notes.erase(note);
			}

			RedrawWindow(windowHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);

			break;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}
		case WM_SIZE:
		{
			OutputDebugString(L"WM_SIZE\n");

			if (MIDI_PIANO.openGlRenderingContext == NULL) {
				break;
			}

			INT width = LOWORD(lParam);
			INT height = HIWORD(lParam);

			glViewport(0, 0, width, height);

			break;
		}
		case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO minMaxInfo = (LPMINMAXINFO)lParam;
			minMaxInfo->ptMinTrackSize.x = globalWindowWidth;
			minMaxInfo->ptMinTrackSize.y = globalWindowHeight;

			minMaxInfo->ptMaxTrackSize.x = globalWindowWidth;
			minMaxInfo->ptMaxTrackSize.y = globalWindowHeight;

			break;
		}
		default:
		{
			result = DefWindowProc(windowHandle, message, wParam, lParam);
		}
	}

	return result;
}

int WINAPI wWinMain(_In_ HINSTANCE appInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	MIDI_PIANO.appInstance = appInstance;

	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = MidiPianoMainWindowCallback;
	windowClass.hInstance = appInstance;
	windowClass.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	windowClass.lpszClassName = L"MidiPianoWindowClass";
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	OutputDebugString(L"Acquiring window handle...\n");
	HWND windowHandle = createWindow(windowClass);
	if (windowHandle == NULL) {
		OutputDebugString(L"Failed to acquire a window handle\n");
		return -1;
	}
	OutputDebugString(L"Window handle acquired\n");

	runLoop();

	PostQuitMessage(0);
	return 0;
}
