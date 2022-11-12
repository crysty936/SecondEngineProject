#pragma once
#include <stdint.h>
#include "EASTL/string.h"

class RHITexture2D
{
public:
	int32_t NrChannels = 0;

	// TODO: Debug only
	eastl::string SourcePath;
};