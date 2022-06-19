#include "OpenGLDepthMap.h"
#include "stb_image.h"
#include "Utils/ImageLoading.h"

OpenGLDepthMap::OpenGLDepthMap() = default;
OpenGLDepthMap::~OpenGLDepthMap() = default;

constexpr uint32_t SHADOW_WIDTH = 1024;
constexpr uint32_t SHADOW_HEIGHT = 1024;

void OpenGLDepthMap::Init(const int32_t inTexNr)
{
	glGenTextures(1, &TexHandle);
	glBindTexture(GL_TEXTURE_2D, TexHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

