#include "EntryPoint/EntryPoint.h"
#include "GameMode/FlappyGameMode.h"


int main(int inArgCount, char* inArgs)
{

	return EngineStart(inArgCount, inArgs, eastl::make_shared<FlappyGameMode>());
}