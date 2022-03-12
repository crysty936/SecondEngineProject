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

	void MoveCameraLeft();
	void MoveCameraRight();
	void MoveCameraUp();
	void MoveCameraDown();
	void OnChangeDrawMode();
private:
	eastl::shared_ptr<class Camera> GameCamera;
	eastl::unique_ptr<class Controller> GameController;


	EntityPtr Object;
	EntityPtr YObject;
	EntityPtr ZObject;
	EntityPtr Obj;
	EntityPtr YObj;
	EntityPtr ZObj;
	EntityPtr Obj2;
	EntityPtr YObj2;
	EntityPtr ZObj2;
	eastl::shared_ptr<class CubeShape> Ground;
};