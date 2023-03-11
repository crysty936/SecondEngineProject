#pragma once
#include "EASTL/vector.h"
#include "EASTL/shared_ptr.h"

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

public:
	static void Init();
	static void Terminate();
	void Run();
	void CheckShouldCloseWindow();
	bool IsRunning();
	void StopEngine();

	class WindowsWindow& GetMainWindow() { return *MainWindow; }

private:
	float CurrentDeltaT;

	class GameModeBase* CurrentGameMode = nullptr;
	// TODO 
	// Engine core holds ownership over Window for now, it should be moved to application layer later
	eastl::unique_ptr<class WindowsWindow> MainWindow = nullptr;
};

inline EngineCore* Engine = nullptr;
