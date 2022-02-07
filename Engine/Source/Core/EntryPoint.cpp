#include "GameModeBase.h"
#include "EngineCore.h"

int main(int argc, char** argv)
{
	EngineCore::Init();
	Engine->Run();
}