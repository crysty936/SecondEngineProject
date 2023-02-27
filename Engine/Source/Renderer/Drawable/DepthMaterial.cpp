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
// 	eastl::vector<UniformWithFlag> defaultUniforms = {
// 	{"lsMatrix"},
// 	{"model"}
// 	};
// 
// 	UBuffers.push_back({ defaultUniforms, ConstantBufferType::Vertex });

  	eastl::vector<UniformWithFlag> vertexUniforms = {
  	{"model"}
  	};
  
	UBuffers.push_back({ vertexUniforms, ConstantBufferType::Vertex });

	eastl::vector<UniformWithFlag> geometryUniforms = {
	{"lsMatrices", 3}
	};

	UBuffers.push_back({ geometryUniforms, ConstantBufferType::Geometry });
}

void DepthMaterial::SetUniforms(eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache)
{
	__super::SetUniforms(inUniformsCache);
}

