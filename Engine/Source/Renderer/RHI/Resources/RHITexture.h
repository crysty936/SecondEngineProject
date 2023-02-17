#pragma once
#include <stdint.h>
#include "EASTL/string.h"

enum class ETextureType
{
	Default, // 2D
	Array
};

enum class ERHITextureChannelsType
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

	ETextureType TextureType = ETextureType::Default;
	ERHITextureChannelsType ChannelsType = ERHITextureChannelsType::RGBA;
};