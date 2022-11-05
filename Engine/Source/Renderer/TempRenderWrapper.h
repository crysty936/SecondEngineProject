#pragma once
#include "EASTL/shared_ptr.h"

class TempRenderWrapper
{
public:
	static void Init();
	static void Terminate();
	static void Draw();
};