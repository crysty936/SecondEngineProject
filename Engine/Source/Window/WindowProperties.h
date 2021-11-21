#pragma once
#include "EASTL/string_view.h"

struct WindowProperties
{
	eastl::string_view Title = "MainWindow";
	uint32_t Width = 1280;
	uint32_t Height = 720;
};