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
	if (EntityPtr parentShared = GameCamera->GetParent().lock())
	{
		parentShared->GetRelativeTransform().Translation.z = 10.f;
	}
	// Rotate slightly to the side so that we know we're doing additive rotation further on
	//GameCamera->GetRelativeTransform().Rotate(-45.f, glm::vec3(0.f, 1.f, 0.f));

	Object = BasicShapes::CreateCubeObject();
	currentScene.AddEntity(Object);

	Ground = BasicShapes::CreateCubeObject();
	Ground->GetRelativeTransform().Translation.y = -10.f;
	Ground->GetRelativeTransform().Scale.x = 200.f;
	Ground->GetRelativeTransform().Scale.z = 200.f;
	currentScene.AddEntity(Ground);

	{
		Obj = BasicShapes::CreateCubeObject();
		Obj->GetRelativeTransform().Translation.x += 5.f;
		Object->AddChild(Obj);

		Obj2 = BasicShapes::CreateCubeObject();
		Obj2->GetRelativeTransform().Translation.x += 3.f;
		Obj->AddChild(Obj2);
		Obj2->GetRelativeTransform().Scale.x = 0.5f;
		Obj2->GetRelativeTransform().Scale.y = 0.5f;
		Obj2->GetRelativeTransform().Scale.z = 0.5f;
	}
	{
		EntityPtr SecondModel = BasicShapes::CreateCubeObject();
		SecondModel->GetRelativeTransform().Translation.z += 20.f;
		currentScene.AddEntity(SecondModel);
	}
}

void TestGameMode::Tick(float inDeltaT)
{
	GameController->ExecuteCallbacks();

	Object->GetRelativeTransform().Rotate(2.f, glm::vec3(0.f, 1.f, 0.f));
	Obj->GetRelativeTransform().Rotate(2.f, glm::vec3(0.f, 1.f, 0.f));
	Obj2->GetRelativeTransform().Rotate(8.f, glm::vec3(0.f, 1.f, 0.f));
	//Object->GetRelativeTransform().Rotation.y += 0.5f;
}

void TestGameMode::MoveCameraLeft()
{
	GameCamera->Move(MovementDirection::Left);
	//GameCamera->GetRelativeTransform().Rotate(5.f, glm::vec3(0.f, 1.f, 0.f));
}

void TestGameMode::MoveCameraRight()
{
	GameCamera->Move(MovementDirection::Right);
	//GameCamera->GetRelativeTransform().Rotate(-5.f, glm::vec3(0.f, 1.f, 0.f));
}

void TestGameMode::MoveCameraUp()
{
	GameCamera->Move(MovementDirection::Forward);
}

void TestGameMode::MoveCameraDown()
{
	GameCamera->Move(MovementDirection::Back);
}
