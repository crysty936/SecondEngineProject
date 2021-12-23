#include "Core/EngineCore.h"


int main(int inArgCount, char* inArgs)
{
	EngineCore::Init();

	Engine->Run();


	return 0;
}