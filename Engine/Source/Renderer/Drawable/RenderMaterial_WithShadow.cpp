#include "Renderer/Drawable/RenderMaterial_WithShadow.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "Core/EngineUtils.h"
#include "Renderer/Material/MaterialHelpers.h"
#include "Renderer/ForwardRenderer.h"

RenderMaterial_WithShadow::RenderMaterial_WithShadow() = default;
RenderMaterial_WithShadow::~RenderMaterial_WithShadow() = default;

const eastl::string lsMatrixName = "lsMatrix";

void RenderMaterial_WithShadow::SetRequiredUniforms()
{
	__super::SetRequiredUniforms();
	eastl::vector<UniformWithFlag> defaultUniforms = {
	{"lsMatrices", 3},
	{"ShadowCameraViewMatrix"},
	{"DirectionalLightDirection"},
	{"cascadesCount"},
	{"bShadowVisualizeMode"},
	{"bNormalVisualizeMode"},
	{"bUseNormalMapping"},
	{"bUseShadows"},
	{"shadowCascadeFarPlanes", 3},
	};

	UBuffers.push_back({ defaultUniforms, ConstantBufferType::Vertex });


	eastl::vector<UniformWithFlag> LightingUniforms = {
		{"ViewPos"},
	};

	UBuffers.push_back({ LightingUniforms, ConstantBufferType::Vertex });

}

void RenderMaterial_WithShadow::SetUniforms(eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache)
{
	__super::SetUniforms(inUniformsCache);

	const eastl::weak_ptr<RHITexture2D> depthTexture = ForwardRenderer::Get().GetDepthTexture();
	
	if (eastl::find_if(ExternalTextures.begin(), ExternalTextures.end(), 
		[depthTexture](const eastl::weak_ptr<RHITexture2D>& inPtr) {
		return depthTexture.lock() == inPtr.lock(); 
		}) != ExternalTextures.end())
	{
		return;
	}

	ExternalTextures.push_back(ForwardRenderer::Get().GetDepthTexture());
}

