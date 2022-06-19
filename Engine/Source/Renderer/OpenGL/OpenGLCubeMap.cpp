#include "OpenGLCubeMap.h"
#include "stb_image.h"
#include "Utils/ImageLoading.h"

OpenGLCubeMap::OpenGLCubeMap() = default;
OpenGLCubeMap::~OpenGLCubeMap() = default;

void OpenGLCubeMap::Bind() const
{
	ASSERT(TexNr == GL_TEXTURE0);
	ASSERT(TexHandle != 0);

	glActiveTexture(TexNr);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TexHandle);
}

void OpenGLCubeMap::Unbind() const
{
	glActiveTexture(TexNr);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void OpenGLCubeMap::Init(const eastl::vector<eastl::string>& inTexturePaths, const int32_t inTexNr)
{
	glGenTextures(1, &TexHandle);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TexHandle);

	for (int32_t i = 0; i < inTexturePaths.size(); ++i)
	{
		constexpr bool flipped = false;

		ImageData data = ImageLoading::LoadImageData(inTexturePaths[i].data(), flipped);

		if (!data.RawData)
		{
			return;
		}

		NrChannels = data.NrChannels;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, data.Width, data.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.RawData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

