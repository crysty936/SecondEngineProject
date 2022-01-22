#include "OpenGLTexture.h"
#include "Utils/ImageLoading.h"
#include "GLFW/glfw3.h"

OpenGLTexture::OpenGLTexture(const eastl::string& inTexturePath, int32_t inTexNr)
	: TexHandle{ 0 }, TexNr{ inTexNr }, NrChannels{ 0 }, Bound{ false }, Valid{ false }
{
	ImageData data = ImageLoading::LoadImageData(inTexturePath.data());

	if (!data.RawData)
	{
		return;
	}

	NrChannels = data.NrChannels;

	uint32_t handle;
	glGenTextures(1, &handle);

	glActiveTexture(inTexNr);
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

	Valid = true;
}

OpenGLTexture::~OpenGLTexture() = default;

void OpenGLTexture::Bind()
{
	if (NrChannels == 4)
	{
		glEnable(GL_BLEND);

	}

	glActiveTexture(TexNr);
	glBindTexture(GL_TEXTURE_2D, TexHandle);

	Bound = true;
}

void OpenGLTexture::Unbind()
{
	glActiveTexture(TexNr);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (NrChannels == 4)
	{
		glDisable(GL_BLEND);
	}

	Bound = false;
}

void OpenGLTexture::DeleteTexture()
{
	glDeleteTextures(1, &TexHandle);

	Valid = false;
}
