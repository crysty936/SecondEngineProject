#pragma once
#include "Core/GameModeBase.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/vector.h"
#include "Entity/Entity.h"
#include "Controller/GameController.h"

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
	void DebugProjections();
	void DebugCascadesVisualize();
	void DebugCursorMode();
	void BoostCameraSpeed();

private:
	eastl::shared_ptr<class Camera> GameCamera = nullptr;
	eastl::unique_ptr<class GameController> Controller = nullptr;


	eastl::shared_ptr<class AssimpModel3D> AssimpModel;
	eastl::shared_ptr<class LightSource> DirLight;
	eastl::shared_ptr<class AssimpModel3D> FloorModel;
	eastl::shared_ptr<class AssimpModel3DEditorSphere> SphereModel;
	TransformObjPtr Quad;
	TransformObjPtr Billboard;
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