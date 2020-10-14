#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "i_midi_out.h"
#include "i_logger.h"
#include "midi_piano.h"

class MidiOutMock : public MidiPiano::Core::IMidiOut
{
	MOCK_METHOD(void, playNote, (int));
	MOCK_METHOD(void, stopNote, (int));
};

class LoggerMock : public MidiPiano::Core::ILogger
{
	MOCK_METHOD(void, log, (std::wstring));
	MOCK_METHOD(void, log, (wchar_t));
	MOCK_METHOD(void, log, (std::string));
	MOCK_METHOD(void, log, (char));
};

class MidiPianoTest : public ::testing::Test {
	MidiOutMock midiOutMock;
	LoggerMock loggerMock;

	MidiPiano::Core::MidiPiano piano { &midiOutMock, &loggerMock };
};
