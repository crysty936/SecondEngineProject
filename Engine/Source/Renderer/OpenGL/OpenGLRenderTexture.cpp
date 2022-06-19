#include "OpenGLRenderTexture.h"
#include "stb_image.h"
#include "Utils/ImageLoading.h"
#include "OpenGLRenderer.h"
#include "Window/WindowProperties.h"

OpenGLRenderTexture::OpenGLRenderTexture() = default;
OpenGLRenderTexture::~OpenGLRenderTexture() = default;

void OpenGLRenderTexture::Init(const int32_t inTexNr)
{
	TexNr = GL_TEXTURE0;
	glGenTextures(1, &TexHandle);
	glBindTexture(GL_TEXTURE_2D, TexHandle);

	const WindowProperties& windowProps = RHI->GetMainWindow().GetProperties();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowProps.Width, windowProps.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glBindTexture(GL_TEXTURE_2D, 0);
}

