#pragma once
#include "Core/GameModeBase.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/vector.h"
#include "Entity/Entity.h"

class FlappyGameMode : public GameModeBase
{
public:
	FlappyGameMode();
	virtual ~FlappyGameMode();

public:
	virtual void Init() override;
	virtual void Tick(float inDeltat) override;

private:
	void Jump();
	void OnJumpStart();
	void OnJumpTick(float inDeltaT, float inTimeLeft);
	void OnJumpEnd();
	void GameOver();
	eastl::shared_ptr<class CeilingAndFloor> CreateFloor();
	void DebugMoveBird();
	void CheckFloors();
	void SwitchFloors();
	void DebugMoveCameraLeft();
	void DebugMoveCameraRight();

	void CheckPillarsCollision();
private:
	eastl::shared_ptr<class Camera> GameCamera;
	eastl::unique_ptr<class Controller> GameController;

	// Game logic variables
	bool IsJumping;
	bool GameRunning;
	EntityPtr BirdObject;
	EntityPtr BirdParent;
	eastl::shared_ptr<class CeilingAndFloor> Floors[2];
};