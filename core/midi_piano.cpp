#include "midi_piano.h"
#include <string>

int MidiPiano::test_func() {
	std::string str{ "Hello" };
	str.swap(str); // This should get flagged
	return 300;
}
