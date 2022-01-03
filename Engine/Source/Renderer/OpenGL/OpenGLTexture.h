#pragma once
#include "EASTL/string.h"
#include <stdint.h>

class OpenGLTexture
{
public:
	OpenGLTexture(const eastl::string& inTexturePath, int32_t inTextNr);
	~OpenGLTexture();

	void Bind();
	void Unbind();
	 
	void DeleteTexture();

private:
	uint32_t TexHandle;
	int32_t TexNr;
	bool Bound = false;
	bool Valid = false;
};