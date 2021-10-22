#include "Core/Public/EngineCore.h"
#include <assert.h>

EngineCore Engine;

EngineCore::EngineCore()
{
	static bool engineExisting = false;
	assert(!engineExisting);

	engineExisting = true;
}

EngineCore::~EngineCore() = default;

void EngineCore::Init()
{

}

void EngineCore::Run()
{
	
	





}

