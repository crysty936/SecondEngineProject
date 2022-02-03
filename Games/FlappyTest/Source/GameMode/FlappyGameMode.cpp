#include "FlappyGameMode.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Renderer/OpenGL/DrawableObject.h"
#include "Renderer/ShapesUtils/BasicShapes.h"
#include "Controller/Controller.h"
#include "Timer/LogicTimer.h"
#include "Timer/TimersManager.h"
#include "Objects/CeilingAndFloor.h"
#include "Entity/TransformEntity.h"
#include "glm/common.hpp"
#include <stdlib.h>

const float FallSpeed = 2.f;
const float RiseSpeed = 4.f;
float MoveSpeedNormal = 2.f;
float MoveSpeedFast = 2.f;

FlappyGameMode::FlappyGameMode()
	:
	GameModeBase(),
	GameCamera{ nullptr },
	GameController{},
	IsJumping{ false },
	GameRunning{ false },
	BirdObject{ nullptr }
{
}

FlappyGameMode::~FlappyGameMode() = default;

void FlappyGameMode::Init()
{
	GameController = eastl::make_unique<class Controller>();

	KeyActionDelegate jumpDelegate = KeyActionDelegate::CreateRaw(this, &FlappyGameMode::Jump);
	KeyCode jumpKey = KeyCode::LeftAlt;
	OnKeyAction jumpAction = { jumpDelegate, jumpKey, true };

	GameController->AddListener(jumpAction);

	// Debug
	KeyActionDelegate createFloorDelegate = KeyActionDelegate::CreateRaw(this, &FlappyGameMode::SwitchFloors);
	KeyCode createFloorKey = KeyCode::C;
	OnKeyAction createFloorAction = { createFloorDelegate, createFloorKey, true };

	GameController->AddListener(createFloorAction);

	KeyActionDelegate moveBirdDelegate = KeyActionDelegate::CreateRaw(this, &FlappyGameMode::DebugMoveBird);
	KeyCode moveBirdKey = KeyCode::F;
	OnKeyAction moveBirdAction = { moveBirdDelegate, moveBirdKey, true };

	GameController->AddListener(moveBirdAction);

	KeyActionDelegate moveCameraLeftDelegate = KeyActionDelegate::CreateRaw(this, &FlappyGameMode::DebugMoveCameraLeft);
	KeyCode moveCameraLeftKey = KeyCode::D;

	OnKeyAction moveCameraLeftAction = { moveCameraLeftDelegate, moveCameraLeftKey, false };

	GameController->AddListener(moveCameraLeftAction);

	KeyActionDelegate moveCameraRightDelegate = KeyActionDelegate::CreateRaw(this, &FlappyGameMode::DebugMoveCameraRight);
	KeyCode moveCameraRightKey = KeyCode::A;

	OnKeyAction moveCameraRightAction = { moveCameraRightDelegate, moveCameraRightKey, false };

	GameController->AddListener(moveCameraRightAction);
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

	BirdObject = BasicShapes::CreateSquareObject("../Data/Textures/flappy.png");
	EntityPtr triangle = BasicShapes::CreateTriangleObject();
	BirdObject->AddChild(triangle);
	triangle->Model.Translation.x += 10.f;
	//BirdObject->Model.Rotation.y = 180.f;

	BirdParent = eastl::make_shared<TransformEntity>();
	BirdParent->AddChild(BirdObject);

	currentScene.AddEntity(BirdParent);

	// Add the 2 floors to the pool
	Floors[0] = CreateFloor();
	Floors[1] = CreateFloor();

	const float floorWidth = CeilingAndFloor::GetWidth();
	Floors[1]->Model.Translation.x = -floorWidth;

	GameRunning = true;
}

void FlappyGameMode::Tick(float inDeltaT)
{
	if (!GameRunning)
	{
		return;
	}
	GameCamera->Model.Rotate(1.f, glm::vec3(0.f, 0.f, 1.f));

	GameController->ExecuteCallbacks();

	if (!IsJumping)
	{
		const float fallOffset = FallSpeed * inDeltaT;
		const float moveOffset = MoveSpeedNormal * inDeltaT;

 		for (const EntityPtr& floor : Floors)
 		{
 			floor->Model.Translation.x += moveOffset;
 		}

		BirdParent->Model.Translation.y -= fallOffset;

// 		float currentRotation = BirdObject->Model.Rotation.z;
// 		currentRotation -= 40.f * inDeltaT;
// 		currentRotation = glm::clamp(currentRotation, 0.f, 75.f);
// 		BirdObject->Model.Rotation.z = currentRotation;
	}

	const float birdHeight = BirdParent->Model.Translation.y;
	const float floorY = CeilingAndFloor::GetFloorY();
	const float ceilingY = CeilingAndFloor::GetCeilingY();

	if (birdHeight <= (floorY + 0.5f) || birdHeight >= (ceilingY - 0.5f))
	{
		GameOver();
	}

	CheckFloors();
	CheckPillarsCollision();
}

void FlappyGameMode::Jump()
{
	constexpr float timerTime = 1.f;
	TimerLogicDelegate del = TimerLogicDelegate::CreateRaw(this, &FlappyGameMode::OnJumpTick);

	eastl::shared_ptr<LogicTimer> jumpTimer = eastl::make_shared<LogicTimer>(timerTime, del);
	TimersManager::Get().AddTimer(jumpTimer);
}

void FlappyGameMode::OnJumpStart()
{
	IsJumping = true;
}

void FlappyGameMode::OnJumpTick(float inDeltaT, float inTimeLeft)
{
	if (!GameRunning)
	{
		return;
	}

	float riseOffset = RiseSpeed * (inDeltaT * inTimeLeft);
	float moveOffset = MoveSpeedFast * (inDeltaT * inTimeLeft);

 	for (const EntityPtr& floor : Floors)
 	{
 		floor->Model.Translation.x += moveOffset / 5.f;
 	}

	BirdParent->Model.Translation.y += riseOffset;

// 	float currentRotation = BirdObject->Model.Rotation.z;
// 	currentRotation += 100.f * inDeltaT * inTimeLeft * inTimeLeft * inTimeLeft;
// 	currentRotation = glm::clamp(currentRotation, 0.f, 75.f);
// 	BirdObject->Model.Rotation.z = currentRotation;
}

void FlappyGameMode::OnJumpEnd()
{
	IsJumping = false;
}

void FlappyGameMode::GameOver()
{
	//GameRunning = false;
}

void FlappyGameMode::DebugMoveBird()
{
	MoveSpeedNormal += 5.f;
}

void FlappyGameMode::CheckFloors()
{
 	// Check floors positions, teleport if necessary
 	constexpr float floorLimit = 50.f;
 
 	// Floor at 0 position should always be the right most
 	eastl::shared_ptr<CeilingAndFloor> rightFloor = Floors[0];
 	eastl::shared_ptr<CeilingAndFloor> leftFloor = Floors[1];
 
 	const float rightFloorX = rightFloor->GetLocation().x;
 
 	// If floor has moved enough to the right and is out of camera
 	if (rightFloorX > floorLimit)
 	{
 		SwitchFloors();
 	}
}

void FlappyGameMode::SwitchFloors()
{
	const float floorSize = CeilingAndFloor::GetWidth();

	eastl::shared_ptr<CeilingAndFloor> rightFloor = Floors[0];
	eastl::shared_ptr<CeilingAndFloor> leftFloor = Floors[1];

	const float leftFloorX = leftFloor->GetLocation().x;
	const float newFloorX = leftFloorX - floorSize;

	rightFloor->Model.Translation.x = newFloorX;

	// Switch floors
  	Floors[0] = leftFloor;
  	Floors[1] = rightFloor;
}

eastl::shared_ptr<CeilingAndFloor> FlappyGameMode::CreateFloor()
{
	eastl::shared_ptr<CeilingAndFloor> newFloor = eastl::make_shared<CeilingAndFloor>();
	newFloor->Model.Translation.z += 1.f;

	SceneManager& sManager = SceneManager::Get();
	sManager.GetCurrentScene().AddEntity(newFloor);

	return newFloor;
}

void FlappyGameMode::DebugMoveCameraLeft()
{
	GameCamera->Model.Translation.x += 10.f;
}

void FlappyGameMode::DebugMoveCameraRight()
{
	GameCamera->Model.Translation.x -= 10.f;
}

void FlappyGameMode::CheckPillarsCollision()
{
	eastl::vector<EntityPtr> pillars;

	for (const EntityPtr& floor : Floors)
	{
		//pillars.insert(pillars.end(), floor->GetChildren().begin(), floor->GetChildren().end());
	}


	for (const EntityPtr& pillar : pillars)
	{

		//if()



	}

}

