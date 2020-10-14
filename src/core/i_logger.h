#ifndef I_LOGGER_H
#define I_LOGGER_H

#include <string>

namespace MidiPiano::Core
{
	class ILogger
	{
	public:
		virtual void log(std::wstring message) = 0;
		virtual void log(wchar_t message) = 0;

		virtual void log(std::string message) = 0;
		virtual void log(char message) = 0;
	};
}

#endif /* I_LOGGER_H */
