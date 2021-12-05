#include <assert.h>
#include <thread>
#include <iostream>
#include "Core/EngineCore.h"
#include "Core/WindowsPlatformDependent.h"
#include "GLFW/glfw3.h"
#include "Logger/Logger.h"
#include "Renderer/OpenGLRenderer.h"
#include "Window/OpenGLWindow.h"
#include "InputSystem/InputSystem.h"

const float IdealFrameRate = 60.f;

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
    WindowProperties defaultWindowProperties = {};
    MainWindow = RHI.Init(defaultWindowProperties);

    InputSystem::Init(*MainWindow);
    
}

void EngineCore::Run()
{
    WindowsPlatformDependent::InitCycles();
	double deltaTime = 0.0;
	double lastTime = WindowsPlatformDependent::GetTime();
	const float idealFrameTime = 1.0f / IdealFrameRate;

 	while (true)
 	{
 		double currentTime = WindowsPlatformDependent::GetTime();
 		double timeSpent = (currentTime - lastTime);

 		double timeLeft = idealFrameTime - timeSpent;

 		while (timeLeft > 0)
 		{
            WindowsPlatformDependent::Sleep(0);

			currentTime = WindowsPlatformDependent::GetTime();
			timeSpent = (currentTime - lastTime);

			timeLeft = idealFrameTime - timeSpent;
 		}

 		deltaTime = currentTime - lastTime;
		//Logger::GetLogger().Log("Delta time: %lf", deltaTime);

        //Call tickableObjects

        RHI.Draw(*MainWindow);

 		lastTime = currentTime;
 	}
}

