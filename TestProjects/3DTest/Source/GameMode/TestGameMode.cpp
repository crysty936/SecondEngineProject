#include "TestGameMode.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapes.h"
#include "Controller/Controller.h"
#include "glm/common.hpp"
#include <stdlib.h>
#include "Renderer/Model/3D/Assimp/AssimpModel3D.h"
#include "Core/ObjectCreation.h"
#include "Renderer/Drawable/MirrorQuad.h"

TestGameMode GameMode = {};

TestGameMode::TestGameMode()
	: GameModeBase()
{
}

TestGameMode::~TestGameMode() = default;

void TestGameMode::Init()
{
	GameController = eastl::make_unique<Controller>();

	// Debug
	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraLeft);
		EInputKey createFloorKey = EInputKey::A;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };
		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraRight);
		EInputKey createFloorKey = EInputKey::D;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };

		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraUp);
		EInputKey createFloorKey = EInputKey::W;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };
		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::MoveCameraDown);
		EInputKey createFloorKey = EInputKey::S;
		OnKeyAction createFloorAction = { cameraMovementDelegate, createFloorKey, false };

		GameController->AddListener(createFloorAction);
	}

	{
		KeyActionDelegate cameraMovementDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::OnChangeDrawMode);
		EInputKey createFloorKey = EInputKey::F;
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

 	{
		// Ground
   	 	eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();
   		centerObj->Move(glm::vec3(0.f, -2.f, 0.f));
   		centerObj->SetScale(glm::vec3(100.f, 0.5f, 100.f));
 
 	}
	{
		//eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();

		eastl::shared_ptr<SquareShape> centerObj = BasicShapes::CreateSquareObject();

		//eastl::shared_ptr<CubeShape> lightObj = BasicShapes::CreateCubeObject();
		//lightObj->SetRelativeLocation(glm::vec3(10.f, 20.f, 15.f));
	}
// 	{
// 		eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();
// 		centerObj->Move(glm::vec3(0.f, 20.f, 0.f));
// 	}
 	{
// 		for (int32_t j = 0; j < 100; ++j)
// 		{
// 			for (int32_t i = 0; i < 100; ++i)
// 			{
//  				eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();
//  				centerObj->Move(glm::vec3(j * 3.f, 5.f, i * 3.f));
// 				//centerObj->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
// 			}
// 		}
 	}

	//eastl::shared_ptr<AssimpModel3D> shibaModel = ObjectCreation::NewObject<AssimpModel3D>("../Data/Models/Shiba/scene.gltf");
		
	// Really slow because no batching or camera culling or lods
	//for (int32_t i = 0; i < 5; ++i)
	//{
	//	for (int32_t j = 0; j < 15; j++)
	//	{
	//		eastl::shared_ptr<AssimpModel3D> model = ObjectCreation::NewObject<AssimpModel3D>("../Data/Models/Backpack/scene.gltf");
	//		model->Move(glm::vec3(5.f * j, 5.f * i, 5.f));
	//		model->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
	//	}
	//}

//  	{
//  		eastl::shared_ptr<MirrorQuad> test = ObjectCreation::NewObject<MirrorQuad>();
//  
//  		test->Move(glm::vec3(0.0f, 0.0f, -10.f));
//  	}

//    	eastl::shared_ptr<AssimpModel3D> model = ObjectCreation::NewObject<AssimpModel3D>("../Data/Models/Shiba/scene.gltf");
//    	model->Move(glm::vec3(0.f, 0.f, -5.f));
   	//model->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));

	//eastl::shared_ptr<Skybox> skybox = ObjectCreation::NewObject<Skybox>();
// 
//  	{
//  		eastl::shared_ptr<LightSource> lightSource = ObjectCreation::NewObject<LightSource>();
//  		//lightSource->Move(glm::vec3(0.f, 15.f, 0.f));
//  		lightSource->SetRelativeLocation({ -2.0f, 20.0f, -1.0f });
// 		lightSource->Rotate(20.f, glm::vec3(0.f, 1.f, 0.f));
//  	}
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

	//RHI->SetDrawMode(drawMode ? EDrawMode::NORMAL : EDrawMode::DEPTH);

	drawMode = !drawMode;
}
