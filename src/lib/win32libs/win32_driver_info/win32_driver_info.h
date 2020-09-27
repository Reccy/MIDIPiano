#ifndef WIN32_DRIVER_INFO_H
#define WIN32_DRIVER_INFO_H

#include <Windows.h>
#include <mmreg.h>

struct ProductIdentity
{
	WORD id;
	const char name[64]; // Reconsider having the string be in the stack, might be better to have it on the heap?
};

const char* getProductName(WORD id);

// PSC - Windows driver info - move to win32_driver_info.h ?
struct ManufacturerIdentity
{
	WORD id;
	const char name[64]; // Reconsider having the string be in the stack, might be better to have it on the heap?
};

const char* getManufacturerName(WORD id);

struct Technology
{
	WORD id;
	const char name[32];
};

const char* getTechnology(WORD technology);

#endif /* WIN32_DRIVER_INFO_H */
