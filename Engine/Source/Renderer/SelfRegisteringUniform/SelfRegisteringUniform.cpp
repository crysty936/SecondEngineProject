#include "SelfRegisteringUniform.h"
#include "Renderer/OpenGL/OpenGLShader.h"

SelfRegisteringUniform::SelfRegisteringUniform(const float inValue)
	:Type{UniformType::Uniform1f}, Value{inValue} {}

SelfRegisteringUniform::SelfRegisteringUniform(const glm::mat4 inValue)
	: Type{ UniformType::Uniform4fv }, Value{ inValue } {}

// SelfRegisteringUniform::SelfRegisteringUniform(const OpenGLTexture& inTexture)
// 	: Type{UniformType::Texture}, Value(inTexture.TexHandle) {}

SelfRegisteringUniform::SelfRegisteringUniform()
	: Type{}, Value{} {}


void SelfRegisteringUniform::Register(const eastl::string& inThisName, const OpenGLShader& inShader) const
{
	switch (Type)
	{
	case UniformType::Uniform1f:
	{
		inShader.SetUniformValue1f(inThisName, Value.Value1f);
		break;
	}
	case UniformType::Uniform4fv:
	{
		inShader.SetUniformValue4fv(inThisName, Value.Value4fv);
		break;
	}
	}
}

