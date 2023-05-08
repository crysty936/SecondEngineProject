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
#include "Window/WindowsWindow.h"
#include "Renderer/RHI/RHI.h"
#include "Renderer/Renderer.h"
#include "backends/imgui_impl_win32.h"
#include "imgui.h"
#include "Editor/Editor.h"

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
	//Engine->CurrentGameMode = GameModeBase::Get();

	InputSystem::Init();

	// Create Main Window
	Engine->MainWindow = eastl::make_unique<WindowsWindow>();

	SceneManager::Init();

	MaterialsManager::Init();

	RHI::Init();
	RHI::Get()->ImGuiInit();

	// Hide Cursor for input
	InputSystem::Get().SetCursorMode(ECursorMode::Disabled, Engine->MainWindow->GetHandle());

	Renderer::Init();

	TimersManager::Init();

	//SceneManager::Get().LoadScene();

	// TODO [Editor-Game Separation]: Only if compiled with editor
	Editor::Init();

	// After initializing all engine subsystems, Game Mode init is called 
	// TODO [Editor-Game Separation]: This should be initialized like this only when editor is missing otherwise by the editor
	//Engine->CurrentGameMode->Init();
	//SceneManager::Get().GetCurrentScene().InitObjects();
}

void EngineCore::Terminate()
{
	// TODO [Editor-Game Separation]: Only if compiled with editor
	Editor::Terminate();

	TimersManager::Terminate();
	Renderer::Terminate();
	InputSystem::Terminate();

	RHI::Terminate();
	MaterialsManager::Terminate();

	SceneManager::Terminate();

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

		RHI::Get()->ImGuiBeginFrame();
 		ImGui_ImplWin32_NewFrame();
 		ImGui::NewFrame();
 
 		//ImGui::ShowDemoWindow();

 		SceneManager::Get().GetCurrentScene().TickObjects(CurrentDeltaT);
 		SceneManager::Get().GetCurrentScene().DisplayObjects();

		// TODO [Editor-Game Separation]: If editor is not present
 		//CurrentGameMode->Tick(CurrentDeltaT); 

		GEditor->Tick(CurrentDeltaT);

		Renderer::Get().Draw();

		// Draw ImGui
		ImGui::EndFrame();
		ImGui::Render();
		RHI::Get()->ImGuiRenderDrawData();

		// Update and Render additional Platform windows
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		Renderer::Get().Present();

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