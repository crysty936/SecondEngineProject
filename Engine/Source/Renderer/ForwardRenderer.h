#pragma once
#include "EASTL/shared_ptr.h"

class ForwardRenderer
{
public:
	static void Init();
	static void Terminate();
	void Draw();

	inline static ForwardRenderer* Instance = nullptr;
};