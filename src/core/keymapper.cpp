#include "keymapper.h"
#include <string>

namespace MidiPiano::Core
{
	// TEMP!!!
	int Keymapper::map(char character)
	{
		std::string cmp = std::string() + character;

		// NOTE(aaron.meaney): This is terrible, terrible code - replace this once I get to the proper keymapper feature
		if (cmp == "Z") return 36;
		if (cmp == "S") return 37;
		if (cmp == "X") return 38;
		if (cmp == "D") return 39;
		if (cmp == "C") return 40;
		if (cmp == "V") return 41;
		if (cmp == "G") return 42;
		if (cmp == "B") return 43;
		if (cmp == "H") return 44;
		if (cmp == "N") return 45;
		if (cmp == "J") return 46;
		if (cmp == "M") return 47;
		if (cmp == ",") return 48;
		
		return -1;
	};
}
