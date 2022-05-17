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

	// Lazy initialization
	void Init(const eastl::string& inTexturePath);

	void Bind() const;
	void Unbind() const;
	 
	void DeleteTexture();

	bool operator==(const OpenGLTexture& inOther);

public:
	eastl::string TexPath{};
	uint32_t TexHandle{0};
	int32_t TexNr{0};
	int32_t NrChannels{0};
	TextureType TexType{TextureType::Diffuse};
};