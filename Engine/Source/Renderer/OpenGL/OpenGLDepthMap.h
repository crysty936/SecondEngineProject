#include "OpenGLTexture.h"
#include "EASTL/vector.h"

class OpenGLDepthMap : public OpenGLTexture
{
public:
	OpenGLDepthMap(const eastl::string& inTexName);
	virtual ~OpenGLDepthMap();

public:
	void Init();
};