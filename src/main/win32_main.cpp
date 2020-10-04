#ifndef UNICODE
#define UNICODE
#endif

// Platform Headers
#include <windows.h>
#include <wingdi.h>

// Third Party Headers
#include <win32_window.h>
#include <win32_midi.h>
#include <win32_note.cpp>
#include <win32_opengl.h>

// PSC - Refactor this to try and remove any global references
// Change it to Win32MidiPiano class or IMidiPianoUI?
static struct MIDI_PIANO {
	Win32Midi* midi;
	Win32OpenGl* openGl;
} MIDI_PIANO;

// ASC - Add a way to create a window generically
// Also move these down to where they're used, should not be global
static int GLOBAL_WINDOW_WIDTH = 300;
static int GLOBAL_WINDOW_HEIGHT = 100;

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

// PSC
void onKeyDown(LPCSTR character)
{
	Note* note = charToNote(character);

	if (note == nullptr) return;
	
	MIDI_PIANO.midi->playNote(note);
}

// PSC
void onKeyUp(LPCSTR character)
{
	Note* note = charToNote(character);

	if (note == nullptr) return;

	MIDI_PIANO.midi->stopNote(note);
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
	LPCSTR character = (LPCSTR)&wParam;

	// Win32Window.emitEvent(KEY_DOWN, character)
	// Might need to create an event system?
	// The emit methods should be private, so no outside code can emit events
	onKeyDown(character);

	// Win32Window.redraw - private method
	RedrawWindow(windowHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
}

void win32CallbackKeyup(HWND windowHandle, WPARAM wParam)
{
	LPCSTR character = (LPCSTR)&wParam;

	// Win32Window.emitEvent(KEY_UP, character)
	onKeyUp(character);

	// Win32Window.redraw - private method
	RedrawWindow(windowHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);
}

void win32CallbackSize(LPARAM lParam)
{
	INT width = LOWORD(lParam);
	INT height = HIWORD(lParam);

	MIDI_PIANO.openGl->updateViewport(0, 0, width, height);
}

int WINAPI wWinMain(_In_ HINSTANCE appInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	Win32Window window(appInstance, L"MIDI Piano", GLOBAL_WINDOW_WIDTH, GLOBAL_WINDOW_HEIGHT, win32CallbackCreate);
	window.callbackPaint = win32CallbackPaint;
	window.callbackSize = win32CallbackSize;

	// Instead make this emit events on the window event systems
	// E.g.
	// appWindow.emitEvent(KEYDOWN, char)
	// appWinodw.emitEvent(KEYUP, char)
	// 
	// The MidiPiano class would then listen to this
	// appWindow.listenEvent(KEYDOWN, onKeydown)
	// appWindow.listenEvent(KEYUP, onKeyUp)
	//
	// Inside those events we can then handle the generic midi playback
	// without any platform independence
	//
	// void onKeydown(const char* character)
	// {
	//    midi->playNote(character); <-- Application layer / adapter pattern, doesn't care about platform
	// }
	window.callbackKeydown = win32CallbackKeydown;
	window.callbackKeyup = win32CallbackKeyup;

	window.show();

	win32PollMessages();

	PostQuitMessage(0);
	return 0;
}
