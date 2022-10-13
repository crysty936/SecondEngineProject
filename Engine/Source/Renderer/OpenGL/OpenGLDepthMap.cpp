#include "OpenGLDepthMap.h"
#include "stb_image.h"
#include "Utils/ImageLoading.h"
#include "OpenGLRenderer.h"
#include "glad/glad.h"

OpenGLDepthMap::OpenGLDepthMap(const eastl::string& inTexName) 
	: OpenGLTexture(inTexName, GL_TEXTURE_2D) {}

OpenGLDepthMap::~OpenGLDepthMap() = default;

void OpenGLDepthMap::Init()
{
	glGenTextures(1, &TexHandle);
	glBindTexture(GLTexType, TexHandle);

	glTexImage2D(GLTexType, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GLTexType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GLTexType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindTexture(GLTexType, 0);
}

