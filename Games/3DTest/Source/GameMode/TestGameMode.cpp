#include "TestGameMode.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/OpenGL/DrawableObject.h"
#include "Renderer/ShapesUtils/BasicShapes.h"
#include "Controller/Controller.h"
#include "Entity/TransformEntity.h"
#include "glm/common.hpp"
#include <stdlib.h>

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
	GameController = eastl::make_unique<class Controller>();

// 	KeyActionDelegate jumpDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::Jump);
// 	KeyCode jumpKey = KeyCode::LeftAlt;
// 	OnKeyAction jumpAction = { jumpDelegate, jumpKey, true };
// 
// 	GameController->AddListener(jumpAction);

	// Debug
// 	KeyActionDelegate createFloorDelegate = KeyActionDelegate::CreateRaw(this, &TestGameMode::SwitchFloors);
// 	KeyCode createFloorKey = KeyCode::C;
// 	OnKeyAction createFloorAction = { createFloorDelegate, createFloorKey, true };
// 
// 	GameController->AddListener(createFloorAction);
	// Debug

	// Scene setup
	SceneManager& sManager = SceneManager::Get();
	Scene& currentScene = sManager.GetCurrentScene();
	GameCamera = currentScene.CurrentCamera;

	// Push camera back a bit and orient it towards center
	GameCamera->Model.Translation.z = 50.f;
	GameCamera->Model.Rotation.z = -1.f;

	// Why 90 degrees and not 180
	//GameCamera->Model.Rotate(180.f, glm::vec3(0.f, 0.f, 1.f));

// 	BirdObject = BasicShapes::CreateSquareObject("../Data/Textures/flappy.png");
// 
// 	BirdParent = eastl::make_shared<TransformEntity>();
// 	BirdParent->AddChild(BirdObject);
	//currentScene.AddEntity(BirdParent);


}

void TestGameMode::Tick(float inDeltaT)
{


}

