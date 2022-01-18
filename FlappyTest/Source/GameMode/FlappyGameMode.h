#pragma once
#include "Core/GameModeBase.h"

class FlappyGameMode : public GameModeBase
{

public:
	virtual void Init() override;

	virtual void Tick(float inDelta) override;

};