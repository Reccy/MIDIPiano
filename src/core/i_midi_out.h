#ifndef I_MIDI_H
#define I_MIDI_H

namespace MidiPiano::Core
{
	class IMidiOut
	{
	public:
		virtual void playNote(int midiNote) = 0;
		virtual void stopNote(int midiNote) = 0;
	};
}

#endif /* I_MIDI_H */