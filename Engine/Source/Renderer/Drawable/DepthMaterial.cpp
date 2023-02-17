#include "Renderer/Drawable/DepthMaterial.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "Core/EngineUtils.h"
#include "Renderer/Material/MaterialHelpers.h"
#include "glm/ext/matrix_transform.inl"

DepthMaterial::DepthMaterial() = default;
DepthMaterial::~DepthMaterial() = default;

const eastl::string lsMatrixName = "lsMatrix";

void DepthMaterial::SetRequiredUniforms()
{
	eastl::vector<UniformWithFlag> defaultUniforms = {
	{"lsMatrix"},
	{"model"}
	};

	UBuffers.push_back({ defaultUniforms, ConstantBufferType::Vertex });
}

void DepthMaterial::SetUniforms(eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache)
{
	__super::SetUniforms(inUniformsCache);
}

