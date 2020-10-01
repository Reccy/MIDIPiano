#include <win32_midi.h>
#include <win32_create_midi_exception.h>
#include <mmreg.h>
#include <string>
#include <sstream>
#include <win32_driver_info.h>

Win32Midi::Win32Midi(HWND windowHandle) : windowHandle(windowHandle)
{
	initialize();
}

void Win32Midi::initialize()
{
	int win32MidiDeviceId = findWin32MidiDevice();
	openMidiDevice(windowHandle, win32MidiDeviceId);
}

HMIDIOUT Win32Midi::getHandle()
{
	return midiHandle;
}

int Win32Midi::findWin32MidiDevice()
{
	const unsigned int midiDeviceCount = midiOutGetNumDevs();

	for (unsigned int midiDeviceIndex = 0; midiDeviceIndex < midiDeviceCount; midiDeviceIndex++)
	{
		MIDIOUTCAPS midiOutCaps = {};
		midiOutGetDevCaps(midiDeviceIndex, &midiOutCaps, sizeof(midiOutCaps));

		logMidiDevice(midiOutCaps, midiDeviceIndex, midiDeviceCount);

		if (isMidiDeviceWin32Generic(midiOutCaps))
		{
			return midiDeviceIndex;
		}
	}

	throw(Win32CreateMidiException());
}

bool Win32Midi::isMidiDeviceWin32Generic(MIDIOUTCAPS midi)
{
	return midi.wPid == MM_MSFT_GENERIC_MIDISYNTH && midi.wTechnology == MOD_SWSYNTH;
}


void Win32Midi::openMidiDevice(HWND windowHandle, unsigned int win32MidiDeviceId)
{
	MMRESULT midiOpenResult = midiOutOpen(&midiHandle, win32MidiDeviceId, NULL, NULL, CALLBACK_NULL); // Change NULL to MidiOutProc later - is this needed?

	if (midiOpenResult == MMSYSERR_NOERROR)
	{
		return;
	}

	raiseMidiDeviceException(midiOpenResult);	
}

void Win32Midi::raiseMidiDeviceException(MMRESULT midiOpenResult)
{
	std::wstringstream ss;
	ss.str(L"");
	ss << L"Could not open MIDI device \n";
	ss << L"Error code: " << std::to_wstring(midiOpenResult) << std::endl;

	// Logger
	OutputDebugString(ss.str().c_str());
	// Dialog box
	MessageBox(windowHandle, ss.str().c_str(), L"FATAL ERROR", MB_OK | MB_ICONERROR);

	throw(Win32CreateMidiException());
}

void Win32Midi::logMidiDevice(MIDIOUTCAPS midi, unsigned int midiDeviceIndex, int midiDeviceCount)
{
	std::string manufacturer = getManufacturerName(midi.wMid);
	std::string product = getProductName(midi.wPid);
	std::string driverVersion = getDriverVersion(midi.vDriverVersion);
	std::wstring productName = midi.szPname;
	std::string technology = getTechnology(midi.wTechnology);

	// This should be moved into like a print function or something
	std::wstringstream ss;
	ss << L"==========\nFound device " << std::to_wstring(midiDeviceIndex + 1) << L" of " << std::to_wstring(midiDeviceCount) << "\n";
	ss << "Manufacturer: " << manufacturer.c_str() << "\n";
	ss << "Product: " << product.c_str() << "\n";
	ss << "Driver Version: " << driverVersion.c_str() << "\n";
	ss << "Product Name: " << productName << "\n";
	ss << "Technology: " << technology.c_str() << "\n";
	ss << "Voices: " << midi.wVoices << "\n";
	ss << "Channel Mask: " << midi.wChannelMask << "\n";
	ss << "Support: " << midi.dwSupport << "\n";
	ss << std::endl;

	// Logger
	OutputDebugString(ss.str().c_str());
}

// PSC - Should make this win32 play MIDI?
void Win32Midi::playNote(Note* note)
{
	// Keep this :D
	union {
		DWORD word;
		BYTE bytes[4];
	} midiMessage;

	midiMessage.bytes[0] = MIDI_STATUS_NOTE_ON;
	midiMessage.bytes[1] = note->id;
	midiMessage.bytes[2] = 127;
	midiMessage.bytes[3] = 0;

	// Consider using a MIDI Stream instead?
	// Slight latency with current method
	// https://docs.microsoft.com/en-us/windows/win32/multimedia/managing-midi-data-blocks
	// Might need to use a double buffer and use it this way to prevent the audio latency
	sendMidiMessage(midiMessage.word);
}

// PSC - Should make this win32 stop MIDI?
void Win32Midi::stopNote(Note* note)
{
	// Keep this :D
	union {
		DWORD word;
		BYTE bytes[4];
	} midiMessage;

	midiMessage.bytes[0] = MIDI_STATUS_NOTE_OFF;
	midiMessage.bytes[1] = note->id;
	midiMessage.bytes[2] = 127;
	midiMessage.bytes[3] = 0;

	sendMidiMessage(midiMessage.word);
}


// PSC - May need to be refactored too
void Win32Midi::sendMidiMessage(DWORD message)
{
	MMRESULT response = midiOutShortMsg(midiHandle, message);

	if (response != MMSYSERR_NOERROR)
	{
		std::wstringstream ss;
		ss << L"Could not send message to MIDI device\n";
		ss << L"Error code: " << std::to_wstring(response) << std::endl;

		// Logger
		OutputDebugString(ss.str().c_str());

		// Dialog
		MessageBox(windowHandle, ss.str().c_str(), L"FATAL ERROR", MB_OK | MB_ICONERROR);
		exit(1);
	}
}
