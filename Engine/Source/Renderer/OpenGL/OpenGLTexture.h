#pragma once
#include "EASTL/string.h"
#include <stdint.h>

enum class TextureType
{
	Diffuse,
	Specular
};

class OpenGLTexture
{
public:
	OpenGLTexture(const eastl::string& inTexturePath, int32_t inTexNr);
	OpenGLTexture();
	~OpenGLTexture();

	void Bind() const;
	void Unbind() const;
	 
	void DeleteTexture();

public:
	eastl::string TexPath{};
	uint32_t TexHandle{};
	int32_t TexNr{};
	int32_t NrChannels{};
	TextureType TexType{TextureType::Diffuse};
};