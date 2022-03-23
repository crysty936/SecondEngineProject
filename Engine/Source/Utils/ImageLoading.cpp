#include "ImageLoading.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Core/EngineUtils.h"

ImageData ImageLoading::LoadImageData(const char* inTexurePath)
{
	stbi_set_flip_vertically_on_load(true);

	unsigned char* rawData = nullptr;
	int32_t width, height, nrChannels;
	width, height, nrChannels = 0;

	rawData = stbi_load(inTexurePath, &width, &height, &nrChannels, 0);

	if (!ENSURE_MSG(rawData, "Image Loading Failed"))
	{
		return {};
	}

	ImageData data{ rawData, nrChannels, width, height };

	return data;
}

void ImageLoading::FreeImageData(unsigned char* inData)
{
	stbi_image_free(inData);
}
