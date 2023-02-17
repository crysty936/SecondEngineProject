#include "SelfRegisteringUniform.h"
#include "Renderer/RHI/Resources/UniformBufferContainer.h"

SelfRegisteringUniform::SelfRegisteringUniform(const uint32_t inValue)
	: Type{ UniformType::Uniform1ui }, Data{eastl::make_unique<SelfRegisteringUniformData<uint32_t>>(inValue)} {}

SelfRegisteringUniform::SelfRegisteringUniform(const float inValue)
	:Type{UniformType::Uniform1f}, Data{ eastl::make_unique<SelfRegisteringUniformData<float>>(inValue) } {}

SelfRegisteringUniform::SelfRegisteringUniform(const glm::mat4 inValue)
	: Type{ UniformType::Uniform4fv }, Data{ eastl::make_unique<SelfRegisteringUniformData<glm::mat4>>(inValue) } {}

SelfRegisteringUniform::SelfRegisteringUniform(const glm::vec3 inValue)
	: Type{ UniformType::Uniform3f }, Data{ eastl::make_unique<SelfRegisteringUniformData<glm::vec3>>(inValue) } {}

SelfRegisteringUniform::SelfRegisteringUniform()
	: Type{} {}

SelfRegisteringUniform::SelfRegisteringUniform(const eastl::vector<glm::mat4>& inValue)
	: Type{ UniformType::Uniform4fvArray }, Data{ eastl::make_unique<SelfRegisteringUniformData<eastl::vector<glm::mat4>>>(inValue) } {}

void SelfRegisteringUniform::Register(UniformBufferContainer& inBuffer) const
{
	inBuffer.AddData(Data->GetData(), Data->GetSize());
}

