#include "GLShader.h"
#include "glad/glad.h"

GLShader::GLShader(const uint32_t inGLHandle, const uint32_t inVertexHandle, const uint32_t inFragmentHandle, const uint32_t inGeometryHandle)
	: MainHandle(inGLHandle), VertexHandle(inVertexHandle), FragmentHandle(inFragmentHandle), GeometryHandle(inGeometryHandle) {}
