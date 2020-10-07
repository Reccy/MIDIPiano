#include <gtest/gtest.h>
#include "keymapper.h"

class KeymapperTest : public ::testing::Test {
protected:
	MidiPiano::Core::Keymapper keymapper;
};

TEST_F(KeymapperTest, HandlesCharacterZ) {
	int result = keymapper.map('Z');
	EXPECT_EQ(result, 36);
}

TEST_F(KeymapperTest, HandlesCharacterS) {
	int result = keymapper.map('S');
	EXPECT_EQ(result, 37);
}

TEST_F(KeymapperTest, HandlesCharacterX) {
	int result = keymapper.map('X');
	EXPECT_EQ(result, 38);
}

TEST_F(KeymapperTest, HandlesCharacterD) {
	int result = keymapper.map('D');
	EXPECT_EQ(result, 39);
}

TEST_F(KeymapperTest, HandlesCharacterC) {
	int result = keymapper.map('C');
	EXPECT_EQ(result, 40);
}

TEST_F(KeymapperTest, HandlesCharacterV) {
	int result = keymapper.map('V');
	EXPECT_EQ(result, 41);
}

TEST_F(KeymapperTest, HandlesCharacterG) {
	int result = keymapper.map('G');
	EXPECT_EQ(result, 42);
}

TEST_F(KeymapperTest, HandlesCharacterB) {
	int result = keymapper.map('B');
	EXPECT_EQ(result, 43);
}

TEST_F(KeymapperTest, HandlesCharacterH) {
	int result = keymapper.map('H');
	EXPECT_EQ(result, 44);
}

TEST_F(KeymapperTest, HandlesCharacterN) {
	int result = keymapper.map('N');
	EXPECT_EQ(result, 45);
}

TEST_F(KeymapperTest, HandlesCharacterJ) {
	int result = keymapper.map('J');
	EXPECT_EQ(result, 46);
}

TEST_F(KeymapperTest, HandlesCharacterM) {
	int result = keymapper.map('M');
	EXPECT_EQ(result, 47);
}

TEST_F(KeymapperTest, HandlesCharacterComma) {
	int result = keymapper.map(',');
	EXPECT_EQ(result, 48);
}

TEST_F(KeymapperTest, HandlesInvalidCharacter) {
	int result = keymapper.map('!');
	EXPECT_EQ(result, -1);
}
