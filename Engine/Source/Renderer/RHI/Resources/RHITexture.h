#pragma once
#include <stdint.h>
#include "EASTL/string.h"

enum class RHITextureType
{
	RGBA,
	Depth
};

class RHITexture2D
{
public:
	int32_t NrChannels = 0;
	int32_t Width = 0;
	int32_t Height = 0;

	// TODO: Debug only
	eastl::string SourcePath;

	RHITextureType TextureType = RHITextureType::RGBA;
};