#ifndef _MIDI_PIANO_H
#define _MIDI_PIANO_H
#endif // _MIDI_PIANO_H

#include "i_midi_out.h"
#include "i_user_io.h"
#include "i_logger.h"
#include "keymapper.h"

#include <set>

namespace MidiPiano::Core
{
	class MidiPiano {
	public:
		MidiPiano(IMidiOut* midiOut, IUserIO* userIO, ILogger* logger);
		int test_func();
	private:
		IMidiOut* midiOut;
		IUserIO* userIO;
		ILogger* logger;

		Keymapper keymapper;

		std::set<int> soundedNotes = {};

		void onKeyup(char character);
		void onKeydown(char character);

		bool notePlaying(int midiNote);
		bool noteSilenced(int midiNote);
	};
}
