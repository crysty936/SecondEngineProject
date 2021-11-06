#include <assert.h>
#include <thread>
#include <iostream>
#include <windows.h>
#include "Core/EngineCore.h"
#include "GLFW/glfw3.h"
#include "Logger/Logger.h"

const float IdealFrameRate = 120.f;
static double SecondsPerCycle = 0;

EngineCore Engine;

static __forceinline void InitCycles()
{
	// Initialise required frequency for seconds calc
	::LARGE_INTEGER Frequency;
	::QueryPerformanceFrequency(&Frequency);
	SecondsPerCycle = 1.0 / Frequency.QuadPart;

   
    // Set the main thread to work only on one core to avoid timing synchrionization problems between multiple cores
    // Source: https://docs.microsoft.com/en-us/windows/win32/dxtecharts/game-timing-and-multicore-processors
    DWORD_PTR threadMask = static_cast<DWORD_PTR>(1);
    ::SetThreadAffinityMask(::GetCurrentThread(), threadMask);
}

static __forceinline double seconds()
{
    // Making use of Windows Platform functions for getting the time
	::LARGE_INTEGER cycles;
	::QueryPerformanceCounter(&cycles);

	return cycles.QuadPart * SecondsPerCycle + 16777216.0;
}


EngineCore::EngineCore()
{
	static bool engineExists = false;
	assert(!engineExists);

	engineExists = true;
}

EngineCore::~EngineCore() = default;

void EngineCore::Init()
{
	glfwInit();


}

void EngineCore::Run()
{
    InitCycles();
	double deltaTime = 0.0;
	double lastTime = seconds();
	const float idealFrameTime = 1.0f / IdealFrameRate;

 	while (true)
 	{
 		double currentTime = seconds();
 		double timeSpent = (currentTime - lastTime);
 		// sleep for x amount to reach the ideal frame time
 		double sleepTime = idealFrameTime - timeSpent;
 		if (sleepTime > 0)
 		{
            uint32_t milliseconds = static_cast<uint32_t>(sleepTime * 1000.0);
            ::Sleep(milliseconds);
 		}
 
 		currentTime = seconds();
 		deltaTime = currentTime - lastTime;
 		lastTime = currentTime;
 
        Logger::GetLogger().Log("Frame time %lf", deltaTime);

        //std::cout << "Frame time: " << deltaTime << std::endl << std::flush;
 	}
}

