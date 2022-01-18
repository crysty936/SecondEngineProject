#pragma once


class GameModeBase
{
public:
	GameModeBase();
	virtual ~GameModeBase();

public:
	// Scene setup (loading as well until specific implementation)
	virtual void Init();

	// Game Mode Tick is Run after all other objects
	virtual void Tick(float inDelta);
};