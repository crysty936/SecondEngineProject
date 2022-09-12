#pragma once
#include "Core/GameModeBase.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/vector.h"
#include "Entity/Entity.h"

class TestGameMode : public GameModeBase
{
public:
	TestGameMode();
	virtual ~TestGameMode();

public:
	virtual void Init() override;
	virtual void Tick(float inDeltat) override;

// 	void MoveCameraLeft();
// 	void MoveCameraRight();
// 	void MoveCameraUp();
// 	void MoveCameraDown();
// 	void OnChangeDrawMode();
private:
	eastl::shared_ptr<class Camera> GameCamera = nullptr;
	eastl::unique_ptr<class Controller> GameController = nullptr;

};