#include "Core/EngineCore.h"
#include "Core/WindowsPlatform.h"
#include "GLFW/glfw3.h"
#include "Logger/Logger.h"
#include "Window/OpenGLWindow.h"
#include "InputSystem/InputSystem.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Entity/Entity.h"
#include "Core/GameModeBase.h"
#include "Timer/TimersManager.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/Renderer.h"

constexpr float IdealFrameRate = 60.f;
constexpr float IdealFrameTime = 1.0f / IdealFrameRate;
bool bIsRunning = true;

void StopEngine()
{
	bIsRunning = false;
}

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
	InputSystem::Init();
	Renderer::Init();
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
	Renderer::Terminate();
	InputSystem::Terminate();

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
		eastl::wstring text;
		text.sprintf(L"Ms: %f", CurrentDeltaT);
		WindowsPlatform::SetWindowsWindowText(text);

		//Logger::GetLogger().Log("Delta time: %lf", CurrentDeltaT);
		lastTime = currentTime;

		InputSystem::Get().PollEvents();
		//Call tickableObjects: Camera, etc

		// Tick Timers
		TimersManager::Get().TickTimers(CurrentDeltaT);

		SceneManager::Get().GetCurrentScene().TickObjects(CurrentDeltaT);
		CurrentGameMode->Tick(CurrentDeltaT);

		Renderer::RHI->Draw();
	}

	Terminate();
}

bool EngineCore::IsRunning()
{
	return bIsRunning;
}
