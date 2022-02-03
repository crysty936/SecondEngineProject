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

private:
	eastl::shared_ptr<class Camera> GameCamera;
	eastl::unique_ptr<class Controller> GameController;


	EntityPtr Object;
};