#pragma once
#include "Core/GameModeBase.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/vector.h"

class FlappyGameMode : public GameModeBase
{
	using GameObjectPtr = eastl::shared_ptr<class IGameObject>;

public:
	FlappyGameMode();
	virtual ~FlappyGameMode();

public:
	virtual void Init() override;
	virtual void Tick(float inDelta) override;

	void Jump();
	void OnJumpStart();
	void OnJumpTick(float inDeltaT, float inTimeLeft);
	void OnJumpEnd();
	void GameOver();

	
private:
	eastl::shared_ptr<class Camera> GameCamera;
	eastl::unique_ptr<class Controller> GameController;


	// Game logic variables
	bool IsJumping;
	bool GameRunning;
	eastl::shared_ptr<class IGameObject> BirdObject;
	eastl::vector<GameObjectPtr> Floors;

};