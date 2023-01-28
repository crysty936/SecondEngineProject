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
	glm::mat4 view(glm::identity<glm:: mat4>());
	bool success = MaterialHelpers::TryGetUniformFromCache<glm::mat4>(inUniformsCache, "view", view);

	glm::mat4 proj(glm::identity<glm::mat4>());
	success = MaterialHelpers::TryGetUniformFromCache(inUniformsCache, "projection", proj);

	if (!success)
	{
		return;
	}

	inUniformsCache[lsMatrixName] = proj * view;

	__super::SetUniforms(inUniformsCache);
}

