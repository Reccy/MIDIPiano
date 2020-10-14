#include "midi_piano.h"

#include <string>
#include <functional>

namespace MidiPiano::Core {
	MidiPiano::MidiPiano(IMidiOut* midiOut, ILogger* logger) : 
		midiOut(midiOut),
		logger(logger)
	{}

	void MidiPiano::onKeydown(char character)
	{
		int midiNote = keymapper.map(character);

		if (midiNote < 0) return;

		if (noteSilenced(midiNote))
		{
			playNote(midiNote);
		}
	}

	void MidiPiano::onKeyup(char character)
	{
		int midiNote = keymapper.map(character);

		if (midiNote < 0) return;

		if (notePlaying(midiNote))
		{
			silenceNote(midiNote);
		}
	}

	void MidiPiano::silenceAllNotes()
	{
		// NOTE(aaron.meaney): We create a copy of soundedNotes as silencing a note
		// will mutate the original set while inside an iterator.
		// This will cause a data structure invalidation!
		std::set soundedNotesCopy(soundedNotes);

		for (const int midiNote : soundedNotesCopy)
		{
			silenceNote(midiNote);
		}
	}

	void MidiPiano::playNote(int midiNote)
	{
		soundedNotes.insert(midiNote);
		midiOut->playNote(midiNote);
	}

	void MidiPiano::silenceNote(int midiNote)
	{
		soundedNotes.erase(midiNote);
		midiOut->stopNote(midiNote);
	}

	bool MidiPiano::notePlaying(int midiNote)
	{
		return (soundedNotes.find(midiNote) != soundedNotes.end());
	}

	bool MidiPiano::noteSilenced(int midiNote)
	{
		return (soundedNotes.find(midiNote) == soundedNotes.end());
	}
}
