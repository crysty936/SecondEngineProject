#include "OpenGLTexture.h"
#include "EASTL/vector.h"

class OpenGLCubeMap : public OpenGLTexture
{
public:
	OpenGLCubeMap(const eastl::string& inTexName);
	virtual ~OpenGLCubeMap();

public:
	void Init(const eastl::vector<eastl::string>& inTexturePaths, const int32_t inTexNr = GL_TEXTURE0);
};