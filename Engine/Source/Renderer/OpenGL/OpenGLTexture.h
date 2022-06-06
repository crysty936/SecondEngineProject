#pragma once
#include "EASTL/string.h"
#include <stdint.h>

enum class TextureType : uint8_t
{
	Diffuse,
	Specular
};

/**
 * Texture wrapper class for OpenGl functionality
 * Creates and initializes texture in constructor and deletes in destructor
 * Textures should always be moved unless purposely cached!
 */
class OpenGLTexture
{
public:
	OpenGLTexture();
	~OpenGLTexture();
	OpenGLTexture(const eastl::string& inTexturePath, int32_t inTexNr);
	OpenGLTexture(const OpenGLTexture& inOther) = default;
	OpenGLTexture(OpenGLTexture&& inOther);
	OpenGLTexture& operator=(const OpenGLTexture& inOther) = default;
	OpenGLTexture& operator=(OpenGLTexture&& inOther) = default;

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