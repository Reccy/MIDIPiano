#include<gtest/gtest.h>
#include "keymapper.h"

const struct KeymapTestData
{
	char input;
	int result;
};

std::ostream& operator<<(std::ostream& os, const KeymapTestData& keymapTestData)
{
	return os << "input: " << keymapTestData.input << " result: " << keymapTestData.result;
};

class KeymapperTest : public ::testing::TestWithParam<KeymapTestData> {
protected:
	MidiPiano::Core::Keymapper keymapper;
};

TEST_P(KeymapperTest, map) {
	int actual_result = keymapper.map(GetParam().input);
	int expected_result = GetParam().result;

	EXPECT_EQ(expected_result, actual_result);
}

KeymapTestData validParams[] = {
	{ 'Z', 36 },
	{ 'S', 37 },
	{ 'X', 38 },
	{ 'D', 39 },
	{ 'C', 40 },
	{ 'V', 41 },
	{ 'G', 42 },
	{ 'B', 43 },
	{ 'H', 44 },
	{ 'N', 45 },
	{ 'J', 46 },
	{ 'M', 47 },
	{ ',', 48 }
};

INSTANTIATE_TEST_SUITE_P(KeymapperValidInputs, KeymapperTest, ::testing::ValuesIn(validParams));

KeymapTestData invalidParams[] = {
	{ '!', -1 },
	{ 'Q', -1 },
	{ 'W', -1 },
	{ 'E', -1 },
	{ 'R', -1 },
	{ 'T', -1 },
	{ 'Y', -1 },
	{ 'z', -1 },
	{ 'x', -1 },
	{ 'c', -1 },
	{ 'v', -1 },
	{ 'b', -1 },
	{ 'n', -1 },
	{ 'm', -1 }
};

INSTANTIATE_TEST_SUITE_P(KeymapperInvalidInputs, KeymapperTest, ::testing::ValuesIn(invalidParams));
