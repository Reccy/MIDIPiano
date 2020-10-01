#include <win32_window.h>
#include <win32_create_window_exception.h>

// PUBLIC
Win32Window::Win32Window(HINSTANCE appInstance, LPCWSTR windowName, int width, int height) :
	appInstance(appInstance),
	windowName(windowName),
	width(width),
	height(height)
{
	initialize();
}

void Win32Window::show()
{
	ShowWindow(windowHandle, SW_SHOW);
}

// PRIVATE
void Win32Window::initialize()
{
	windowClass = buildWindowClass();
	windowHandle = buildWindow();
}

WNDCLASSW Win32Window::buildWindowClass()
{
	WNDCLASSW windowClass = {};
	windowClass.lpfnWndProc = windowThreadProcProxy;
	windowClass.hInstance = appInstance;
	windowClass.hbrBackground = getWindowBackground();
	windowClass.lpszClassName = getWindowClassName();
	windowClass.style = getWindowClassStyles();
	return windowClass;
}

HWND Win32Window::buildWindow()
{
	if (!RegisterClassW(&windowClass))
	{
		throw Win32CreateWindowException();
	}

	HWND result = buildWindowHandle();

	if (!result) {
		throw Win32CreateWindowException();
	}

	return result;
}

HWND Win32Window::buildWindowHandle()
{
	LPCWSTR windowClassName = windowClass.lpszClassName;
	DWORD windowStyle = getWindowStyle();
	int initialPosX = getInitialWindowPosX();
	int initialPosY = getInitialWindowPosY();
	int initialWidth = width;
	int initialHeight = height;
	HWND windowParent = NULL;
	HMENU menu = NULL;
	HINSTANCE windowInstance = windowClass.hInstance;
	LPVOID lParam = this;

	HWND result = CreateWindowW(
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

	return result;
}

const HBRUSH Win32Window::getWindowBackground()
{
	// We use the COLOR_BACKGROUND because it is the default color for a window
	return (HBRUSH)COLOR_BACKGROUND;
}

const LPCWSTR Win32Window::getWindowClassName()
{
	return L"Win32Window";
}

const UINT Win32Window::getWindowClassStyles()
{
	// CS_OWNDC is used because the same DC needs to exist while the OpenGL context exists
	return CS_OWNDC;
}

const DWORD Win32Window::getWindowStyle()
{
	// WS_OVERLAPPED: Gives the window a title bar and a border
	// WS_MINIMIZEBOX: Gives the window a minimize button
	// WS_SYSMENU: Gives the window a menu for minimize and close buttons
	return WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
}

const int Win32Window::getInitialWindowPosX()
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	return screenWidth / 2 - width;
}

const int Win32Window::getInitialWindowPosY()
{
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	return screenHeight / 2 - height;
}

// Stores the pointer to the Win32Window instance inside the Window context when WM_NCCREATE is received.
// When other messages are received, retrieves the pointer, dereferences and casts it to Win32Window.
// It then calls the normal window proc on the instance.
LRESULT Win32Window::windowThreadProcProxy(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCCREATE)
	{
		// Logger
		OutputDebugString(L"WN_NCCREATE\n");
		SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*) lParam)->lpCreateParams);
	}

	Win32Window* delegate = ((Win32Window*)GetWindowLongPtr(windowHandle, GWLP_USERDATA));

	if (delegate == nullptr)
	{
		return TRUE;
	}

	return delegate->windowThreadProc(windowHandle, message, wParam, lParam);
}

LRESULT Win32Window::windowThreadProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_CREATE:
	{
		// Logger
		OutputDebugString(L"WM_CREATE\n");

		if (callbackCreate) callbackCreate(windowHandle);
		
		break;
	}
	case WM_PAINT:
	{
		// Logger
		OutputDebugString(L"WM_PAINT\n");

		if (callbackPaint) callbackPaint(windowHandle);

		break;
	}
	case WM_KEYDOWN:
	{
		// Logger
		OutputDebugString(L"WM_KEYDOWN\n");

		if (callbackKeydown) callbackKeydown(windowHandle, wParam);

		break;
	}
	case WM_KEYUP:
	{
		// Logger
		OutputDebugString(L"WM_KEYUP\n");

		if (callbackKeyup) callbackKeyup(windowHandle, wParam);

		break;
	}
	case WM_SIZE:
	{
		// Logger
		OutputDebugString(L"WM_SIZE\n");

		if (callbackSize) callbackSize(lParam);

		break;
	}
	case WM_CLOSE:
	{
		// Logger
		OutputDebugString(L"WM_CLOSE\n");

		if (callbackClose) callbackClose();

		break;
	}
	}

	return DefWindowProc(windowHandle, message, wParam, lParam);
}
