#include "win32_driver_info.h"

#include <sstream>

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

const char* getProductName(WORD id)
{
	size_t arr_size = sizeof(PRODUCT_IDENTITIES) / sizeof(PRODUCT_IDENTITIES[0]);
	for (int i = 0; i < arr_size; i++)
	{
		ProductIdentity* identity = &PRODUCT_IDENTITIES[i];

		if (identity->id != id) continue;

		return identity->name;
	}

	return UNKNOWN_PRODUCT;
}

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

const char* getManufacturerName(WORD id)
{
	size_t arr_size = sizeof(MANUFACTURER_IDENTITIES) / sizeof(MANUFACTURER_IDENTITIES[0]);
	for (int i = 0; i < arr_size; i++)
	{
		ManufacturerIdentity* identity = &MANUFACTURER_IDENTITIES[i];

		if (identity->id != id) continue;

		return identity->name;
	}

	return UNKNOWN_MANUFACTURER;
}

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

// Should be a better way to write this
const char* getTechnology(WORD technology)
{
	size_t arr_size = sizeof(TECHNOLOGIES) / sizeof(TECHNOLOGIES[0]);
	for (int i = 0; i < arr_size; i++)
	{
		Technology* tech = &TECHNOLOGIES[i];

		if (tech->id != technology) continue;

		return tech->name;
	}

	return UNKNOWN_TECHNOLOGY;
}

std::string getDriverVersion(MMVERSION version)
{
	// Create inline function to get high and low words?
	int minor = HIWORD(version);
	int major = LOWORD(version);

	std::stringstream ss;
	ss << "v" << major << "." << minor;

	return ss.str();
}