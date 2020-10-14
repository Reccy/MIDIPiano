#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "i_midi_out.h"
#include "i_logger.h"
#include "midi_piano.h"

class MidiOutMock : public MidiPiano::Core::IMidiOut
{
public:
	MOCK_METHOD(void, playNote, (int));
	MOCK_METHOD(void, stopNote, (int));
	MOCK_METHOD(void, silenceAllNotes, ());
};

class LoggerMock : public MidiPiano::Core::ILogger
{
public:
	MOCK_METHOD(void, log, (std::wstring));
	MOCK_METHOD(void, log, (wchar_t));
	MOCK_METHOD(void, log, (std::string));
	MOCK_METHOD(void, log, (char));
};

class MidiPianoTest : public ::testing::Test {
protected:
	MidiOutMock midiOutMock;
	LoggerMock loggerMock;

	MidiPiano::Core::MidiPiano piano { &midiOutMock, &loggerMock };

	const char TEST_KEY_Z = 'Z';
	const int TEST_MIDI_NOTE_Z = 36;

	const char TEST_KEY_X = 'X';
	const int TEST_MIDI_NOTE_X = 38;

	void GivenKeydown(char key)
	{
		piano.onKeydown(key);
	}
};

TEST_F(MidiPianoTest, OnKeydown_PlaysNote)
{
	EXPECT_CALL(midiOutMock, playNote(TEST_MIDI_NOTE_X))
		.Times(1);

	piano.onKeydown(TEST_KEY_X);
};

TEST_F(MidiPianoTest, OnKeydown_PlaysNote_OnlyOnce)
{
	EXPECT_CALL(midiOutMock, playNote(TEST_MIDI_NOTE_X))
		.Times(1);

	piano.onKeydown(TEST_KEY_X);
	piano.onKeydown(TEST_KEY_X);
}

TEST_F(MidiPianoTest, OnKeyup_SilencesNote)
{
	piano.onKeydown(TEST_KEY_X);

	EXPECT_CALL(midiOutMock, stopNote(TEST_MIDI_NOTE_X))
		.Times(1);

	piano.onKeyup(TEST_KEY_X);
}

TEST_F(MidiPianoTest, OnKeyup_SilencesNote_OnlyOnce)
{
	EXPECT_CALL(midiOutMock, stopNote(TEST_MIDI_NOTE_X))
		.Times(1);

	piano.onKeydown(TEST_KEY_X);
	piano.onKeyup(TEST_KEY_X);
	piano.onKeyup(TEST_KEY_X);
}

TEST_F(MidiPianoTest, OnKeyup_IgnoresInvalidNote)
{
	piano.onKeyup(TEST_KEY_X);
}

TEST_F(MidiPianoTest, SilenceAllNotes_StopsAllNotes)
{
	EXPECT_CALL(midiOutMock, stopNote(TEST_MIDI_NOTE_X))
		.Times(1);

	EXPECT_CALL(midiOutMock, stopNote(TEST_MIDI_NOTE_Z))
		.Times(1);

	piano.onKeydown(TEST_KEY_X);
	piano.onKeydown(TEST_KEY_Z);

	piano.silenceAllNotes();
}

TEST_F(MidiPianoTest, SilenceAllNotes_WithNoPlayingNotes)
{
	piano.silenceAllNotes();
}
