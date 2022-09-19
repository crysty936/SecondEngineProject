#include "OpenGLTexture.h"
#include "Utils/ImageLoading.h"
#include "GLFW/glfw3.h"
#include "OpenGLRenderer.h"
#include "OpenGLShader.h"
#include "Window/WindowsWindow.h"

// Create a texture buffer
OpenGLTexture::OpenGLTexture(const eastl::string& inTexName, const uint32_t inGlTexType)
	: TexName{ inTexName }, GLTexType{ inGlTexType }{}

OpenGLTexture::~OpenGLTexture()
{
	DeleteTexture();
}

void OpenGLTexture::Init(const eastl::string& inTexturePath)
{
	glGenTextures(1, &TexHandle);
	glBindTexture(GLTexType, TexHandle);

	const WindowProperties& windowProps = RHI->GetMainWindow().GetProperties();

	glTexImage2D(GLTexType, 0, GL_RGB, windowProps.Width, windowProps.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	ImageData data = ImageLoading::LoadImageData(inTexturePath.data());

	if (!data.RawData)
	{
		return;
	}

	NrChannels = data.NrChannels;

	glGenTextures(1, &TexHandle);

	glBindTexture(GLTexType, TexHandle);

	GLenum imageFormat = 0;
	switch (data.NrChannels)
	{
	case 1:
	{
		imageFormat = GL_RED;
		break;
	}
	case 3:
	{
		imageFormat = GL_RGB;
		break;
	}
	case 4:
	{
		imageFormat = GL_RGBA;
		break;
	}
	}

	glTexImage2D(GLTexType, 0, GL_RGBA8, data.Width, data.Height, 0, imageFormat, GL_UNSIGNED_BYTE, data.RawData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GLTexType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GLTexType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GLTexType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GLTexType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	ImageLoading::FreeImageData(data.RawData);

	glBindTexture(GLTexType, 0);
}

void OpenGLTexture::Bind(const uint32_t inTexNr) const
{
	// TODO: Make the textures be just uniforms that self register, with the others instead of binding the separately

// 	if (NrChannels == 4)
// 	{
// 		glEnable(GL_BLEND);
// 
// 	}

	ASSERT(TexHandle != 0);

	glActiveTexture(GL_TEXTURE0 + inTexNr);
	glBindTexture(GLTexType, TexHandle);
}

void OpenGLTexture::Unbind(const uint32_t inTexNr) const
{
	glActiveTexture(GL_TEXTURE0 + inTexNr);
	glBindTexture(GLTexType, 0);
// 
// 	if (NrChannels == 4)
// 	{
// 		glDisable(GL_BLEND);
// 	}
}

void OpenGLTexture::DeleteTexture()
{
	if (TexHandle != -1)
	{
		LOG_WARNING("Texture %s is being deleted, take care that this is intended.", TexName.c_str());
		glDeleteTextures(1, &TexHandle);
	}
}

bool OpenGLTexture::operator==(const OpenGLTexture& inOther)
{
	return inOther.TexHandle == this->TexHandle;
}
