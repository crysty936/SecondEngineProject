#include "OpenGLTexture.h"
#include "EASTL/vector.h"

class OpenGLDepthMap : public OpenGLTexture
{
public:
	OpenGLDepthMap();
	virtual ~OpenGLDepthMap();

public:
	void Init(const int32_t inTexNr = GL_TEXTURE0);
};