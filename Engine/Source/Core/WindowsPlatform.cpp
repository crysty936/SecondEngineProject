#include "Core/WindowsPlatform.h"
#include <windows.h>

namespace WindowsPlatform
{
	// Class which exposes Windows.h functionality
	static double SecondsPerCycle = 0;

	void InitCycles()
	{
		// Set the main thread to work only on one core to avoid timing synchrionization problems between multiple cores
		// Source: https://docs.microsoft.com/en-us/windows/win32/dxtecharts/game-timing-and-multicore-processors
		DWORD_PTR threadMask = static_cast<DWORD_PTR>(1);
		::SetThreadAffinityMask(::GetCurrentThread(), threadMask);

		// Initialise required frequency for seconds calc
		::LARGE_INTEGER Frequency;
		::QueryPerformanceFrequency(&Frequency);
		SecondsPerCycle = 1.0 / Frequency.QuadPart;
	}

	double GetTime()
	{
		// Making use of Windows Platform functions for getting the time
		::LARGE_INTEGER cycles;
		::QueryPerformanceCounter(&cycles);

		return cycles.QuadPart * SecondsPerCycle + 16777216.0;
	}

 	void Sleep(uint32_t inMilliseconds)
 	{
 		::Sleep(inMilliseconds);
 	}
}