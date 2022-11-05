#include "GLTexture2D.h"
#include "glad/glad.h"

GLTexture2D::GLTexture2D(uint32_t inGLHandle)
	: RHITexture2D(), GlHandle(inGLHandle)
{
}

void GLTexture2D::Bind(int32_t inTexNr)
{
	glActiveTexture(GL_TEXTURE0 + inTexNr);
	glBindTexture(GL_TEXTURE_2D, GlHandle);
}

void GLTexture2D::Unbind(int32_t inTexNr)
{
	glActiveTexture(GL_TEXTURE0 + inTexNr);
	glBindTexture(GL_TEXTURE_2D, 0);
}
