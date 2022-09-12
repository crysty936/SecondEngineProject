#pragma once
#include "EASTL/string.h"
#include <stdint.h>
#include "glad/glad.h"

enum class TextureType : uint8_t
{
	Diffuse,
	Specular
};

/**
 * Texture wrapper class for OpenGl functionality
 * Creates and initializes texture in constructor and deletes in destructor
 * Textures should always be moved unless intentionally cached!
 */
class OpenGLTexture
{
public:
	OpenGLTexture(const eastl::string& inTexName, const uint32_t inGlTexType = GL_TEXTURE_2D);
	virtual ~OpenGLTexture();
	OpenGLTexture(const OpenGLTexture& inOther) = default;
	OpenGLTexture(OpenGLTexture&& inOther);
	OpenGLTexture& operator=(const OpenGLTexture& inOther) = default;
	OpenGLTexture& operator=(OpenGLTexture&& inOther) = default;

	// Lazy initialization
	void Init(const eastl::string& inTexturePath);

	void Bind(const uint32_t inTexNr) const;
	void Unbind(const uint32_t inTexNr) const;
	 
	void DeleteTexture();

	bool operator==(const OpenGLTexture& inOther);

public:
	uint32_t GLTexType = GL_TEXTURE_2D;
	eastl::string TexName;
	eastl::string TexPath;
	uint32_t TexHandle = 0;
	int32_t NrChannels = 0;
	TextureType TexType{TextureType::Diffuse};
};