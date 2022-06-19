#include "OpenGLTexture.h"
#include "EASTL/vector.h"

class OpenGLCubeMap : public OpenGLTexture
{
public:
	OpenGLCubeMap();
	virtual ~OpenGLCubeMap();

	void Bind() const override;
	void Unbind() const override;

public:
	void Init(const eastl::vector<eastl::string>& inTexturePaths, const int32_t inTexNr = GL_TEXTURE0);
};