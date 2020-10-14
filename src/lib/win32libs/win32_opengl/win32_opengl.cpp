#include <win32_opengl.h>
#include <Windows.h>
#include <sstream>
#include <win32_open_gl_init_exception.h>
#include "../../commonlibs/glad/glad.h"

// PUBLIC

Win32OpenGl::Win32OpenGl(HWND windowHandle, int windowWidth, int windowHeight) : windowHandle(windowHandle), windowWidth(windowWidth), windowHeight(windowHeight)
{
	initialize();
}

void Win32OpenGl::updateViewport(int, int, int width, int height)
{
	glViewport(0, 0, width, height);
}

// PRIVATE

void Win32OpenGl::initialize()
{
	// Logger
	OutputDebugString(L"Initializing OpenGL context\n");

	HDC deviceContext = GetDC(windowHandle);

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = buildPixelFormatDescriptor();

	setPixelFormat(deviceContext, &pixelFormatDescriptor);

	renderingContext = buildRenderingContext(deviceContext);
	
	// Logger
	OutputDebugString(L"OpenGL context created\n");
	
	setRenderingContext(deviceContext, &renderingContext);

	// Logger
	OutputDebugString(L"OpenGL context set\n");

	loadGL();

	setVersion();

	// Logger
	std::wstringstream wss;
	wss << L"Successfully initialized OpenGL v" << version.major << L"." << version.minor << L"" << std::endl;
	OutputDebugString(wss.str().c_str());

	updateViewport(0, 0, windowWidth, windowHeight);
}

PIXELFORMATDESCRIPTOR Win32OpenGl::buildPixelFormatDescriptor()
{
	// TODO(aaron.meaney): document this properly later
	return PIXELFORMATDESCRIPTOR{
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

void Win32OpenGl::setPixelFormat(HDC deviceContext, PIXELFORMATDESCRIPTOR* pixelFormatDescriptor)
{
	int pixelFormat = ChoosePixelFormat(deviceContext, pixelFormatDescriptor);
	SetPixelFormat(deviceContext, pixelFormat, pixelFormatDescriptor);
}

HGLRC Win32OpenGl::buildRenderingContext(HDC deviceContext)
{
	HGLRC openGlRenderingContext = wglCreateContext(deviceContext);

	if (openGlRenderingContext == NULL)
	{
		OutputDebugString(L"Failed to build OpenGL rendering context\n");
		throw(OpenGlInitException());
	}

	return openGlRenderingContext;
}

void Win32OpenGl::setRenderingContext(HDC deviceContext, HGLRC* openGlRenderingContext)
{
	BOOL success = wglMakeCurrent(deviceContext, *openGlRenderingContext);

	if (!success)
	{
		// Logger
		OutputDebugString(L"Failed to set OpenGL context \n");
		throw(OpenGlInitException());
	}

	return;
}

// Might investigate difference between these two loaders again later
void Win32OpenGl::loadGL()
{
	if (loadWithGladLoadGL()) return;

	if (loadWithLoadGLLoader()) return;

	throw(OpenGlInitException());
}

// PSC - We should move OpenGL code to the platform specific layer for the moment
bool Win32OpenGl::loadWithLoadGLLoader()
{
	if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress))
	{
		OutputDebugString(L"Failed to load OpenGL library using gladLoadGLLoader\n");
		return false;
	}

	OutputDebugString(L"Loaded OpenGL library using gladLoadGLLoader\n");
	return true;
}

bool Win32OpenGl::loadWithGladLoadGL()
{
	if (!gladLoadGL())
	{
		OutputDebugString(L"Failed to load OpenGL library using gladLoadGL\n");
		return false;
	}

	OutputDebugString(L"Loaded OpenGL library using gladLoadGL\n");
	return true;
}

void Win32OpenGl::setVersion()
{
	glGetIntegerv(GL_MAJOR_VERSION, &version.major);
	glGetIntegerv(GL_MINOR_VERSION, &version.minor);
}
