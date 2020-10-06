#include "midi_piano.h"

#include <string>
#include <functional>

namespace MidiPiano::Core {
	MidiPiano::MidiPiano() {} // TEMP: Delete this when writing new tests

	MidiPiano::MidiPiano(IMidiOut* midiOut, IUserIO* userIO, ILogger* logger) : 
		midiOut(midiOut),
		userIO(userIO),
		logger(logger)
	{
		userIO->onKeydown = [=](char character) {
			this->onKeydown(character);
		};

		userIO->onKeyup = [=](char character) {
			this->onKeyup(character);
		};
	}

	void MidiPiano::onKeydown(char character)
	{
		int pianoKey = keymapper.map(character);

		if (noteSilenced(pianoKey))
		{
			soundedNotes.insert(pianoKey);
			midiOut->playNote(pianoKey);
		}
	}

	void MidiPiano::onKeyup(char character)
	{
		int pianoKey = keymapper.map(character);

		if (notePlaying(pianoKey))
		{
			soundedNotes.erase(pianoKey);
			midiOut->stopNote(pianoKey);
		}
	}

	bool MidiPiano::notePlaying(int midiNote)
	{
		return (soundedNotes.find(midiNote) != soundedNotes.end());
	}

	bool MidiPiano::noteSilenced(int midiNote)
	{
		return (soundedNotes.find(midiNote) == soundedNotes.end());
	}

	int MidiPiano::test_func() {
		return 300;
	}
}
