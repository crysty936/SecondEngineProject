#pragma once
#include "Core/GameModeBase.h"
#include "EASTL/shared_ptr.h"

class FlappyGameMode : public GameModeBase
{
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

	
private:
	// Game Objects
	eastl::shared_ptr<class Camera> GameCamera;
	eastl::shared_ptr<class IGameObject> BirdObject;
	eastl::unique_ptr<class Controller> GameController;

	// Game logic variables
	bool IsJumping;

};