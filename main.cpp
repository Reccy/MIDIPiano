#include <iostream>
#include "midi_piano.h"

int main() {
	MidiPiano piano;
	std::cout <<
		"Hello, Piano!" << "\n" <<
		"You are returning number " << piano.test() <<
		std::endl;

};
