#pragma once
#include "EASTL/string_view.h"

struct WindowProperties
{
	eastl::string_view Title = "MainWindow";
	int32_t Width = 1920;
	int32_t Height = 1080;
	bool VSyncEnabled = false;
};