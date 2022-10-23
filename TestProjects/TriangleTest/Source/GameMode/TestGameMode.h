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
	TransformObjPtr triangle;
	TransformObjPtr triangle2;
};