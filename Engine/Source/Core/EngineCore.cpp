#include "Core/EngineCore.h"
#include "Core/WindowsPlatform.h"
#include "GLFW/glfw3.h"
#include "Logger/Logger.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Window/OpenGLWindow.h"
#include "InputSystem/GLFWInput/InputSystem.h"

constexpr float IdealFrameRate = 60.f;
constexpr float IdealFrameTime = 1.0f / IdealFrameRate;
bool IsRunning = true;

void StopEngineRunning()
{
    IsRunning = false;
}

EngineCore* Engine = nullptr;

EngineCore::EngineCore()
{
	static bool engineExists = false;
	ASSERT(!engineExists);

	engineExists = true;
}

EngineCore::~EngineCore() = default;

void EngineCore::Init()
{
    Engine = new EngineCore{};

    WindowProperties defaultWindowProperties{};
    InputSystem::Init();
    OpenGLRenderer::Init(defaultWindowProperties);

}

void EngineCore::Terminate()
{


    OpenGLRenderer::Terminate();

	InputSystem::Terminate();

    delete Engine;
}

void EngineCore::Run()
{
    WindowsPlatform::InitCycles();
	double deltaTime = 0.0;
	double lastTime = WindowsPlatform::GetTime();

 	while (IsRunning)
 	{
 		double currentTime = WindowsPlatform::GetTime();
 		double timeSpent = currentTime - lastTime;

 		double timeLeft = IdealFrameTime - timeSpent;

        // Sleep 0 until time is out, granularity can be set to avoid this but it's unnecessary given
        // current context where this works well
 		while (timeLeft > 0)
 		{
            WindowsPlatform::Sleep(0);

			currentTime = WindowsPlatform::GetTime();
			timeSpent = currentTime - lastTime;

			timeLeft = IdealFrameTime - timeSpent;
 		}

 		deltaTime = currentTime - lastTime;
		//Logger::GetLogger().Log("Delta time: %lf", deltaTime);
 		lastTime = currentTime;

        InputSystem::Get().PollEvents();
        //Call tickableObjects: Camera, etc

        RHI->Draw();
 	}

    Terminate();
}

