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
	BirdObject{ nullptr },
	GameController{},
	IsJumping{false}
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

	BirdObject = BasicShapes::CreateSquareObject();

	currentScene.SceneObjects.push_back(BirdObject);

	currentScene.SceneObjects.push_back(BasicShapes::CreateTriangleObject());


	currentScene.SceneObjects.push_back(eastl::make_shared<CeilingAndFloor>());
}

void FlappyGameMode::Tick(float inDelta)
{
	GameController->ExecuteCallbacks();

	if (!IsJumping)
	{
		float fallOffset = FallSpeed * inDelta;

		BirdObject->Model.Translation.x -= fallOffset;
		BirdObject->Model.Translation.y -= fallOffset;
	}

	GameCamera->Model.Translation.x = BirdObject->Model.Translation.x;





}

void FlappyGameMode::Jump()
{
	constexpr float timerTime = 1.f;
	LogicDelegate del = LogicDelegate::CreateRaw(this, &FlappyGameMode::OnJumpTick);

 	eastl::shared_ptr<LogicTimer> jumpTimer = eastl::make_shared<LogicTimer>(timerTime, del);
 	TimersManager::Get().AddTimer(jumpTimer);
}

void FlappyGameMode::OnJumpStart()
{
	IsJumping = true;
}

void FlappyGameMode::OnJumpTick(float inDeltaT, float inTimeLeft)
{
	float riseOffset = RiseSpeed * (inDeltaT * inTimeLeft);

	BirdObject->Model.Translation.x -= riseOffset / 5.f;
	BirdObject->Model.Translation.y += riseOffset;
}

void FlappyGameMode::OnJumpEnd()
{
	IsJumping = false;
}
