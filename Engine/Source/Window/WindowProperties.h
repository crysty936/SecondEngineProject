#pragma once
#include "EASTL/string_view.h"

struct WindowProperties
{
	eastl::string_view Title = "MainWindow";
	int32_t Width = 1280;
	int32_t Height = 720;
	bool VSyncEnabled = false;
};