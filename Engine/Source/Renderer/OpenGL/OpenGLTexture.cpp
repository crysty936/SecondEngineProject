#include "OpenGLTexture.h"
#include "Utils/ImageLoading.h"
#include "GLFW/glfw3.h"

OpenGLTexture::OpenGLTexture(const eastl::string& inTexturePath, int32_t inTexNr)
	: TexPath{inTexturePath}, TexNr{ inTexNr }
{
	Init(inTexturePath);
}

OpenGLTexture::OpenGLTexture() = default;
OpenGLTexture::~OpenGLTexture() = default;

void OpenGLTexture::Init(const eastl::string & inTexturePath)
{
	ImageData data = ImageLoading::LoadImageData(inTexturePath.data());

	if (!data.RawData)
	{
		return;
	}

	NrChannels = data.NrChannels;

	uint32_t handle;
	glGenTextures(1, &handle);

	glActiveTexture(TexNr);
	glBindTexture(GL_TEXTURE_2D, handle);

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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, data.Width, data.Height, 0, imageFormat, GL_UNSIGNED_BYTE, data.RawData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	TexHandle = handle;

	ImageLoading::FreeImageData(data.RawData);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture::Bind() const
{
// 	if (NrChannels == 4)
// 	{
// 		glEnable(GL_BLEND);
// 
// 	}

	ASSERT(TexNr != 0);

	glActiveTexture(TexNr);
	glBindTexture(GL_TEXTURE_2D, TexHandle);
}

void OpenGLTexture::Unbind() const
{
	glActiveTexture(TexNr);
	glBindTexture(GL_TEXTURE_2D, 0);
// 
// 	if (NrChannels == 4)
// 	{
// 		glDisable(GL_BLEND);
// 	}
}

void OpenGLTexture::DeleteTexture()
{
	glDeleteTextures(1, &TexHandle);
}
