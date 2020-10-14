#ifndef _MIDI_PIANO_H
#define _MIDI_PIANO_H
#endif // _MIDI_PIANO_H

#include "i_midi_out.h"
#include "i_logger.h"
#include "keymapper.h"

#include <set>

namespace MidiPiano::Core
{
	class MidiPiano {
	public:
		MidiPiano(IMidiOut* midiOut, ILogger* logger);

		void onKeydown(char character);
		void onKeyup(char character);

		void silenceAllNotes();
	private:
		IMidiOut* midiOut;
		ILogger* logger;

		Keymapper keymapper;

		std::set<int> soundedNotes = {};

		void playNote(int midiNote);
		void silenceNote(int midiNote);
		bool notePlaying(int midiNote);
		bool noteSilenced(int midiNote);
	};
}
