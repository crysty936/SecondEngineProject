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

const float FallSpeed = 2.f;
const float RiseSpeed = 4.f;

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
	KeyActionDelegate createFloorDelegate = KeyActionDelegate::CreateRaw(this, &FlappyGameMode::CreateFloor);
	KeyCode createFloorKey = KeyCode::C;
	OnKeyAction createFloorAction = { createFloorDelegate, createFloorKey };

	GameController->AddListener(createFloorAction);


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
	BirdParent->Children.push_back(BirdObject);

	currentScene.SceneObjects.push_back(BirdParent);

	Floors.push_back(eastl::make_shared<CeilingAndFloor>());
	Floors.push_back(eastl::make_shared<CeilingAndFloor>());

	for (int32_t i = 0; i < Floors.size(); i++)
	{
		const EntityPtr& floor = Floors[i];
		floor->Model.Translation.x -= 25.f * i;

		// Place the walls a bit ahead of the bird for the z test
		floor->Model.Translation.z += 1.f;
		currentScene.SceneObjects.push_back(floor);
	}

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
		float fallOffset = FallSpeed * inDeltaT;

		for (const EntityPtr& floor : Floors)
		{
			floor->Model.Translation.x += fallOffset;
		}

		BirdParent->Model.Translation.y -= fallOffset;

		float currentRotation = BirdObject->Model.Rotation.z;
		currentRotation -= 40.f * inDeltaT;
		currentRotation = glm::clamp(currentRotation, 0.f, 75.f);
		BirdObject->Model.Rotation.z = currentRotation;
	}

	GameCamera->Model.Translation.x = BirdParent->Model.Translation.x;

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

	for (const EntityPtr& floor : Floors)
	{
		floor->Model.Translation.x += riseOffset / 5.f;
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
		currentScene.SceneObjects.erase_first(*iter);
	}
}

void FlappyGameMode::CreateFloors()
{
	EntityIterator lastFloorPlusOne = Floors.end();
	EntityIterator lastFloorIter = --lastFloorPlusOne;
	EntityPtr lastFloor = *lastFloorIter;

	if (lastFloor->Model.Translation.x > 0.f)
	{
		CreateFloor();
	}
}

void FlappyGameMode::CreateFloor()
{
	EntityPtr newFloor = eastl::make_shared<CeilingAndFloor>();
	newFloor->Model.Translation.x -= 25.f;
	newFloor->Model.Translation.z += 1.f;

	SceneManager& sManager = SceneManager::Get();
	Scene& currentScene = sManager.GetCurrentScene();
	currentScene.SceneObjects.push_back(newFloor);
	Floors.push_back(newFloor);




}