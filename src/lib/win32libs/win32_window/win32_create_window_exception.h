#ifndef WIN32_CREATE_WINDOW_EXCEPTION_H
#define WIN32_CREATE_WINDOW_EXCEPTION_H

struct Win32CreateWindowException : public std::exception
{
	const char* what() const throw()
	{
		return "Win32CreateWindowException";
	}
};

#endif /* WIN32_CREATE_WINDOW_EXCEPTION_H */
