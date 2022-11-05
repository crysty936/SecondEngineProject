#include "GLShader.h"
#include "glad/glad.h"

GLShader::GLShader(const uint32_t inGLHandle)
	: Handle(inGLHandle) {}

void GLShader::Bind()
{
	glUseProgram(Handle);
}

void GLShader::Unbind()
{
	glUseProgram(0);
}
