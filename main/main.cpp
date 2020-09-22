#ifndef UNICODE
#define UNICODE
#endif

#pragma warning(push)
#pragma warning(disable: 4244)
#include <windows.h>
#include <MMREG.H>
#include <wingdi.h>

#include <cassert>

#include <sstream>
#include <string>
#include <set>
#include <algorithm>
#include <iostream>

#include "glad.h"
#pragma warning(pop)

#define global static
#define internal static

global struct MIDI_PIANO {
	HINSTANCE appInstance;
	HGLRC openGlRenderingContext;
	HWND windowHandle;
	HMIDIOUT midiOutHandle;
} MIDI_PIANO;

global int globalWindowWidth = 400;
global int globalWindowHeight = 300;

internal bool loadWithLoadGLLoader()
{
	if (!gladLoadGLLoader((GLADloadproc)wglGetProcAddress))
	{
		OutputDebugString(L"Failed to load OpenGL library using gladLoadGLLoader\n");
		return false;
	}

	OutputDebugString(L"Loaded OpenGL library using gladLoadGLLoader\n");
	return true;
}

internal bool loadWithGladLoadGL()
{
	if (!gladLoadGL())
	{
		OutputDebugString(L"Failed to load OpenGL library using gladLoadGL\n");
		return false;
	}

	OutputDebugString(L"Loaded OpenGL library using gladLoadGL\n");
	return true;
}

internal int loadGL()
{
	if (loadWithGladLoadGL())
	{
		return true;
	}

	if (loadWithLoadGLLoader())
	{
		return true;
	}

	return false;
}

internal bool initOpenGl(HWND windowHandle)
{
	OutputDebugString(L"Initializing OpenGL context\n");
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24, // Depth Buffer
		8,  // Stencil Buffer
		0,  // Aux Buffer
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	HDC deviceContext = GetDC(windowHandle);

	int pixelFormat = ChoosePixelFormat(deviceContext, &pixelFormatDescriptor);
	SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDescriptor);

	HGLRC openGlRenderingContext = wglCreateContext(deviceContext);
	if (openGlRenderingContext == NULL)
	{
		OutputDebugString(L"Failed to create OpenGL context\n");
		return false;
	}
	OutputDebugString(L"OpenGL context created\n");

	if (wglMakeCurrent(deviceContext, openGlRenderingContext) == FALSE)
	{
		OutputDebugString(L"Failed to set OpenGL context\n");
		return false;
	}
	OutputDebugString(L"OpenGL context set\n");

	if (!loadGL())
	{
		return false;
	}

	int versionMajor;
	int versionMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
	glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

	std::wstringstream wss;
	wss << L"Successfully initialized OpenGL v" << versionMajor << L"." << versionMinor << L"" << std::endl;
	OutputDebugStringW(wss.str().c_str());

	MIDI_PIANO.openGlRenderingContext = openGlRenderingContext;

	glViewport(0, 0, globalWindowHeight, globalWindowWidth);

	ReleaseDC(windowHandle, deviceContext);

	return true;
}

internal void centerWindow(HWND windowHandle)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = {};
	GetWindowRect(windowHandle, &windowRect);

	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;

	SetWindowPos(windowHandle, NULL, screenWidth / 2 - windowWidth, screenHeight / 2 - windowHeight, 0, 0, SWP_SHOWWINDOW);
}

internal HWND createWindow(WNDCLASS windowClass)
{
	if (!RegisterClass(&windowClass))
	{
		return NULL;
	}

	HWND windowHandle = CreateWindowW(
		windowClass.lpszClassName,
		L"MIDI Piano",
		WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		NULL,
		NULL,
		globalWindowWidth,
		globalWindowHeight,
		NULL,
		NULL,
		windowClass.hInstance,
		NULL
	);

	centerWindow(windowHandle);

	if (!windowHandle) {
		return NULL;
	}

	return windowHandle;
}

internal void runLoop()
{
	OutputDebugString(L"Running Message Loop\n");

	MSG message = {};
	while (GetMessage(&message, 0, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

global HDC wtDeviceContext = 0;
global RECT* wtRectangle = 0;
global int wtLeftPadding = 0;
global int wtTopPadding = 0;
global int wtTopStep = 20;

void initWriteText(HDC deviceContext, RECT* rectangle, int leftPadding)
{
	wtDeviceContext = deviceContext;
	wtRectangle = rectangle;
	wtLeftPadding = leftPadding;
	wtTopPadding = 0;
}

void writeText(std::wstring string)
{
	ExtTextOut(wtDeviceContext, wtLeftPadding, wtTopPadding + wtTopStep, ETO_OPAQUE, wtRectangle, string.c_str(), (UINT)string.size(), NULL);
	wtTopPadding += wtTopStep;
}

void writeBlank()
{
	writeText(L"");
}

struct ManufacturerIdentity
{
	WORD id;
	const char name[64];
};

struct ManufacturerIdentity MANUFACTURER_IDENTITIES[] =
{
	MM_GRAVIS, "Advanced Gravis Computer Technology, Ltd.",
	MM_ANTEX, "Antex Electronics Corporation",
	MM_APPS, "APPS Software",
	MM_ARTISOFT, "Artisoft, Inc.",
	MM_AST, "AST Research, Inc.",
	MM_ATI, "ATI Technologies, Inc.",
	MM_AUDIOFILE, "Audio, Inc.",
	MM_APT, "Audio Processing Technology",
	MM_AUDIOPT, "Audio Processing Technology",
	MM_AURAVISION, "Auravision Corporation",
	MM_AZTECH, "Aztech Labs, Inc.",
	MM_CANOPUS, "Canopus, Co., Ltd.",
	MM_COMPUSIC, "Compusic",
	MM_CAT, "Computer Aided Technology, Inc.",
	MM_COMPUTER_FRIENDS, "Computer Friends, Inc.",
	MM_CONTROLRES, "Control Resources Corporation",
	MM_CREATIVE, "Creative Labs, Inc.",
	MM_DIALOGIC, "Dialogic Corporation",
	MM_DOLBY, "Dolby Laboratories, Inc.",
	MM_DSP_GROUP, "DSP Group, Inc.",
	MM_DSP_SOLUTIONS, "DSP Solutions, Inc.",
	MM_ECHO, "Echo Speech Corporation",
	MM_ESS, "ESS Technology, Inc.",
	MM_EVEREX, "Everex Systems, Inc.",
	MM_EXAN, "EXAN, Ltd.",
	MM_FUJITSU, "Fujitsu, Ltd.",
	MM_IOMAGIC, "I/O Magic Corporation",
	MM_ICL_PS, "ICL Personal Systems",
	MM_OLIVETTI, "Ing. C. Olivetti & C., S.p.A.",
	MM_ICS, "Integrated Circuit Systems, Inc.",
	MM_INTEL, "Intel Corporation",
	MM_INTERACTIVE, "InterActive, Inc.",
	MM_IBM, "International Business Machines",
	MM_ITERATEDSYS, "Iterated Systems, Inc.",
	MM_LOGITECH, "Logitech, Inc.",
	MM_LYRRUS, "Lyrrus, Inc.",
	MM_MATSUSHITA, "Matsushita Electric Corporation of America",
	MM_MEDIAVISION, "Media Vision, Inc.",
	MM_METHEUS, "Metheus Corporation",
	MM_MELABS, "microEngineering Labs",
	MM_MICROSOFT, "Microsoft Corporation",
	MM_MOSCOM, "MOSCOM Corporation",
	MM_MOTOROLA, "Motorola, Inc.",
	MM_NMS, "Natural MicroSystems Corporation",
	MM_NCR, "NCR Corporation",
	MM_NEC, "NEC Corporation",
	MM_NEWMEDIA, "New Media Corporation",
	MM_OKI, "OKI",
	MM_OPTI, "OPTi, Inc.",
	MM_ROLAND, "Roland Corporation",
	MM_SCALACS, "SCALACS",
	MM_EPSON, "Seiko Epson Corporation, Inc.",
	MM_SIERRA, "Sierra Semiconductor Corporation",
	MM_SILICONSOFT, "Silicon Software, Inc.",
	MM_SONICFOUNDRY, "Sonic Foundry",
	MM_SPEECHCOMP, "Speech Compression",
	MM_SUPERMAC, "Supermac Technology, Inc.",
	MM_TANDY, "Tandy Corporation",
	MM_KORG, "Toshihiko Okuhura, Korg, Inc.",
	MM_TRUEVISION, "Truevision, Inc.",
	MM_TURTLE_BEACH, "Turtle Beach Systems",
	MM_VAL, "Video Associates Labs, Inc.",
	MM_VIDEOLOGIC, "VideoLogic, Inc.",
	MM_VITEC, "Visual Information Technologies, Inc.",
	MM_VOCALTEC, "VocalTec, Inc.",
	MM_VOYETRA, "Voyetra Technologies",
	MM_WANGLABS, "Wang Laboratories",
	MM_WILLOWPOND, "Willow Pond Corporation",
	MM_WINNOV, "Winnov, LP",
	MM_XEBEC, "Xebec Multimedia Solutions Limited",
	MM_YAMAHA, "Yamaha Corporation of America",
};

const char UNKNOWN_MANUFACTURER[] = "Unknown Manufacturer";

std::string manufacturerName(WORD id)
{
	size_t arr_size = sizeof(MANUFACTURER_IDENTITIES) / sizeof(MANUFACTURER_IDENTITIES[0]);
	for (int i = 0; i < arr_size; i++)
	{
		ManufacturerIdentity identity = MANUFACTURER_IDENTITIES[i];

		if (identity.id != id) continue;

		return identity.name;
	}

	return UNKNOWN_MANUFACTURER;
}

struct ProductIdentity
{
	WORD id;
	const char name[64];
};

struct ProductIdentity PRODUCT_IDENTITIES[] = {
	MM_ADLIB, "dlib-compatible synthesizer",
	MM_MSFT_ACM_G711, "G.711 codec",
	MM_MSFT_ACM_GSM610, "GSM 610 codec",
	MM_MSFT_ACM_IMAADPCM, "IMA ADPCM codec",
	MM_PC_JOYSTICK, "Joystick adapter",
	MM_MIDI_MAPPER, "MIDI mapper",
	MM_MPU401_MIDIIN, "MPU 401-compatible MIDI input port",
	MM_MPU401_MIDIOUT, "MPU 401-compatible MIDI output port",
	MM_MSFT_ACM_MSADPCM, "MS ADPCM codec",
	MM_MSFT_WSS_FMSYNTH_STEREO, "MS audio board stereo FM synthesizer",
	MM_MSFT_WSS_AUX, "MS audio board aux port",
	MM_MSFT_WSS_MIXER, "MS audio board mixer driver",
	MM_MSFT_WSS_WAVEIN, "MS audio board waveform input",
	MM_MSFT_WSS_WAVEOUT, "MS audio board waveform output",
	MM_MSFT_MSACM, "MS audio compression manager",
	MM_MSFT_ACM_MSFILTER, "MS filter",
	MM_MSFT_WSS_OEM_AUX, "MS OEM audio aux port",
	MM_MSFT_WSS_OEM_MIXER, "MS OEM audio board mixer driver",
	MM_MSFT_WSS_OEM_FMSYNTH_STEREO, "MS OEM audio board stereo FM synthesizer",
	MM_MSFT_WSS_OEM_WAVEIN, "MS OEM audio board waveform input",
	MM_MSFT_WSS_OEM_WAVEOUT, "MS OEM audio board waveform output",
	MM_MSFT_GENERIC_AUX_CD, "MS vanilla driver aux (CD)",
	MM_MSFT_GENERIC_AUX_LINE, "MS vanilla driver aux (line in)",
	MM_MSFT_GENERIC_AUX_MIC, "MS vanilla driver aux (mic)",
	MM_MSFT_GENERIC_MIDIOUT, "MS vanilla driver MIDI external out",
	MM_MSFT_GENERIC_MIDIIN, "MS vanilla driver MIDI in",
	MM_MSFT_GENERIC_MIDISYNTH, "MS vanilla driver MIDI synthesizer",
	MM_MSFT_GENERIC_WAVEIN, "MS vanilla driver waveform input",
	MM_MSFT_GENERIC_WAVEOUT, "MS vanilla driver wavefrom output",
	MM_PCSPEAKER_WAVEOUT, "PC speaker waveform output",
	MM_MSFT_ACM_PCM, "PCM converter",
	MM_SNDBLST_SYNTH, "Sound Blaster internal synthesizer",
	MM_SNDBLST_MIDIIN, "Sound Blaster MIDI input port",
	MM_SNDBLST_MIDIOUT, "Sound Blaster MIDI output port",
	MM_SNDBLST_WAVEIN, "Sound Blaster waveform input",
	MM_SNDBLST_WAVEOUT, "Sound Blaster waveform output",
	MM_WAVE_MAPPER, "Wave mapper"
};

const char UNKNOWN_PRODUCT[] = "Unknown Product";

std::string productName(WORD id)
{
	size_t arr_size = sizeof(PRODUCT_IDENTITIES) / sizeof(PRODUCT_IDENTITIES[0]);
	for (int i = 0; i < arr_size; i++)
	{
		ProductIdentity identity = PRODUCT_IDENTITIES[i];

		if (identity.id != id) continue;

		return identity.name;
	}

	return UNKNOWN_PRODUCT;
}

std::string getDriverVersion(MMVERSION version)
{
	int minor = version & 0xFF;
	int major = version >> 8;
	
	std::stringstream ss;
	ss << "v" <<  major << "." << minor;

	return ss.str();
}

struct Technology
{
	WORD id;
	const char name[32];
};

struct Technology TECHNOLOGIES[] = {
	MOD_MIDIPORT, "MIDI hardware port",
	MOD_SYNTH, "Synthesizer",
	MOD_SQSYNTH, "Square wave synthesizer",
	MOD_FMSYNTH, "FM synthesizer",
	MOD_MAPPER, "Microsoft MIDI mapper",
	MOD_WAVETABLE, "Hardware wavetable synthesizer",
	MOD_SWSYNTH, "Software synthesizer"
};

const char UNKNOWN_TECHNOLOGY[] = "Unknown Technology";

std::string getTechnology(WORD technology)
{
	size_t arr_size = sizeof(TECHNOLOGIES) / sizeof(TECHNOLOGIES[0]);
	for (int i = 0; i < arr_size; i++)
	{
		Technology tech = TECHNOLOGIES[i];

		if (tech.id != technology) continue;

		return tech.name;
	}

	return UNKNOWN_TECHNOLOGY;
}

struct Note
{
	byte id;
	const char name[8];
};

Note C0{ 0, "C0" };
Note CSharp0{ 1, "C#0" };
Note D0{ 2, "D0" };
Note DSharp0{ 3, "D#0" };
Note E0{ 4, "E0" };
Note F0{ 5, "F0" };
Note FSharp0{ 6, "F#0" };
Note G0{ 7, "G0" };
Note GSharp0{ 8, "G#0" };
Note A1{ 9, "A1" };
Note ASharp1{ 10, "A#1" };
Note B1{ 11, "B1" };
Note C1{ 12, "C1" };
Note CSharp1{ 13, "C#1" };
Note D1{ 14, "D1" };
Note DSharp1{ 15, "D#1" };
Note E1{ 16, "E1" };
Note F1{ 17, "F1" };
Note FSharp1{ 18, "F#1" };
Note G1{ 19, "G1" };
Note GSharp1{ 20, "G#1" };
Note A2{ 21, "A2" };
Note ASharp2{ 22, "A#2" };
Note B2{ 23, "B2" };
Note C2{ 24, "C2" };
Note CSharp2{ 25, "C#2" };
Note D2{ 26, "D2" };
Note DSharp2{ 27, "D#2" };
Note E2{ 28, "E2" };
Note F2{ 29, "F2" };
Note FSharp2{ 30, "F#2" };
Note G2{ 31, "G2" };
Note GSharp2{ 32, "G#2" };
Note A3{ 33, "A3" };
Note ASharp3{ 34, "A#3" };
Note B3{ 35, "B3" };
Note C3{ 36, "C3" };
Note CSharp3{ 37, "C#3" };
Note D3{ 38, "D3" };
Note DSharp3{ 39, "D#3" };
Note E3{ 40, "E3" };
Note F3{ 41, "F3" };
Note FSharp3{ 42, "F#3" };
Note G3{ 43, "G3" };
Note GSharp3{ 44, "G#3" };
Note A4{ 45, "A4" };
Note ASharp4{ 46, "A#4" };
Note B4{ 47, "B4" };
Note C4{ 48, "C4" };
Note CSharp4{ 49, "C#4" };
Note D4{ 50, "D4" };
Note DSharp4{ 51, "D#4" };
Note E4{ 52, "E4" };
Note F4{ 53, "F4" };
Note FSharp4{ 54, "F#4" };
Note G4{ 55, "G4" };
Note GSharp4{ 56, "G#4" };
Note A5{ 57, "A5" };
Note ASharp5{ 58, "A#5" };
Note B5{ 59, "B5" };
Note C5{ 60, "C5" };
Note CSharp5{ 61, "C#5" };
Note D5{ 62, "D5" };
Note DSharp5{ 63, "D#5" };
Note E5{ 64, "E5" };
Note F5{ 65, "F5" };
Note FSharp5{ 66, "F#5" };
Note G5{ 67, "G5" };
Note GSharp5{ 68, "G#5" };
Note A6{ 69, "A6" };
Note ASharp6{ 70, "A#6" };
Note B6{ 71, "B6" };
Note C6{ 72, "C6" };
Note CSharp6{ 73, "C#6" };
Note D6{ 74, "D6" };
Note DSharp6{ 75, "D#6" };
Note E6{ 76, "E6" };
Note F6{ 77, "F6" };
Note FSharp6{ 78, "F#6" };
Note G6{ 79, "G6" };
Note GSharp6{ 80, "G#6" };
Note A7{ 81, "A7" };
Note ASharp7{ 82, "A#7" };
Note B7{ 83, "B7" };
Note C7{ 84, "C7" };
Note CSharp7{ 85, "C#7" };
Note D7{ 86, "D7" };
Note DSharp7{ 87, "D#7" };
Note E7{ 88, "E7" };
Note F7{ 89, "F7" };
Note FSharp7{ 90, "F#7" };
Note G7{ 91, "G7" };
Note GSharp7{ 92, "G#7" };
Note A8{ 93, "A8" };
Note ASharp8{ 94, "A#8" };
Note B8{ 95, "B8" };
Note C8{ 96, "C8" };
Note CSharp8{ 97, "C#8" };
Note D8{ 98, "D8" };
Note DSharp8{ 99, "D#8" };
Note E8{ 100, "E8" };
Note F8{ 101, "F8" };
Note FSharp8{ 102, "F#8" };
Note G8{ 103, "G8" };
Note GSharp8{ 104, "G#8" };
Note A9{ 105, "A9" };
Note ASharp9{ 106, "A#9" };
Note B9{ 107, "B9" };
Note C9{ 108, "C9" };
Note CSharp9{ 109, "C#9" };
Note D9{ 110, "D9" };
Note DSharp9{ 111, "D#9" };
Note E9{ 112, "E9" };
Note F9{ 113, "F9" };
Note FSharp9{ 114, "F#9" };
Note G9{ 115, "G9" };
Note GSharp9{ 116, "G#9" };
Note A10{ 117, "A10" };
Note ASharp10{ 118, "A#10" };
Note B10{ 119, "B10" };
Note C10{ 120, "C10" };
Note CSharp10{ 121, "C#10" };
Note D10{ 122, "D10" };
Note DSharp10{ 123, "D#10" };
Note E10{ 124, "E10" };
Note F10{ 125, "F10" };
Note FSharp10{ 126, "F#10" };
Note G10{ 127, "G10" };

std::set<Note*> sustained_notes {};

// NOTE(aaron.meaney): This will essentially be the keymapper, it is hard coded at the moment
Note* stringToNote(std::string character)
{
	std::transform(character.begin(), character.end(), character.begin(), std::toupper);

	Note* note{};

	// NOTE(aaron.meaney): This is terrible, terrible code - replace this once I get to the proper keymapper feature
	if (character == "Z") { note = &C3; }
	if (character == "S") { note = &CSharp3; }
	if (character == "X") { note = &D3; }
	if (character == "D") { note = &DSharp3; }
	if (character == "C") { note = &E3; }
	if (character == "V") { note = &F3; }
	if (character == "G") { note = &FSharp3; }
	if (character == "B") { note = &G3; }
	if (character == "H") { note = &GSharp3; }
	if (character == "N") { note = &A4; }
	if (character == "J") { note = &ASharp4; }
	if (character == "M") { note = &B4; }

	if (note == nullptr) {
		return nullptr;
	}

	return note;
}

#define MIDI_STATUS_NOTE_ON 0x90
#define MIDI_STATUS_NOTE_OFF 0x80

void sendMidiMessage(DWORD message)
{
	MMRESULT response = midiOutShortMsg(MIDI_PIANO.midiOutHandle, message);

	if (response != MMSYSERR_NOERROR)
	{
		std::wstringstream ss;
		ss << L"Could not send message to MIDI device\n";
		ss << L"Error code: " << std::to_wstring(response) << std::endl;

		OutputDebugString(ss.str().c_str());
		MessageBox(MIDI_PIANO.windowHandle, ss.str().c_str(), L"FATAL ERROR", MB_OK | MB_ICONERROR);
		exit(1);
	}
}

void playNote(Note* note)
{
	union {
		DWORD word;
		BYTE bytes[4];
	} midiMessage;

	midiMessage.bytes[0] = MIDI_STATUS_NOTE_ON;
	midiMessage.bytes[1] = note->id;
	midiMessage.bytes[2] = 127;
	midiMessage.bytes[3] = 0;

	sendMidiMessage(midiMessage.word);
}

void stopNote(Note* note)
{
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

void onKeyDown(LPCSTR character)
{
	Note* note = stringToNote(character);

	if (note != nullptr)
	{
		if (sustained_notes.find(note) != sustained_notes.end())
		{
			return;
		}

		sustained_notes.insert(note);

		if (MIDI_PIANO.midiOutHandle != NULL)
		{
			playNote(note);
		}
	}
}

void onKeyUp(LPCSTR character)
{
	Note* note = stringToNote(character);

	if (note != nullptr)
	{
		if (sustained_notes.find(note) == sustained_notes.end())
		{
			return;
		}

		sustained_notes.erase(note);

		if (MIDI_PIANO.midiOutHandle != NULL)
		{
			stopNote(note);
		}
	}
}

LRESULT CALLBACK MidiPianoMainWindowCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (message) {
		case WM_CREATE:
		{
			OutputDebugString(L"WM_CREATE\n");

			if (!initOpenGl(windowHandle))
			{
				return -1;
			}

			break;
		}
		case WM_PAINT:
		{
			OutputDebugString(L"WM_PAINT\n");

			// This is all temporary drawing code until I implement a proper GUI system

			RECT rectangle = {};
			PAINTSTRUCT paint = {};
			HDC deviceContext = BeginPaint(windowHandle, &paint);

			SetBkMode(deviceContext, TRANSPARENT);

			initWriteText(deviceContext, &rectangle, 10);
			writeText(L"MIDI Piano");
			writeText(L"Version 0.0.1 ALPHA");
			writeBlank();
			writeText(L"By Aaron Meaney");
			writeText(L"13/09/2020");

			std::wstringstream ss;

			for (auto it = sustained_notes.begin(); it != sustained_notes.end(); ++it)
			{
				Note* note = *it;
				ss << note->name;
				ss << " ";
			}

			writeText(ss.str().c_str());

			EndPaint(windowHandle, &paint);
			break;
		}
		case WM_KEYDOWN:
		{
			LPCSTR character = (LPCSTR)&wParam;

			onKeyDown(character);
			
			RedrawWindow(windowHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);

			break;
		}
		case WM_KEYUP:
		{
			LPCSTR character = (LPCSTR)&wParam;

			onKeyUp(character);

			RedrawWindow(windowHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);

			break;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}
		case WM_SIZE:
		{
			OutputDebugString(L"WM_SIZE\n");

			if (MIDI_PIANO.openGlRenderingContext == NULL) {
				break;
			}

			INT width = LOWORD(lParam);
			INT height = HIWORD(lParam);

			glViewport(0, 0, width, height);

			break;
		}
		case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO minMaxInfo = (LPMINMAXINFO)lParam;
			minMaxInfo->ptMinTrackSize.x = globalWindowWidth;
			minMaxInfo->ptMinTrackSize.y = globalWindowHeight;

			minMaxInfo->ptMaxTrackSize.x = globalWindowWidth;
			minMaxInfo->ptMaxTrackSize.y = globalWindowHeight;

			break;
		}
		default:
		{
			result = DefWindowProc(windowHandle, message, wParam, lParam);
		}
	}

	return result;
}

void loadMidi(HWND windowHandle)
{
	unsigned int midiDeviceCount = midiOutGetNumDevs();

	std::wstringstream ss;
	ss << L"Number of MIDI output devices: " << midiDeviceCount << "\n\n";

	OutputDebugString(ss.str().c_str());

	if (midiDeviceCount == 0) {
		return;
	}

	ss.str(L"");

	MIDIOUTCAPS midiOutCaps = {};
	bool found = false;
	int midiDeviceId = 0;

	// This should be made into its own function like findWindowsMidiDevice
	for (unsigned int midiDeviceIndex = 0; midiDeviceIndex < midiDeviceCount; midiDeviceIndex++)
	{
		midiOutGetDevCaps(midiDeviceIndex, &midiOutCaps, sizeof(midiOutCaps));

		std::string manufacturer = manufacturerName(midiOutCaps.wMid);
		std::string product = productName(midiOutCaps.wPid);
		std::string driverVersion = getDriverVersion(midiOutCaps.vDriverVersion);
		std::wstring productName = midiOutCaps.szPname;
		std::string technology = getTechnology(midiOutCaps.wTechnology);

		ss << L"==========\nFound device " << std::to_wstring(midiDeviceIndex + 1) << L" of " << std::to_wstring(midiDeviceCount) << "\n";
		ss << "Manufacturer: " << manufacturer.c_str() << "\n";
		ss << "Product: " << product.c_str() << "\n";
		ss << "Driver Version: " << driverVersion.c_str() << "\n";
		ss << "Product Name: " << productName << "\n";
		ss << "Technology: " << technology.c_str() << "\n";
		ss << "Voices: " << midiOutCaps.wVoices << "\n";
		ss << "Channel Mask: " << midiOutCaps.wChannelMask << "\n";
		ss << "Support: " << midiOutCaps.dwSupport << "\n";
		ss << std::endl;

		OutputDebugString(ss.str().c_str());

		ss.clear();

		if (midiOutCaps.wPid == MM_MSFT_GENERIC_MIDISYNTH && midiOutCaps.wTechnology == MOD_SWSYNTH)
		{
			found = true;
			midiDeviceId = midiDeviceIndex;
			break;
		}
	}

	if (!found)
	{
		OutputDebugString(L"Could not find valid MIDI Output Device");
		MessageBox(windowHandle, L"Could not find valid MIDI Output Device.\nThis program will now exit.", L"FATAL ERROR", MB_OK | MB_ICONERROR);
		exit(1);
	}

	HMIDIOUT midiOutHandle = {};
	MMRESULT midiOpenResult = midiOutOpen(&midiOutHandle, midiDeviceId, NULL, NULL, CALLBACK_NULL); // Change NULL to MidiOutProc later

	if (midiOpenResult != MMSYSERR_NOERROR)
	{
		ss.str(L"");
		ss << L"Could not open MIDI device \n";
		ss << L"Error code: " << std::to_wstring(midiOpenResult) << std::endl;

		OutputDebugString(ss.str().c_str());
		MessageBox(windowHandle, ss.str().c_str(), L"FATAL ERROR", MB_OK | MB_ICONERROR);
		exit(1);
	}

	MIDI_PIANO.midiOutHandle = midiOutHandle;
}

int WINAPI wWinMain(_In_ HINSTANCE appInstance, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int)
{
	MIDI_PIANO.appInstance = appInstance;

	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = MidiPianoMainWindowCallback;
	windowClass.hInstance = appInstance;
	windowClass.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	windowClass.lpszClassName = L"MidiPianoWindowClass";
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	OutputDebugString(L"Acquiring window handle...\n");
	HWND windowHandle = createWindow(windowClass);
	if (windowHandle == NULL) {
		OutputDebugString(L"Failed to acquire a window handle\n");
		return -1;
	}
	MIDI_PIANO.windowHandle = windowHandle;
	OutputDebugString(L"Window handle acquired\n");

	loadMidi(windowHandle);

	OutputDebugString(L"Midi handle acquired\n");

	ShowWindow(windowHandle, SW_SHOW);

	OutputDebugString(L"Window displayed\n");
	
	runLoop();

	PostQuitMessage(0);
	return 0;
}
