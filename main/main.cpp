#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <sstream>
#include <string.h>
#include <wingdi.h>
#include <iostream>
#include "glad.h"
#include <cassert>

#define global static
#define internal static

global struct MIDI_PIANO {
	HINSTANCE appInstance;
	HGLRC openGlRenderingContext;
} MIDI_PIANO;

global int globalWindowWidth = 400;
global int globalWindowHeight = 250;

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

void handleCharacter(LPCSTR character)
{
	OutputDebugStringA(character);
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
			writeBlank();
			writeText(L"Please use your keyboard to play musical tones");
			writeText(L"C C# D D# E F F# G G# A A# B");
			writeText(L"Z S  X D  C V G  B H  N J  M");

			EndPaint(windowHandle, &paint);
			break;
		}
		case WM_CHAR:
		{
			OutputDebugString(L"WM_CHAR\n");

			LPCSTR c = (LPCSTR)&wParam;

			handleCharacter(c);
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
