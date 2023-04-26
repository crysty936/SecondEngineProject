#include "OpenGLCubeMap.h"
#include "stb_image.h"
#include "Utils/ImageLoading.h"
#include "glad/glad.h"

OpenGLCubeMap::OpenGLCubeMap(const eastl::string& inTexName) 
	/*: OpenGLTexture(inTexName, GL_TEXTURE_CUBE_MAP)*/ {}

OpenGLCubeMap::~OpenGLCubeMap() = default;

void OpenGLCubeMap::Init(const eastl::vector<eastl::string>& inTexturePaths, int32_t inTexNr)
{
// 	if (inTexNr == 0)
// 	{
// 		inTexNr = GL_TEXTURE0;
// 	}
// 
// 	glGenTextures(1, &TexHandle);
// 	glBindTexture(GLTexType, TexHandle);
// 
// 	for (int32_t i = 0; i < inTexturePaths.size(); ++i)
// 	{
// 		constexpr bool flipped = false;
// 
// 		ImageData data = ImageLoading::LoadImageData(inTexturePaths[i].data(), flipped);
// 
// 		if (!data.RawData)
// 		{
// 			return;
// 		}
// 
// 		NrChannels = data.NrChannels;
// 
// 		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, data.Width, data.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.RawData);
// 	}
// 
// 	glTexParameteri(GLTexType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 	glTexParameteri(GLTexType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 	glTexParameteri(GLTexType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// 	glTexParameteri(GLTexType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
// 	glTexParameteri(GLTexType, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
// 
// 	glBindTexture(GLTexType, 0);
}

