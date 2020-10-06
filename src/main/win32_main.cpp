#ifndef UNICODE
#define UNICODE
#endif

// Platform Headers
#include <windows.h>
#include <wingdi.h>

// Third Party Headers
#include <win32_window.h>
#include <win32_midi.h>
#include <win32_opengl.h>
#include <midi_piano.h>

// STL
#include <string>
#include <sstream>

// Move this to its own class  :/
class Win32UserIO : public MidiPiano::Core::IUserIO
{
public:
	void emitKeydown(LPCSTR character)
	{
		logChar(*character);
		onKeydown(*character);
	}

	void emitKeyup(LPCSTR character)
	{
		logChar(*character);
		onKeyup(*character);
	}
private:
	void logChar(char character)
	{
		char str[3];
		str[0] = character;
		str[1] = '\n';
		str[2] = '\0';
		OutputDebugStringA(str);
	}
};

// PSC - Refactor this to try and remove any global references
// Change it to Win32MidiPiano class or IMidiPianoUI?
static struct MIDI_PIANO {
	Win32Midi* midi;
	Win32OpenGl* openGl;
	Win32UserIO* io;
} MIDI_PIANO;

// ASC - Add a way to create a window generically
// Also move these down to where they're used, should not be global
static int GLOBAL_WINDOW_WIDTH = 300;
static int GLOBAL_WINDOW_HEIGHT = 100;

char mapVkToChar(WPARAM wParam)
{
	UINT mappedChar = MapVirtualKeyW(static_cast<UINT>(wParam), MAPVK_VK_TO_CHAR);
	return static_cast<char>(mappedChar);
}

static void win32PollMessages()
{
	// Logger
	OutputDebugString(L"Running Message Loop\n");

	MSG message = {};
	while (GetMessage(&message, 0, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

void win32CallbackCreate(HWND windowHandle)
{
	Win32Midi* midi = new Win32Midi(windowHandle);
	MIDI_PIANO.midi = midi;

	Win32OpenGl* openGl = new Win32OpenGl(windowHandle, GLOBAL_WINDOW_WIDTH, GLOBAL_WINDOW_HEIGHT);
	MIDI_PIANO.openGl = openGl;
}

void win32CallbackPaint(HWND windowHandle)
{
	// This is all temporary drawing code until I implement a proper GUI system
	RECT rectangle = {};
	PAINTSTRUCT paint = {};
	HDC deviceContext = BeginPaint(windowHandle, &paint);

	SetBkMode(deviceContext, TRANSPARENT);

	EndPaint(windowHandle, &paint);
}

void win32CallbackKeydown(HWND windowHandle, WPARAM wParam)
{
	char character = mapVkToChar(wParam);

	MIDI_PIANO.io->emitKeydown(&character);

	// Win32Window.redraw - private method
	RedrawWindow(windowHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
}

void win32CallbackKeyup(HWND windowHandle, WPARAM wParam)
{
	char character = mapVkToChar(wParam);

	MIDI_PIANO.io->emitKeyup(&character);

	// Win32Window.redraw - private method
	RedrawWindow(windowHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
}

void win32CallbackSize(LPARAM lParam)
{
	INT width = LOWORD(lParam);
	INT height = HIWORD(lParam);

	MIDI_PIANO.openGl->updateViewport(0, 0, width, height);
}

class Win32MidiOutAdapter : virtual public MidiPiano::Core::IMidiOut
{
public:
	Win32MidiOutAdapter(Win32Midi* win32Midi) :
		win32Midi(win32Midi)
	{}

	void playNote(int midiNote)
	{
		if (midiNote < 0) return;

		win32Midi->playNote(midiNote);
	}

	void stopNote(int midiNote)
	{
		if (midiNote < 0) return;

		win32Midi->stopNote(midiNote);
	}
private:
	Win32Midi* win32Midi;
};

class Win32Logger : virtual public MidiPiano::Core::ILogger
{
public:
	Win32Logger() {}

	void log(std::wstring message)
	{
		OutputDebugStringW(message.c_str());
	}

	void log(wchar_t message)
	{
		std::wstringstream ss;
		ss << message;
		log(ss.str());
	}

	void log(std::string message)
	{
		OutputDebugStringA(message.c_str());
	}

	void log(char message)
	{
		std::stringstream ss;
		ss << message;
		log(ss.str());
	}
};

int WINAPI wWinMain(_In_ HINSTANCE appInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	Win32Window window(appInstance, L"MIDI Piano", GLOBAL_WINDOW_WIDTH, GLOBAL_WINDOW_HEIGHT, win32CallbackCreate);
	window.callbackPaint = win32CallbackPaint;
	window.callbackSize = win32CallbackSize;

	MidiPiano::Core::IMidiOut* midiOutAdapter = new Win32MidiOutAdapter(MIDI_PIANO.midi);
	MidiPiano::Core::IUserIO* userIO = new Win32UserIO();
	MidiPiano::Core::ILogger* logger = new Win32Logger();

	// Find a better way to do this, don't just set a global :(
	MIDI_PIANO.io = (Win32UserIO*)userIO;

	window.callbackKeydown = win32CallbackKeydown;
	window.callbackKeyup = win32CallbackKeyup;

	MidiPiano::Core::MidiPiano piano(midiOutAdapter, userIO, logger);

	window.show();

	win32PollMessages();

	PostQuitMessage(0);
	return 0;
}
