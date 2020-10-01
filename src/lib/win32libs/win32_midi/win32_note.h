#ifndef WIN32_NOTE_H
#define WIN32_NOTE_H

#ifndef UNICODE
#define UNICODE
#endif

// ASC - Move to midi.h ?
// Also consider changing this to array or other data structure
// Don't know if it'll ever be relevant to directory access a note by the name
// Could just add a mapping function?
struct Note
{
	char id;
	const char name[8];
};

#endif /* WIN32_NOTE_H */
