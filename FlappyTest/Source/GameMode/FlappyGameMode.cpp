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

const float FallSpeed = 2.f;
const float RiseSpeed = 4.f;

FlappyGameMode::FlappyGameMode()
	: 
	GameModeBase(),
	GameCamera{ nullptr },
	GameController{},
	IsJumping{false},
	GameRunning{false},
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

	// Scene setup
	SceneManager& sManager = SceneManager::Get();
	Scene& currentScene = sManager.GetCurrentScene();
	GameCamera = currentScene.CurrentCamera;

	// Push camera back a bit and orient it towards center
	GameCamera->Model.Translation.z = 15.f;
	GameCamera->Model.Rotation.z = -1.f;

	BirdObject = BasicShapes::CreateSquareObject("../Data/Textures/flappy.png");
	BirdObject->Model.Rotation.y = 180.f;

	currentScene.SceneObjects.push_back(BirdObject);

	Floors.push_back(eastl::make_shared<CeilingAndFloor>());
	Floors.push_back(eastl::make_shared<CeilingAndFloor>());

	for (int32_t i = 0; i < Floors.size(); i++)
	{
		const GameObjectPtr& floor = Floors[i];
		floor->Model.Translation.x -= 15.f * i;

		// Place the walls a bit ahead of the bird for the z test
		floor->Model.Translation.z += 1.f;
		currentScene.SceneObjects.push_back(floor);
	}

	GameRunning = true;
}

void FlappyGameMode::Tick(float inDelta)
{
	if (!GameRunning)
	{
		return;
	}

	GameController->ExecuteCallbacks();

	if (!IsJumping)
	{
		float fallOffset = FallSpeed * inDelta;

		for (const GameObjectPtr& floor : Floors)
		{
			floor->Model.Translation.x += fallOffset;
		}

		BirdObject->Model.Translation.y -= fallOffset;
	}

	GameCamera->Model.Translation.x = BirdObject->Model.Translation.x;

	const float birdHeight = BirdObject->Model.Translation.y;
	const float floorY = CeilingAndFloor::GetFloorY();
	const float ceilingY = CeilingAndFloor::GetCeilingY();

	if (birdHeight <= (floorY + 1.f) || birdHeight >= (ceilingY - 1.f))
	{
		GameOver();
	}
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

	for (const GameObjectPtr& floor : Floors)
	{
		floor->Model.Translation.x += riseOffset / 5.f;
	}

	BirdObject->Model.Translation.y += riseOffset;
}

void FlappyGameMode::OnJumpEnd()
{
	IsJumping = false;
}

void FlappyGameMode::GameOver()
{
	GameRunning = false;
}
