#ifndef WIN32_WINDOW_CREATE_EXCEPTION_H
#define WIN32_WINDOW_CREATE_EXCEPTION_H

struct Win32CreateWindowException : public std::exception
{
	const char* what() const throw()
	{
		return "Win32CreateWindowException";
	}
};

#endif /* WIN32_WINDOW_CREATE_EXCEPTION_H */