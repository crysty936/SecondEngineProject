#include "Core/EngineCore.h"
#include "Core/WindowsPlatform.h"
#include "Logger/Logger.h"
#include "InputSystem/InputSystem.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Entity/Entity.h"
#include "Core/GameModeBase.h"
#include "Timer/TimersManager.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/TempRenderWrapper.h"
#include "Window/WindowsWindow.h"
#include "Renderer/RHI/RHI.h"

constexpr float IdealFrameRate = 60.f;
constexpr float IdealFrameTime = 1.0f / IdealFrameRate;
bool bIsRunning = true;

EngineCore::EngineCore()
	: CurrentDeltaT{ 0.f }
{
	static bool engineExists = false;
	ASSERT(!engineExists);

	engineExists = true;
}

EngineCore::~EngineCore() = default;

// Init all engine subsystems
void EngineCore::Init()
{
	Engine = new EngineCore{};
	Engine->CurrentGameMode = GameModeBase::Get();

	InputSystem::Init();

	// Create Main Window
	Engine->MainWindow = eastl::make_unique<WindowsWindow>();

	// Hide Cursor for input
	InputSystem::Get().SetCursorMode(Engine->MainWindow->GetHandle(), ECursorMode::Disabled);

	RHI::Init();

	TempRenderWrapper::Init();
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
	TempRenderWrapper::Terminate();
	RHI::Terminate();
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

		TempRenderWrapper::Draw();

		CheckShouldCloseWindow();
	}

	Terminate();
}

void EngineCore::CheckShouldCloseWindow()
{
	if (MainWindow->ShouldClose())
	{
		StopEngine();
	}
}

bool EngineCore::IsRunning()
{
	return bIsRunning;
}

void EngineCore::StopEngine()
{
	bIsRunning = false;
}