#include "GLTexture2D.h"
#include "glad/glad.h"

GLTexture2D::GLTexture2D(uint32_t inGLHandle)
	: RHITexture2D(), GlHandle(inGLHandle)
{
}
