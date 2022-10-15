#pragma once
#include "EASTL/vector.h"
#include "EASTL/shared_ptr.h"

#define WITH_GLFW 0

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

public:
	static void Init();
	static void Terminate();
	void Run();
	bool IsRunning();

private:
	float CurrentDeltaT;

	// TODO 
	// Engine holds ownership over GameMode for now, it should be abstracted into Scene later on so that
	// each scene can have its own GameMode
	class GameModeBase* CurrentGameMode = nullptr;
};

void StopEngine();

inline EngineCore* Engine = nullptr;
