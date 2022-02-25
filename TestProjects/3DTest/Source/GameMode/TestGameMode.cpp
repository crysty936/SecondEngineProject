#include "TestGameMode.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/Drawable/SimpleShapeDrawable.h"
#include "Renderer/Drawable/BasicShapes.h"
#include "Controller/Controller.h"
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
		// Move the camera parent
		parentShared->Move(glm::vec3(0.f, 0.f, 10.f));
	}

	//GameCamera->Rotate(-45.f, glm::vec3(0.f, 1.f, 0.f));

 	Object = BasicShapes::CreateCubeObject();
 	currentScene.AddEntity(Object);

	Ground = BasicShapes::CreateCubeObject();
	Ground->Move(glm::vec3(0.f, -10.f, 0.f));
	Ground->SetScale(glm::vec3(200.f, 1.f, 200.f));
	currentScene.AddEntity(Ground);
 
 	{
 		Obj = BasicShapes::CreateCubeObject();
 		Obj->Move(glm::vec3(5.f, 0., 0.f));
 		Object->AddChild(Obj);
 
 		Obj2 = BasicShapes::CreateCubeObject();
 		Obj2->Move(glm::vec3(3.f, 0.f, 0.f));
 		Obj->AddChild(Obj2);
 		Obj->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
 	}
 	{
 		EntityPtr SecondModel = BasicShapes::CreateCubeObject();
 		SecondModel->Move(glm::vec3(0.f, 0.f, 20.f));
 		currentScene.AddEntity(SecondModel);
 	}
}

void TestGameMode::Tick(float inDeltaT)
{
	GameController->ExecuteCallbacks();

	Object->Rotate(2.f, glm::vec3(0.f, 1.f, 0.f));
	Obj->Rotate(2.f, glm::vec3(0.f, 1.f, 0.f));
	Obj2->Rotate(8.f, glm::vec3(0.f, 1.f, 0.f));
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
