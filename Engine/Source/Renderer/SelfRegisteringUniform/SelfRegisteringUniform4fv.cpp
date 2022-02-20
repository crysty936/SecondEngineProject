#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform4fv.h"
#include "Renderer/OpenGL/OpenGLShader.h"

SelfRegisteringUniform4fv::SelfRegisteringUniform4fv(const glm::mat4 inValue)
	: Value{inValue}
{
}

SelfRegisteringUniform4fv::~SelfRegisteringUniform4fv() = default;

void SelfRegisteringUniform4fv::RegisterUniform(const OpenGLShader& inShader, const eastl::string& inName)
{
	inShader.SetUniformValue4fv(inName, Value);
}
