#include "Core/EngineCore.h"


int EngineStart(int inArgCount, char* inArgs)
{
	EngineCore::Init();

	Engine->Run();

	return 0;
}