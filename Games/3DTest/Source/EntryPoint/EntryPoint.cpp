#include "EntryPoint/EntryPoint.h"
#include "GameMode/TestGameMode.h"


int main(int inArgCount, char* inArgs)
{

	return EngineStart(inArgCount, inArgs, eastl::make_shared<TestGameMode>());
}