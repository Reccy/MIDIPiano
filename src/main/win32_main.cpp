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

// PSC - Refactor this to try and remove any global references
// Change it to Win32MidiPiano class or IMidiPianoUI?
static struct MIDI_PIANO {
	Win32Midi* midi;
	Win32OpenGl* openGl;
	MidiPiano::Core::MidiPiano* piano;
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
	PAINTSTRUCT paint = {};
	HDC deviceContext = BeginPaint(windowHandle, &paint);

	SetBkMode(deviceContext, TRANSPARENT);

	EndPaint(windowHandle, &paint);
}

void win32CallbackKillFocus(WPARAM)
{
	if (!MIDI_PIANO.piano) return;

	MIDI_PIANO.piano->silenceAllNotes();
}

void win32CallbackKeydown(HWND windowHandle, WPARAM wParam)
{
	char character = mapVkToChar(wParam);

	MIDI_PIANO.piano->onKeydown(character);

	// Win32Window.redraw - private method
	RedrawWindow(windowHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
}

void win32CallbackKeyup(HWND windowHandle, WPARAM wParam)
{
	char character = mapVkToChar(wParam);

	MIDI_PIANO.piano->onKeyup(character);

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
	explicit Win32MidiOutAdapter(Win32Midi* win32Midi) :
		win32Midi(win32Midi)
	{}

	void playNote(int midiNote)
	{
		win32Midi->playNote(midiNote);
	}

	void stopNote(int midiNote)
	{
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
		
		std::wstring converted_message = ss.str();
		log(converted_message);
	}

	void log(std::string message)
	{
		OutputDebugStringA(message.c_str());
	}

	void log(char message)
	{
		std::stringstream ss;
		ss << message;

		std::string converted_message = ss.str();
		log(converted_message);
	}
};

int WINAPI wWinMain(_In_ HINSTANCE appInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	Win32Window window(appInstance, L"MIDI Piano", GLOBAL_WINDOW_WIDTH, GLOBAL_WINDOW_HEIGHT, win32CallbackCreate);
	window.callbackPaint = win32CallbackPaint;
	window.callbackKillFocus = win32CallbackKillFocus;
	window.callbackSize = win32CallbackSize;

	MidiPiano::Core::IMidiOut* midiOutAdapter = new Win32MidiOutAdapter(MIDI_PIANO.midi);
	MidiPiano::Core::ILogger* logger = new Win32Logger();

	window.callbackKeydown = win32CallbackKeydown;
	window.callbackKeyup = win32CallbackKeyup;

	MidiPiano::Core::MidiPiano piano(midiOutAdapter, logger);

	MIDI_PIANO.piano = &piano;

	window.show();

	win32PollMessages();

	PostQuitMessage(0);
	return 0;
}
