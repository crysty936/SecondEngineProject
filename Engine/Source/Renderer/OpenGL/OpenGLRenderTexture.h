#include "EASTL/vector.h"
#include "EASTL/string.h"

class OpenGLRenderTexture/* : public OpenGLTexture*/
{
public:
	OpenGLRenderTexture(const eastl::string& inTexName);
	virtual ~OpenGLRenderTexture();

public:
	void Init();
};