#include "Core/EngineCore.h"
#include "EASTL/shared_ptr.h"


int EngineStart(int inArgCount, char* inArgs, eastl::shared_ptr<GameModeBase> inGameMode)
{
	EngineCore::Init(inGameMode);

	Engine->Run();

	return 0;
}