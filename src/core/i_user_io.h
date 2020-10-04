#ifndef I_USER_IO_H
#define I_USER_IO_H

#include <functional>

namespace MidiPiano::Core
{
	class IUserIO
	{
	public:
		std::function<void(char character)> onKeydown = 0;
		std::function<void(char character)> onKeyup = 0;
	};
}

#endif /* I_USER_IO_H */
