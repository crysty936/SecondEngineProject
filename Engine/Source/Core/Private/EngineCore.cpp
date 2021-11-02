#include <assert.h>
#include <thread>
#include <iostream>
#include <windows.h>
#include "Core/Public/EngineCore.h"
#include "GLFW/glfw3.h"

const float IdealFrameRate = 120.f;

EngineCore Engine;

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

// Windows platform code for getting time spent
static __forceinline double seconds()
{
	constexpr double secondsPerCycle = 1.0f / 1000000.0f;
	LARGE_INTEGER cycles;
	::QueryPerformanceCounter(&cycles);

    return cycles.QuadPart * secondsPerCycle + 16777216.0;
}

void EngineCore::Run()
{
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
            uint32_t milliseconds = (uint32_t)(sleepTime * 1000.0);
            ::Sleep(milliseconds);
 		}
 
 		currentTime = seconds();
 		deltaTime = currentTime - lastTime;
 		lastTime = currentTime;
 
        std::cout << "Frame time: " << deltaTime << std::endl << std::flush;
 	}
}

