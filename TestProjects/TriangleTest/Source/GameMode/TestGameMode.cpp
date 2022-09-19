#include "TestGameMode.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapes.h"
#include "Controller/Controller.h"
#include "glm/common.hpp"
#include <stdlib.h>
#include "Renderer/Model/3D/Assimp/AssimpModel3D.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Core/ObjectCreation.h"
#include "Renderer/Drawable/MirrorQuad.h"

TestGameMode GameMode = {};

TestGameMode::TestGameMode()
	: GameModeBase()
{}

TestGameMode::~TestGameMode() = default;

void TestGameMode::Init()
{
	//GameController = eastl::make_unique<Controller>();

	triangle = ObjectCreation::NewObject<TriangleShape>();
	triangle->Move(glm::vec3(0.f, 0.f, -2.f));

// 	{
// 		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraLeft);
// 		KeyCode createFloorKey = KeyCode::A;
// 		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };
// 		GameController->AddListener(createFloorAction);
// 	}
// 
// 	{
// 		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraRight);
// 		KeyCode createFloorKey = KeyCode::D;
// 		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };
// 
// 		GameController->AddListener(createFloorAction);
// 	}
// 
// 	{
// 		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraUp);
// 		KeyCode createFloorKey = KeyCode::W;
// 		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };
// 		GameController->AddListener(createFloorAction);
// 	}
// 
// 	{
// 		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraDown);
// 		KeyCode createFloorKey = KeyCode::S;
// 		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };
// 
// 		GameController->AddListener(createFloorAction);
// 	}

// 	SceneManager& sManager = SceneManager::Get();
// 	Scene& currentScene = sManager.GetCurrentScene();
// 	GameCamera = currentScene.CurrentCamera;
// 	GameCamera->SetMovementDelegates(*GameController);

//   	{
//  		// Ground
//    	 	eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();
//    		centerObj->Move(glm::vec3(0.f, -2.f, 0.f));
//    		centerObj->SetScale(glm::vec3(100.f, 0.5f, 100.f));
//   
//   	}
}

 void TestGameMode::Tick(float inDeltaT)
 {
 	//GameController->ExecuteCallbacks();
	 triangle->Rotate(inDeltaT, glm::vec3(0.f, 0.f, 1.f));
 }

// void TestGameMode::MoveCameraLeft()
// {
// 	GameCamera->Move(MovementDirection::Left);
// 	//GameCamera->GetRelativeTransform().Rotate(5.f, glm::vec3(0.f, 1.f, 0.f));
// }
// 
// void TestGameMode::MoveCameraRight()
// {
// 	GameCamera->Move(MovementDirection::Right);
// 	//GameCamera->GetRelativeTransform().Rotate(-5.f, glm::vec3(0.f, 1.f, 0.f));
// }
// 
// void TestGameMode::MoveCameraUp()
// {
// 	GameCamera->Move(MovementDirection::Forward);
// }
// 
// void TestGameMode::MoveCameraDown()
// {
// 	GameCamera->Move(MovementDirection::Back);
// }
