#include <gtest/gtest.h>
#include "midi_piano.h"

class MidiPianoTest : public ::testing::Test {
	MidiPianoTest()
	{}
protected:
	MidiPiano::Core::MidiPiano piano;
};

TEST_F(MidiPianoTest, TestFuncReturns300) {
	int result = piano.test_func();
	EXPECT_EQ(result, 300);
}
