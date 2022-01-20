#include "Core/EngineCore.h"
#include "Core/WindowsPlatform.h"
#include "GLFW/glfw3.h"
#include "Logger/Logger.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Window/OpenGLWindow.h"
#include "InputSystem/GLFWInput/InputSystem.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "IGameObject.h"
#include "Core/GameModeBase.h"

constexpr float IdealFrameRate = 60.f;
constexpr float IdealFrameTime = 1.0f / IdealFrameRate;
bool IsRunning = true;

void StopEngineRunning()
{
	IsRunning = false;
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

void EngineCore::Init(eastl::shared_ptr<GameModeBase> inGameMode)
{
	Engine = new EngineCore{};
	Engine->CurrentGameMode = inGameMode;

	// Init all engine subsystems
	OpenGLRenderer::Init();
	InputSystem::Init();
	SceneManager::Init();

	SceneManager::Get().LoadScene();

	// After initializing all engine subsystems, Game Mode init is called
	Engine->CurrentGameMode->Init();
}

void EngineCore::Terminate()
{
	SceneManager::Terminate();
	OpenGLRenderer::Terminate();
	InputSystem::Terminate();

	ASSERT(Engine);
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

		CurrentDeltaT = static_cast<float>(currentTime - lastTime);
		//Logger::GetLogger().Log("Delta time: %lf", CurrentDeltaT);
		lastTime = currentTime;

		InputSystem::Get().PollEvents();
		//Call tickableObjects: Camera, etc

		Scene& currentScene = SceneManager::Get().GetCurrentScene();
		RecursivelyTickObjects(currentScene.SceneObjects);
		CurrentGameMode->Tick(CurrentDeltaT);

		RHI->Draw();
	}

	Terminate();
}

void EngineCore::RecursivelyTickObjects(eastl::vector<eastl::shared_ptr<IGameObject>>&inObjects)
{
	for (eastl::shared_ptr<IGameObject>& obj : inObjects)
	{
		RecursivelyTickObjects(obj->Children);

		obj->Tick(CurrentDeltaT);
	}
}

