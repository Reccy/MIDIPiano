#ifndef WIN32_WINDOW_H
#define WIN32_WINDOW_h

#include <windows.h>
#include <functional>

class Win32Window
{
public:
	Win32Window(HINSTANCE appInstance, LPCWSTR windowName, int width, int height);
	void show();

	std::function<void(HWND windowHandle)> callbackCreate = {};
	std::function<void(HWND windowHandle)> callbackPaint = {};
	std::function<void(HWND windowHandle, WPARAM wParam)> callbackKeydown = {};
	std::function<void(HWND windowHandle, WPARAM wParam)> callbackKeyup = {};
	std::function<void()> callbackClose = {};
	std::function<void(LPARAM lParam)> callbackSize = {};
	std::function<void(LPARAM lParam)> callbackGetMinMaxInfo = {};

	HWND windowHandle = {}; // This is a temporary public windowHandle until usage of it is removed from main
private:
	HINSTANCE appInstance = {};
	WNDCLASSW windowClass = {};
	LPCWSTR windowName = {};

	int width = {};
	int height = {};

	void initialize();

	WNDCLASSW buildWindowClass();
	HWND buildWindow();
	HWND buildWindowHandle();

	const HBRUSH getWindowBackground();
	const LPCWSTR getWindowClassName();
	const UINT getWindowClassStyles();
	const int getInitialWindowPosX();
	const int getInitialWindowPosY();
	const DWORD getWindowStyle();

	static LRESULT windowThreadProcProxy(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK windowThreadProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif /* WIN32_WINDOW_H */
