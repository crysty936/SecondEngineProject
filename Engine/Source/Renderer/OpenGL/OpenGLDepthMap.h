#include "EASTL/vector.h"
#include "EASTL/string.h"

class OpenGLDepthMap /*: public OpenGLTexture*/
{
public:
	OpenGLDepthMap(const eastl::string& inTexName);
	virtual ~OpenGLDepthMap();

public:
	void Init();
};