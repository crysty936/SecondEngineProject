#include "OpenGLTexture.h"
#include "EASTL/vector.h"

class OpenGLRenderTexture : public OpenGLTexture
{
public:
	OpenGLRenderTexture(const eastl::string& inTexName);
	virtual ~OpenGLRenderTexture();

public:
	void Init();
};