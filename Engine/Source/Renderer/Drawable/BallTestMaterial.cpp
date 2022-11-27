#include "Renderer/Drawable/BallTestMaterial.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"

BallTestMaterial::BallTestMaterial() = default;
BallTestMaterial::~BallTestMaterial() = default;

void BallTestMaterial::SetUniforms(eastl::unordered_map<eastl::string, SelfRegisteringUniform>& inUniformsCache)
{
	glm::vec3 lightPos(10.f, 20.f, 15.f);
	static float xRotation = 0.0f;
	xRotation += 0.05f;
	inUniformsCache["XRotation"] = SelfRegisteringUniform(xRotation);

	inUniformsCache["LightPos"] = SelfRegisteringUniform(lightPos);

	const glm::mat4& proj = inUniformsCache["projection"].Value.Value4fv;
	const glm::mat4& view = inUniformsCache["view"].Value.Value4fv;

	const glm::mat4 invProjView = glm::inverse((proj * view));

	inUniformsCache["ScreenToWorld"] = SelfRegisteringUniform(invProjView);

 	RenderMaterial::SetUniforms(inUniformsCache);
}

void BallTestMaterial::SetRequiredUniforms()
{
  	eastl::vector<UniformWithFlag> additionalUniforms = {
  	{"LightPos"},
  	{"ScreenToWorld"},
	{"XRotation"},
  	};
  
	UBuffers.push_back({ additionalUniforms, ConstantBufferType::Pixel });

	__super::SetRequiredUniforms();
}
