#include <assert.h>
#include <thread>
#include <iostream>
#include "Core/EngineCore.h"
#include "Core/WindowsPlatform.h"
#include "GLFW/glfw3.h"
#include "Logger/Logger.h"
#include "Renderer/OpenGLRenderer.h"
#include "Window/OpenGLWindow.h"
#include "InputSystem/InputSystem.h"

const float IdealFrameRate = 60.f;

bool IsRunning = true;

void StopEngineRunning()
{
    IsRunning = false;
}

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

    WindowProperties defaultWindowProperties{};
    RHI.Init(defaultWindowProperties);

    InputSystem::Init();
    



}

void EngineCore::Run()
{
    WindowsPlatform::InitCycles();
	double deltaTime = 0.0;
	double lastTime = WindowsPlatform::GetTime();
	const float idealFrameTime = 1.0f / IdealFrameRate;

 	while (IsRunning)
 	{
 		double currentTime = WindowsPlatform::GetTime();
 		double timeSpent = currentTime - lastTime;

 		double timeLeft = idealFrameTime - timeSpent;

        // Sleep 0 until time is out, granularity can be set to avoid this but it's unnecessary given
        // current context where this works well
 		while (timeLeft > 0)
 		{
            WindowsPlatform::Sleep(0);

			currentTime = WindowsPlatform::GetTime();
			timeSpent = currentTime - lastTime;

			timeLeft = idealFrameTime - timeSpent;
 		}

 		deltaTime = currentTime - lastTime;
		//Logger::GetLogger().Log("Delta time: %lf", deltaTime);

        //Call tickableObjects

        RHI.Draw();

 		lastTime = currentTime;
 	}

    RHI.Terminate();
}

