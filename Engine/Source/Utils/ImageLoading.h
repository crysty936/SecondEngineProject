#pragma once

#include <stdint.h>
#include "glad/glad.h"
#include "Core/EngineUtils.h"

/**
 * Wrapper for stb_image
 */

struct ImageData
{
	unsigned char* RawData = nullptr;
	int32_t NrChannels = 0;
	int32_t Width = 0;
	int32_t Height = 0;
};

namespace ImageLoading
{
	ImageData LoadImageData(const char* inTexurePath);
	void FreeImageData(unsigned char* inData);
}

