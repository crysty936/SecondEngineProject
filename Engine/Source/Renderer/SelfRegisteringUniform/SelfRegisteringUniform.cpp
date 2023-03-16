#include "SelfRegisteringUniform.h"
#include "Renderer/RHI/Resources/UniformBufferContainer.h"

SelfRegisteringUniform::SelfRegisteringUniform(const int32_t inValue)
	: Type{ UniformType::Uniform1i }, Data{ eastl::make_unique<SelfRegisteringUniformData<int32_t>>(inValue) } {}

SelfRegisteringUniform::SelfRegisteringUniform(const uint32_t inValue)
	: Type{ UniformType::Uniform1ui }, Data{eastl::make_unique<SelfRegisteringUniformData<uint32_t>>(inValue)} {}

SelfRegisteringUniform::SelfRegisteringUniform(const float inValue)
	:Type{UniformType::Uniform1f}, Data{ eastl::make_unique<SelfRegisteringUniformData<float>>(inValue) } {}

SelfRegisteringUniform::SelfRegisteringUniform(const glm::mat4& inValue)
	: Type{ UniformType::Uniform4fv }, Data{ eastl::make_unique<SelfRegisteringUniformData<glm::mat4>>(inValue) } {}

SelfRegisteringUniform::SelfRegisteringUniform(const glm::vec3& inValue)
	: Type{ UniformType::Uniform3f }, Data{ eastl::make_unique<SelfRegisteringUniformData<glm::vec3>>(inValue) } {}

SelfRegisteringUniform::SelfRegisteringUniform(const glm::vec4& inValue)
	: Type{ UniformType::Uniform4f }, Data{ eastl::make_unique<SelfRegisteringUniformData<glm::vec4>>(inValue) } {}

SelfRegisteringUniform::SelfRegisteringUniform()
	: Type{} {}

SelfRegisteringUniform::SelfRegisteringUniform(const eastl::vector<float>& inValue)
	: Type{ UniformType::UniformfArray }, Data{ eastl::make_unique<SelfRegisteringUniformData<eastl::vector<float>>>(inValue) } {}

SelfRegisteringUniform::SelfRegisteringUniform(const eastl::vector<glm::mat4>& inValue)
	: Type{ UniformType::Uniform4fvArray }, Data{ eastl::make_unique<SelfRegisteringUniformData<eastl::vector<glm::mat4>>>(inValue) } {}

SelfRegisteringUniform::SelfRegisteringUniform(const eastl::shared_ptr<RHITexture2D>& inValue)
	: Type{ UniformType::UniformTexture }, Data{ eastl::make_unique < SelfRegisteringUniformData<eastl::shared_ptr<RHITexture2D>>>(inValue) } {}


void SelfRegisteringUniform::Register(UniformBufferContainer& inBuffer, const size_t inRequiredCount) const
{
	Data->SelfRegister(inBuffer, inRequiredCount);
}

