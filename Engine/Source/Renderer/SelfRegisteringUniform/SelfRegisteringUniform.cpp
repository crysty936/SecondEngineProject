#include "SelfRegisteringUniform.h"
#include "Renderer/RHI/Resources/UniformBufferContainer.h"

SelfRegisteringUniform::SelfRegisteringUniform(const uint32_t inValue)
	: Type{ UniformType::Uniform1ui }, Value{ inValue } {}

SelfRegisteringUniform::SelfRegisteringUniform(const float inValue)
	:Type{UniformType::Uniform1f}, Value{inValue} {}

SelfRegisteringUniform::SelfRegisteringUniform(const glm::mat4 inValue)
	: Type{ UniformType::Uniform4fv }, Value{ inValue } {}

SelfRegisteringUniform::SelfRegisteringUniform(const glm::vec3 inValue)
	: Type{ UniformType::Uniform3f }, Value{ inValue } {}

// SelfRegisteringUniform::SelfRegisteringUniform(const OpenGLTexture& inTexture)
// 	: Type{UniformType::Texture}, Value(inTexture.TexHandle) {}

SelfRegisteringUniform::SelfRegisteringUniform()
	: Type{}, Value{} {}

void SelfRegisteringUniform::Register(UniformBufferContainer& inBuffer) const
{
	switch (Type)
	{
	case UniformType::Uniform1f:
	{
		constexpr size_t thisSize = sizeof(float);
		char thisData[thisSize];
		memcpy(&thisData, &Value.Value1f, thisSize);

		inBuffer.AddData(thisData, thisSize);

		break;
	}
	case UniformType::Uniform4fv:
	{
		constexpr size_t thisSize = sizeof(float) * 4 * 4;
		char thisData[thisSize];
		memcpy(&thisData, &Value.Value4fv, thisSize);

		inBuffer.AddData(thisData, thisSize);
		break;
	}
	case UniformType::Uniform1ui:
	{
		constexpr size_t thisSize = sizeof(uint32_t);
		char thisData[thisSize];
		memcpy(&thisData, &Value.Value1ui, thisSize);

		inBuffer.AddData(thisData, thisSize);
		break;
	}
	case UniformType::Uniform3f:
	{
		constexpr size_t thisSize = sizeof(float) * 4; // vec3 has the same alignment as vec4 in GLSL, TODO: Test if this applies in HLSL as well
		char thisData[thisSize];
		memcpy(&thisData, &Value.Value3f, thisSize);

		inBuffer.AddData(thisData, thisSize);
		break;
	}
	}
}

