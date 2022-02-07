#include "TestGameMode.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/OpenGL/SimpleShapeDrawable.h"
#include "Renderer/ShapesUtils/BasicShapes.h"
#include "Controller/Controller.h"
#include "Entity/TransformEntity.h"
#include "glm/common.hpp"
#include <stdlib.h>

TestGameMode GameMode = {};

TestGameMode::TestGameMode()
	:
	GameModeBase(),
	GameCamera{ nullptr },
	GameController{}
{
}

TestGameMode::~TestGameMode() = default;

void TestGameMode::Init()
{
	GameController = eastl::make_unique<Controller>();

	// Debug
	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraLeft);
		KeyCode createFloorKey = KeyCode::A;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };
		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraRight);
		KeyCode createFloorKey = KeyCode::D;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };

		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraUp);
		KeyCode createFloorKey = KeyCode::W;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };
		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraDown);
		KeyCode createFloorKey = KeyCode::S;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };

		GameController->AddListener(createFloorAction);
	}
	// Debug

	// Scene setup
	SceneManager& sManager = SceneManager::Get();
	Scene& currentScene = sManager.GetCurrentScene();
	GameCamera = currentScene.CurrentCamera;
	GameCamera->SetMovementDelegates(*GameController);

	// Push camera back a bit
	GameCamera->Model.Translation.z = 10.f;

	Object = BasicShapes::CreateCubeObject();
	currentScene.AddEntity(Object);

	Ground = BasicShapes::CreateCubeObject();
	Ground->Model.Translation.y = -10.f;
	Ground->Model.Scale.x = 200.f;
	Ground->Model.Scale.z = 200.f;
	currentScene.AddEntity(Ground);

	{
		EntityPtr obj = BasicShapes::CreateCubeObject();
		obj->Model.Translation.x += 5.f;
		Object->AddChild(obj);
		//currentScene.AddEntity(obj);
	}
	{
		EntityPtr SecondModel = BasicShapes::CreateCubeObject();
		SecondModel->Model.Translation.z += 20.f;
		currentScene.AddEntity(SecondModel);
	}
}

void TestGameMode::Tick(float inDeltaT)
{
	GameController->ExecuteCallbacks();

	Object->Model.Rotate(2.f, glm::vec3(0.f, 1.f, 0.f));
	//Object->Model.Rotation.y += 0.5f;
}

void TestGameMode::MoveCameraLeft()
{
	GameCamera->Move(MovementDirection::Left);
	//GameCamera->Model.Rotate(5.f, glm::vec3(0.f, 1.f, 0.f));
}

void TestGameMode::MoveCameraRight()
{
	GameCamera->Move(MovementDirection::Right);
	//GameCamera->Model.Rotate(-5.f, glm::vec3(0.f, 1.f, 0.f));
}

void TestGameMode::MoveCameraUp()
{
	GameCamera->Move(MovementDirection::Forward);
}

void TestGameMode::MoveCameraDown()
{
	GameCamera->Move(MovementDirection::Back);
}
