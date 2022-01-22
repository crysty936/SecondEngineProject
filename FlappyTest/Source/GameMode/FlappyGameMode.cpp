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
#include "Objects/Pillar.h"
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
	OnKeyAction jumpAction = { jumpDelegate, jumpKey };

	GameController->AddListener(jumpAction);

	// Debug
	KeyActionDelegate createFloorDelegate = KeyActionDelegate::CreateRaw(this, &FlappyGameMode::DebugCreateFloor);
	KeyCode createFloorKey = KeyCode::C;
	OnKeyAction createFloorAction = { createFloorDelegate, createFloorKey };

	GameController->AddListener(createFloorAction);

	KeyActionDelegate moveBirdDelegate = KeyActionDelegate::CreateRaw(this, &FlappyGameMode::DebugMoveBird);
	KeyCode moveBirdKey = KeyCode::F;
	OnKeyAction moveBirdAction = { moveBirdDelegate, moveBirdKey };

	GameController->AddListener(moveBirdAction);

	KeyActionDelegate moveCameraLeftDelegate = KeyActionDelegate::CreateRaw(this, &FlappyGameMode::DebugMoveCameraLeft);
	KeyCode moveCameraLeftKey = KeyCode::A;

	OnKeyAction moveCameraLeftAction = { moveCameraLeftDelegate, moveCameraLeftKey };

	GameController->AddListener(moveCameraLeftAction);

	KeyActionDelegate moveCameraRightDelegate = KeyActionDelegate::CreateRaw(this, &FlappyGameMode::DebugMoveCameraRight);
	KeyCode moveCameraRightKey = KeyCode::D;

	OnKeyAction moveCameraRightAction = { moveCameraRightDelegate, moveCameraRightKey };

	GameController->AddListener(moveCameraRightAction);
	//

	// Scene setup
	SceneManager& sManager = SceneManager::Get();
	Scene& currentScene = sManager.GetCurrentScene();
	GameCamera = currentScene.CurrentCamera;

	// Push camera back a bit and orient it towards center
	GameCamera->Model.Translation.z = 50.f;
	GameCamera->Model.Rotation.z = -1.f;

	BirdObject = BasicShapes::CreateSquareObject("../Data/Textures/flappy.png");
	BirdObject->Model.Rotation.y = 180.f;

	BirdParent = eastl::make_shared<TransformEntity>();
	BirdParent->AddChild(BirdObject);

	currentScene.AddEntity(BirdParent);

	constexpr bool firstTime = true;
	CreateFloor(firstTime);

	GameRunning = true;
}

void FlappyGameMode::Tick(float inDeltaT)
{
	if (!GameRunning)
	{
		return;
	}

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

		float currentRotation = BirdObject->Model.Rotation.z;
		currentRotation -= 40.f * inDeltaT;
		currentRotation = glm::clamp(currentRotation, 0.f, 75.f);
		BirdObject->Model.Rotation.z = currentRotation;
	}

	const float birdHeight = BirdParent->Model.Translation.y;
	const float floorY = CeilingAndFloor::GetFloorY();
	const float ceilingY = CeilingAndFloor::GetCeilingY();

	if (birdHeight <= (floorY + 0.5f) || birdHeight >= (ceilingY - 0.5f))
	{
		GameOver();
	}

	CreateFloors();
	EraseFloors();
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

	float currentRotation = BirdObject->Model.Rotation.z;
	currentRotation += 100.f * inDeltaT * inTimeLeft * inTimeLeft * inTimeLeft;
	currentRotation = glm::clamp(currentRotation, 0.f, 75.f);
	BirdObject->Model.Rotation.z = currentRotation;
}

void FlappyGameMode::OnJumpEnd()
{
	IsJumping = false;
}

void FlappyGameMode::GameOver()
{
	//GameRunning = false;
}

void FlappyGameMode::EraseFloors()
{
	eastl::vector<EntityIterator> toRemove;

	for (EntityIterator iter = Floors.begin(); iter != Floors.end(); ++iter)
	{
		EntityPtr floor = *iter;

		if (floor->Model.Translation.x > 50.f)
		{
			toRemove.push_back(iter);
		}
	}

	for (EntityIterator iter : toRemove)
	{
		Floors.erase(iter);
		SceneManager& sManager = SceneManager::Get();
		Scene& currentScene = sManager.GetCurrentScene();

		EntityIterator sceneIter = eastl::find(currentScene.Entities.begin(), currentScene.Entities.end(), *iter);
		ENSURE(sceneIter);

		currentScene.Entities.erase(sceneIter);
	}
}

void FlappyGameMode::CreateFloors()
{
	EntityIterator lastFloorPlusOne = Floors.end();
	EntityIterator lastFloorIter = --lastFloorPlusOne;
	EntityPtr lastFloor = *lastFloorIter;

	if (lastFloor->Model.Translation.x > 10.f)
	{
		CreateFloor();
	}
}

void FlappyGameMode::DebugCreateFloor()
{
	constexpr bool firstTime = false;
	CreateFloor(firstTime);
}

void FlappyGameMode::DebugMoveBird()
{
	MoveSpeedNormal += 5.f;

}

void FlappyGameMode::CreateFloor(bool firstTime)
{
	const float floorOffset = firstTime ? 0.f : 25.f;

	EntityPtr newFloor = eastl::make_shared<CeilingAndFloor>();
	newFloor->Model.Translation.x -= floorOffset;
	newFloor->Model.Translation.z += 1.f;

	SceneManager& sManager = SceneManager::Get();
	sManager.GetCurrentScene().AddEntity(newFloor);
	Floors.push_back(newFloor);

	constexpr float minHeight = 5.f;
	constexpr float maxHeight = 10.f;

	const float height = static_cast<float>(glm::max(5, (rand() % 10)));
	const float distance = 5.f;
	const float distanceFromStart = -20.f;

	for (int32_t i = 0; i < 3; ++i)
	{
		EntityPtr newPillar = eastl::make_shared<Pillar>();



		newPillar->Model.Translation.x = newFloor->Model.Translation.x - distance *(i + 1);

		newFloor->AddChild(newPillar);
	}
}

void FlappyGameMode::DebugMoveCameraLeft()
{
	GameCamera->Model.Translation.x += 10.f;
}

void FlappyGameMode::DebugMoveCameraRight()
{
	GameCamera->Model.Translation.x -= 10.f;
}