#include "Core/EngineCore.h"
#include "Core/WindowsPlatform.h"
#include "GLFW/glfw3.h"
#include "Logger/Logger.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Window/OpenGLWindow.h"
#include "InputSystem/GLFWInput/InputSystem.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Entity/Entity.h"
#include "Core/GameModeBase.h"
#include "Timer/TimersManager.h"
#include "Renderer/Material/MaterialsManager.h"

constexpr float IdealFrameRate = 60.f;
constexpr float IdealFrameTime = 1.0f / IdealFrameRate;
bool bIsRunning = true;

void StopEngine()
{
	bIsRunning = false;
}

EngineCore* Engine = nullptr;

EngineCore::EngineCore()
	: CurrentDeltaT{ 0.f }
{
	static bool engineExists = false;
	ASSERT(!engineExists);

	engineExists = true;
}

EngineCore::~EngineCore() = default;

void EngineCore::Init()
{
	Engine = new EngineCore{};
	Engine->CurrentGameMode = GameModeBase::Get();

	// Init all engine subsystems
	OpenGLRenderer::Init();
#if WITH_GLFW
	InputSystem::Init();
#endif
	SceneManager::Init();
	TimersManager::Init();
	MaterialsManager::Init();

	SceneManager::Get().LoadScene();

	// After initializing all engine subsystems, Game Mode init is called
	Engine->CurrentGameMode->Init();
	SceneManager::Get().GetCurrentScene().InitObjects();
}

void EngineCore::Terminate()
{
	MaterialsManager::Terminate();
	TimersManager::Terminate();
	SceneManager::Terminate();
	OpenGLRenderer::Terminate();
#if WITH_GLFW
	InputSystem::Terminate();
#endif

	ASSERT(Engine);
	delete Engine;
}

void EngineCore::Run()
{
	WindowsPlatform::InitCycles();
	double deltaTime = 0.0;
	double lastTime = WindowsPlatform::GetTime();

	while (bIsRunning)
	{
		double currentTime = WindowsPlatform::GetTime();
		double timeSpent = currentTime - lastTime;
		double timeLeft = IdealFrameTime - timeSpent;

		// Sleep 0 until time is out, granularity can be set to avoid this but this works well
		while (timeLeft > 0)
		{
			WindowsPlatform::Sleep(0);

			currentTime = WindowsPlatform::GetTime();
			timeSpent = currentTime - lastTime;

			timeLeft = IdealFrameTime - timeSpent;
		}

		CurrentDeltaT = static_cast<float>(currentTime - lastTime);
		//Logger::GetLogger().Log("Delta time: %lf", CurrentDeltaT);
		lastTime = currentTime;

#if WITH_GLFW
		InputSystem::Get().PollEvents();
#endif
		//Call tickableObjects: Camera, etc

		// Tick Timers
		TimersManager::Get().TickTimers(CurrentDeltaT);

		SceneManager::Get().GetCurrentScene().TickObjects(CurrentDeltaT);
		CurrentGameMode->Tick(CurrentDeltaT);

		RHI->Draw();
	}

	Terminate();
}

bool EngineCore::IsRunning()
{
	return bIsRunning;
}
