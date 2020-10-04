#ifndef WIN32_OPEN_GL_INIT_EXCEPTION_H
#define WIN32_OPEN_GL_INIT_EXCEPTION_H

struct OpenGlInitException : public std::exception
{
	const char* what() const throw()
	{
		return "OpenGlInitException";
	}
};

#endif WIN32_OPEN_GL_INIT_EXCEPTION_H
