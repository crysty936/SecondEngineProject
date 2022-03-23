#include "Core/GameModeBase.h"
#include "EngineUtils.h"

GameModeBase* GameModeBase::GameMode = nullptr;

GameModeBase::GameModeBase()
{
	ASSERT_MSG(!GameMode);

	GameMode = this;
}

GameModeBase::~GameModeBase()
= default;

void GameModeBase::Init()
{

}

void GameModeBase::Tick(float inDeltaT)
{

}
