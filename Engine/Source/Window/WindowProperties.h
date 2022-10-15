#pragma once
#include "EASTL/string_view.h"

struct WindowProperties
{
	eastl::string_view Title = "MainWindow";
	int32_t Width = 800;
	int32_t Height = 800;
	bool VSyncEnabled = false;
};