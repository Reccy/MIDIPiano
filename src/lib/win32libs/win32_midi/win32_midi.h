#ifndef WIN32_MIDI_H
#define WIN32_MIDI_H

#ifndef UNICODE
#define UNICODE
#endif

#include <Windows.h>

class Win32Midi
{
public:
	Win32Midi(HWND windowHandle);
	HMIDIOUT getHandle();

	void playNote(int midiNote);
	void stopNote(int midiNote);
private:
	Win32Midi();

	const int MIDI_STATUS_NOTE_ON = 0x90;
	const int MIDI_STATUS_NOTE_OFF = 0x80;

	const HWND windowHandle;
	HMIDIOUT midiHandle;

	void logMidiDevice(MIDIOUTCAPS, unsigned int, int);

	void initialize();
	int findWin32MidiDevice();
	bool isMidiDeviceWin32Generic(MIDIOUTCAPS midi);
	void openMidiDevice(HWND, unsigned int);
	void raiseMidiDeviceException(MMRESULT);
	void sendMidiMessage(DWORD);
};

#endif /* WIN32_MIDI_H */
