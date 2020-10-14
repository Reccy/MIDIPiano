#ifndef WIN32_OPENGL_H
#define WIN32_OPENGL_H

#ifndef UNICODE
#define UNICODE
#endif /* UNICODE */

#include <Windows.h>
#include <exception>
#include <win32_opengl_version.h>

class Win32OpenGl
{
public:
	Win32OpenGl(HWND windowHandle, int windowWidth, int windowHeight);
	void updateViewport(int, int, int width, int height);
private:
	HWND windowHandle;
	int windowWidth;
	int windowHeight;
	HGLRC renderingContext;
	Win32OpenGlVersion version;

	void initialize();
	PIXELFORMATDESCRIPTOR buildPixelFormatDescriptor();
	void setPixelFormat(HDC deviceContext, PIXELFORMATDESCRIPTOR* pixelFormatDescriptor);
	HGLRC buildRenderingContext(HDC deviceContext);
	void setRenderingContext(HDC deviceContext, HGLRC* openGlRenderingContext);
	void loadGL();
	bool loadWithLoadGLLoader();
	bool loadWithGladLoadGL();
	void setVersion();
};

#endif /* WIN32_OPENGL_H */
