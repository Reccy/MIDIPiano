#include <win32_note.h>
#include <ctype.h>
#include <string>

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

// ASC - Make sure the PSC sends in generic info to this function, so it can be handled by other platforms - define interface
// Legit, this function should be handled by the MidiPiano::Core code
// Maybe MidiPiano::Core::VirtualPiano.charToNote(character) -> note
// ...
// MidiPiano::Core::VirtualPiano.playNote(note)
// NOTE(aaron.meaney): This will essentially be the keymapper, it is hard coded at the moment
Note* charToNote(const char* character)
{
	Note* note{};

	std::string cmp = std::string() + character;

	// NOTE(aaron.meaney): This is terrible, terrible code - replace this once I get to the proper keymapper feature
	if (cmp == "Z") { note = &C3; }
	if (cmp == "S") { note = &CSharp3; }
	if (cmp == "X") { note = &D3; }
	if (cmp == "D") { note = &DSharp3; }
	if (cmp == "C") { note = &E3; }
	if (cmp == "V") { note = &F3; }
	if (cmp == "G") { note = &FSharp3; }
	if (cmp == "B") { note = &G3; }
	if (cmp == "H") { note = &GSharp3; }
	if (cmp == "N") { note = &A4; }
	if (cmp == "J") { note = &ASharp4; }
	if (cmp == "M") { note = &B4; }

	if (note == nullptr) {
		return nullptr;
	}

	return note;
}
