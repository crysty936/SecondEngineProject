#pragma once
#include <stdint.h>

// Wrapper for windows.h to not pollute the dev environment

namespace WindowsPlatformDependent
{
	void InitCycles();
	double GetTime();
	void Sleep(uint32_t inMilliseconds);
}
