#include "EABase/eabase.h"

// Necessary overload of operator new for EASTL to work.

void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	return new uint8_t[size];
}
