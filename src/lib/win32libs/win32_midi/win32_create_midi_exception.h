#ifndef WIN32_CREATE_MIDI_EXCEPTION_H
#define WIN32_CREATE_MIDI_EXCEPTION_H

#include <stdexcept>

struct Win32CreateMidiException : public std::exception
{
	const char* what() const throw()
	{
		return "Win32CreateMidiException";
	}
};

#endif /* WIN32_CREATE_MIDI_EXCEPTION_H */
