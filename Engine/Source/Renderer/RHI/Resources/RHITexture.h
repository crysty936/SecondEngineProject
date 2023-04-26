#pragma once
#include <stdint.h>
#include "EASTL/string.h"

enum class ETextureType
{
	Single, // 2D
	Array
};

enum class ERHITextureChannelsType
{
	RGBA,
	RGBAHDR,
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

	ETextureType TextureType = ETextureType::Single;
	ERHITextureChannelsType ChannelsType = ERHITextureChannelsType::RGBA;
};