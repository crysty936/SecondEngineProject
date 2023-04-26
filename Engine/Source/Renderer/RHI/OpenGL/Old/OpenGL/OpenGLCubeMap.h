#include "EASTL/vector.h"
#include "EASTL/string.h"

class OpenGLCubeMap /*: public OpenGLTexture*/
{
public:
	OpenGLCubeMap(const eastl::string& inTexName);
	virtual ~OpenGLCubeMap();

public:
	void Init(const eastl::vector<eastl::string>& inTexturePaths, int32_t inTexNr = 0);
};