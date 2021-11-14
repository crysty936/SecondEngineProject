#include <assert.h>
#include <thread>
#include <iostream>
#include <windows.h>
#include "Core/EngineCore.h"
#include "GLFW/glfw3.h"
#include "Logger/Logger.h"

const float IdealFrameRate = 60.f;
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

static __forceinline double GetTime()
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
	double lastTime = GetTime();
	const float idealFrameTime = 1.0f / IdealFrameRate;

 	while (true)
 	{
 		double currentTime = GetTime();
		//Logger::GetLogger().Log("Current time before sleep: %lf", currentTime);

 		double timeSpent = (currentTime - lastTime);
		//Logger::GetLogger().Log("Time spent: %lf", timeSpent);

 		// sleep for x amount to reach the ideal frame time
 		double timeLeft = idealFrameTime - timeSpent;
		//Logger::GetLogger().Log("Sleep Time: %lf", sleepTime);

 		while (timeLeft > 0)
 		{
			//uint32_t milliseconds = static_cast<uint32_t>(sleepTime * 1000.0);
			//Logger::GetLogger().Log("Sleep time in milliseconds: %i", milliseconds);
			//double timeBeforeSleep = GetTime();

			// Sleep for the minimmum amount of time so we get the best granilarity while still giving the thread away
            ::Sleep(0); 

			currentTime = GetTime();
			timeSpent = (currentTime - lastTime);

			//double timeSlept = currentTime - timeBeforeSleep;
			//Logger::GetLogger().Log("Slept for: %lf", timeSlept);

			timeLeft = idealFrameTime - timeSpent;
 		}

		//Logger::GetLogger().Log("Current time after sleep: %lf", currentTime);

 		deltaTime = currentTime - lastTime;
		//Logger::GetLogger().Log("Last time: %lf", lastTime);
		Logger::GetLogger().Log("Delta time: %lf", deltaTime);

 		lastTime = currentTime;
 	}
}

