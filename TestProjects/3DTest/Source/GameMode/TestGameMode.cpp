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

	//GameCamera->Rotate(-45.f, glm::vec3(0.f, 1.f, 0.f));

//  	Ground = BasicShapes::CreateCubeObject();
//  	Ground->Move(glm::vec3(0.f, -10.f, 0.f));
//  	Ground->SetScale(glm::vec3(200.f, 1.f, 200.f));


 	{
 		eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();
 	}
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
//   	{
//   		eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();
//   		centerObj->Move(glm::vec3(0.f, 40.f, 0.f));
//   	}
//   	{
//   		eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();
//   		centerObj->Move(glm::vec3(0.f, 50.f, 0.f));
//   	}
//   	{
//   		eastl::shared_ptr<CubeShape> centerObj = BasicShapes::CreateCubeObject();
//   		centerObj->Move(glm::vec3(0.f, 60.f, 0.f));
//   	}


// 	eastl::shared_ptr<DrawableBase> drawableObject = BasicShapes::CreateCubeObject();
//  	Object = drawableObject;
// 	drawableObject->SetVisible(false);
// 
//  	{
//  		Obj = BasicShapes::CreateCubeObject();
//  		Obj->Move(glm::vec3(5.f, 0., 0.f));
//  		Object->AddChild(Obj);
//  
//  		Obj2 = BasicShapes::CreateCubeObject();
//  		Obj2->Move(glm::vec3(3.f, 0.f, 0.f));
//  		Obj->AddChild(Obj2);
//  		Obj->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
//  	}
//  	{
//  		EntityPtr SecondModel = BasicShapes::CreateCubeObject();
//  		SecondModel->Move(glm::vec3(0.f, 0.f, 20.f));
//  	}
// 
// 
// 	eastl::shared_ptr<DrawableBase> drawableYObject = BasicShapes::CreateCubeObject();
// 	YObject = drawableYObject;
// 	drawableYObject->SetVisible(false);
// 
// 	{
// 		YObj = BasicShapes::CreateCubeObject();
// 		YObj->Move(glm::vec3(5.f, 0., 0.f));
// 		YObject->AddChild(YObj);
// 
// 		YObj2 = BasicShapes::CreateCubeObject();
// 		YObj2->Move(glm::vec3(3.f, 0.f, 0.f));
// 		YObj->AddChild(YObj2);
// 		YObj->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
// 	}
// 	
// 
// 	eastl::shared_ptr<DrawableBase> drawableZObject = BasicShapes::CreateCubeObject();
// 	ZObject = drawableZObject;
// 	drawableZObject->SetVisible(false);
// 
// 	ZObject->Rotate(90.f, glm::vec3(0.f, 1.f, 0.f));
// 
// 	{
// 		ZObj = BasicShapes::CreateCubeObject();
// 		ZObj->Move(glm::vec3(5.f, 0., 0.f));
// 		ZObject->AddChild(ZObj);
// 
// 		ZObj2 = BasicShapes::CreateCubeObject();
// 		ZObj2->Move(glm::vec3(3.f, 0.f, 0.f));
// 		ZObj->AddChild(ZObj2);
// 		ZObj->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
// 	}

	for (int32_t i = 0; i < 5; ++i)
	{
		for (int32_t j = 0; j < 15; j++)
		{
			eastl::shared_ptr<AssimpModel3D> model = ObjectCreation::NewObject<AssimpModel3D>("../Data/Models/Backpack/scene.gltf");
			model->Move(glm::vec3(5.f * j, 5.f * i, 5.f));
			model->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
		}
	}




//  	eastl::shared_ptr<AssimpModel3D> shibaModel = eastl::make_shared<AssimpModel3D>("../Data/Models/Shiba/scene.gltf");
//  	shibaModel->Move(glm::vec3(0.f, 10.f, 5.f));
//  	shibaModel->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

}

void TestGameMode::Tick(float inDeltaT)
{
	GameController->ExecuteCallbacks();

// 	Object->Rotate(2.f, glm::vec3(0.f, 1.f, 0.f));
// 	Obj->Rotate(2.f, glm::vec3(0.f, 1.f, 0.f));
// 	Obj2->Rotate(8.f, glm::vec3(0.f, 1.f, 0.f));

// 	YObject->Rotate(2.f, glm::vec3(0.f, 0.f, 1.f));
// 	YObj->Rotate(2.f, glm::vec3(0.f, 1.f, 0.f));
// 	YObj2->Rotate(8.f, glm::vec3(0.f, 1.f, 0.f));
// 
// 	ZObject->Rotate(2.f, glm::vec3(0.f, 0.f, 1.f));
// 	ZObj->Rotate(2.f, glm::vec3(0.f, 1.f, 0.f));
// 	ZObj2->Rotate(8.f, glm::vec3(0.f, 1.f, 0.f));
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
