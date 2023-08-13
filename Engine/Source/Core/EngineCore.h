#pragma once
#include "EASTL/vector.h"
#include "EASTL/shared_ptr.h"
#include "EventSystem/EventSystem.h"

using PostInitCallback = MulticastDelegate<>;

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
	inline PostInitCallback& GetPostInitMulticast() { return InitDoneMulticast; }

private:
	PostInitCallback InitDoneMulticast;
	float CurrentDeltaT;

	class GameModeBase* CurrentGameMode = nullptr;
	// TODO 
	// Engine core holds ownership over Window for now, it should be moved to application layer later
	eastl::unique_ptr<class WindowsWindow> MainWindow = nullptr;
};

extern EngineCore* GEngine;
extern uint64_t GFrameCounter;

void AddInternalPlugin(class IInternalPlugin* inNewPlugin);

template<class T>
class AddPluginHelper
{
public:
	AddPluginHelper()
	{
		AddInternalPlugin(new T());
	}

	~AddPluginHelper() = default;
};


