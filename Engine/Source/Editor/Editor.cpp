#include "Editor.h"
#include "Controller/EditorController.h"
#include "Scene/SceneManager.h"
#include "Entity/TransformObject.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Renderer/ForwardRenderer.h"
#include "Core/GameModeBase.h"
#include "imgui.h"

Editor* GEditor = nullptr;

void Editor::Init()
{
	GEditor = new Editor();
	GEditor->Internal_PostInit();
}

void Editor::Terminate()
{
	delete GEditor;
}

void Editor::Tick(float inDeltaT)
{
	Controller->ExecuteCallbacks();

	// TODO [Editor-Game Separation]: Only do this based on if the game is started
	CurrentGameMode->Tick(inDeltaT);
}

Editor::Editor()
{
	Controller = eastl::make_unique<EditorController>();
}

Editor::~Editor() = default;

void Editor::Internal_PostInit()
{
	CurrentGameMode = GameModeBase::Get();
	CurrentGameMode->Init();

	SceneManager& sManager = SceneManager::Get();
	Scene& currentScene = sManager.GetCurrentScene();
	ViewportCamera = currentScene.CurrentCamera;
	ViewportCamera->SetMovementDelegates(*Controller);


	{
		KeyActionDelegate del = KeyActionDelegate::CreateRaw(this, &Editor::MoveCameraLeft);
		EInputKey key = EInputKey::A;
		OnKeyAction action = { del, {}, key, false };
		Controller->AddListener(action);
	}

	{
		KeyActionDelegate del = KeyActionDelegate::CreateRaw(this, &Editor::MoveCameraRight);
		EInputKey key = EInputKey::D;
		OnKeyAction action = { del, {}, key, false };

		Controller->AddListener(action);
	}

	{
		KeyActionDelegate del = KeyActionDelegate::CreateRaw(this, &Editor::MoveCameraUp);
		EInputKey key = EInputKey::W;
		OnKeyAction action = { del, {}, key, false };
		Controller->AddListener(action);
	}

	{
		KeyActionDelegate del = KeyActionDelegate::CreateRaw(this, &Editor::MoveCameraDown);
		EInputKey key = EInputKey::S;
		OnKeyAction action = { del, {}, key, false };

		Controller->AddListener(action);
	}

	{
		KeyActionDelegate del = KeyActionDelegate::CreateRaw(this, &Editor::OnChangeDrawMode);
		EInputKey key = EInputKey::F;
		OnKeyAction action = { del, {}, key, true };

		Controller->AddListener(action);
	}
	{
		KeyActionDelegate del = KeyActionDelegate::CreateRaw(this, &Editor::DebugProjections);
		EInputKey key = EInputKey::G;
		OnKeyAction action = { del, {}, key, true };
		Controller->AddListener(action);
	}

	{
		KeyActionDelegate del = KeyActionDelegate::CreateRaw(this, &Editor::DebugCascadesVisualize);
		EInputKey key = EInputKey::T;
		OnKeyAction action = { del, {}, key, true };
		Controller->AddListener(action);
	}

	{
		KeyActionDelegate del = KeyActionDelegate::CreateRaw(this, &Editor::DebugCursorMode);
		EInputKey key = EInputKey::Q;
		OnKeyAction action = { del, {}, key, true };
		Controller->AddListener(action);
	}

	{
		KeyActionDelegate del = KeyActionDelegate::CreateRaw(this, &Editor::BoostCameraSpeed);
		EInputKey key = EInputKey::LeftShift;
		OnKeyAction action = { del, {}, key, true };
		Controller->AddListener(action);
	}

	{
		KeyActionDelegate mouseRightPressedDel = KeyActionDelegate::CreateRaw(this, &Editor::OnMouseRightPressed);
		KeyActionDelegate mouseRightReleasedDel = KeyActionDelegate::CreateRaw(this, &Editor::OnMouseRightReleased);

		EInputKey key = EInputKey::MouseRight;
		OnKeyAction action = { mouseRightPressedDel, mouseRightReleasedDel, key, true };
		Controller->AddListener(action);
	}

	InputSystem::Get().SetCursorMode(ECursorMode::Enabled);
}

float CameraSpeed = 0.4f;

void Editor::MoveCameraLeft()
{
	if (!InViewportNavigateMode)
	{
		return;
	}

	ViewportCamera->Move(MovementDirection::Left, CameraSpeed);
	//ViewportCamera->GetRelativeTransform().Rotate(5.f, glm::vec3(0.f, 1.f, 0.f));
}

void Editor::MoveCameraRight()
{
	if (!InViewportNavigateMode)
	{
		return;
	}

	ViewportCamera->Move(MovementDirection::Right, CameraSpeed);
	//ViewportCamera->GetRelativeTransform().Rotate(-5.f, glm::vec3(0.f, 1.f, 0.f));
}

void Editor::MoveCameraUp()
{
	if (!InViewportNavigateMode)
	{
		return;
	}

	ViewportCamera->Move(MovementDirection::Forward, CameraSpeed);
}

void Editor::MoveCameraDown()
{
	if (!InViewportNavigateMode)
	{
		return;
	}

	ViewportCamera->Move(MovementDirection::Back, CameraSpeed);
}

void Editor::OnChangeDrawMode()
{
	static bool drawMode = false;

	//RHI->SetDrawMode(drawMode ? EDrawMode::NORMAL : EDrawMode::DEPTH);

	drawMode = !drawMode;
}

void Editor::DebugProjections()
{
	ForwardRenderer::Get().UpdateShadowMatrices = !ForwardRenderer::Get().UpdateShadowMatrices;
}

void Editor::DebugCascadesVisualize()
{
	ForwardRenderer::Get().bCascadeVisualizeMode = !ForwardRenderer::Get().bCascadeVisualizeMode;
}

static bool mouseTest = false;

void Editor::DebugCursorMode()
{
	const ECursorMode mode = mouseTest ? ECursorMode::Enabled : ECursorMode::Disabled;
	InputSystem::Get().SetCursorMode(mode);

	mouseTest = !mouseTest;
}

void Editor::BoostCameraSpeed()
{
	static bool boostOn = false;

	if (boostOn)
	{
		CameraSpeed = 0.4f;
	}
	else
	{
		CameraSpeed = 10.f;
	}

	boostOn = !boostOn;
}

void Editor::OnMouseRightPressed()
{
	InputSystem::Get().SetCursorMode(ECursorMode::Disabled);
	InViewportNavigateMode = true;
	LOG_INFO("Viewport navigate mode ON.");
}

void Editor::OnMouseRightReleased()
{
	// Check if we have successfully gone into viewport navigation mode
	if (!InViewportNavigateMode)
	{
		return;
	}

	InputSystem::Get().SetCursorMode(ECursorMode::Enabled);
	InViewportNavigateMode = false;
	LOG_INFO("Viewport navigate mode OFF.");
}





