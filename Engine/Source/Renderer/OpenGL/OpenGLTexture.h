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
 * Textures should always be moved unless purposely cached!
 */
class OpenGLTexture
{
public:
	OpenGLTexture();
	virtual ~OpenGLTexture();
	OpenGLTexture(const OpenGLTexture& inOther) = default;
	OpenGLTexture(OpenGLTexture&& inOther);
	OpenGLTexture& operator=(const OpenGLTexture& inOther) = default;
	OpenGLTexture& operator=(OpenGLTexture&& inOther) = default;

	// Lazy initialization
	void Init(const eastl::string& inTexturePath, const int32_t inTexNr = GL_TEXTURE0);

	virtual void Bind() const;
	virtual void Unbind() const;
	 
	void DeleteTexture();

	bool operator==(const OpenGLTexture& inOther);

public:
	eastl::string TexPath{};
	uint32_t TexHandle{0};
	int32_t TexNr{0};
	int32_t NrChannels{0};
	TextureType TexType{TextureType::Diffuse};
};