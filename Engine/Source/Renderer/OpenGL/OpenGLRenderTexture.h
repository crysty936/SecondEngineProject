#include "OpenGLTexture.h"
#include "EASTL/vector.h"

class OpenGLRenderTexture : public OpenGLTexture
{
public:
	OpenGLRenderTexture();
	virtual ~OpenGLRenderTexture();

public:
	void Init(const int32_t inTexNr = GL_TEXTURE0);
};