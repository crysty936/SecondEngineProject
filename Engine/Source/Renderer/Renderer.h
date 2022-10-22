#pragma once
#include "EASTL/shared_ptr.h"

class Renderer
{
public:
	static void Init();
	static void Terminate();
	void Draw();
	class WindowsWindow& GetMainWindow();



	inline static Renderer* RHI = nullptr;
};