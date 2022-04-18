#include "TestGameMode.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/Drawable/BasicShapes.h"
#include "Controller/Controller.h"
#include "glm/common.hpp"
#include <stdlib.h>
#include "Renderer/Model/3D/Assimp/AssimpModel3D.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Core/ObjectCreation.h"

TestGameMode GameMode = {};

TestGameMode::TestGameMode()
	:
	GameModeBase()
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

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::OnChangeDrawMode);
		KeyCode createFloorKey = KeyCode::F;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, true };

		GameController->AddListener(createFloorAction);
	}
	// Debug

	// Scene setup
	SceneManager& sManager = SceneManager::Get();
	Scene& currentScene = sManager.GetCurrentScene();
	GameCamera = currentScene.CurrentCamera;
	GameCamera->SetMovementDelegates(*GameController);

	// Push camera back a bit
	if (TransformObjPtr parentShared = GameCamera->GetParent().lock())
	{
		// Move the camera parent
		parentShared->Move(glm::vec3(0.f, 0.f, 10.f));
	}

	// 	{
	// 		eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();
	// 	}
	{
		eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();
		centerObj->Move(glm::vec3(0.f, 10.f, 0.f));
	}
	{
		eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();
		centerObj->Move(glm::vec3(0.f, 20.f, 0.f));
	}
	{
		eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();
		centerObj->Move(glm::vec3(0.f, 30.f, 0.f));
	}

	// 	eastl::shared_ptr<AssimpModel3D> model = ObjectCreation::NewObject<AssimpModel3D>("../Data/Models/Backpack/scene.gltf");
	// 	model->Move(glm::vec3(5.f, 5.f, 5.f));
	// 	model->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));


	//eastl::shared_ptr<AssimpModel3D> shibaModel = ObjectCreation::NewObject<AssimpModel3D>("../Data/Models/Shiba/scene.gltf");

	{
		eastl::shared_ptr<SquareShape> square = BasicShapes::CreateSquareObject();
		//centerObj->Move(glm::vec3(0.f, 30.f, 0.f));
	}
	

}

void TestGameMode::Tick(float inDeltaT)
{
	GameController->ExecuteCallbacks();

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

void TestGameMode::OnChangeDrawMode()
{
	static bool drawMode = false;

	RHI->SetDrawMode(drawMode ? EDrawMode::NORMAL : EDrawMode::DEPTH);

	drawMode = !drawMode;
}
