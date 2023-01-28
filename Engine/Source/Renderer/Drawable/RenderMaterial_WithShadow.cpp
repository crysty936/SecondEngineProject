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
	{"lsMatrix"},
	};

	UBuffers.push_back({ defaultUniforms, ConstantBufferType::Vertex });
}

void RenderMaterial_WithShadow::SetUniforms(eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache)
{
	__super::SetUniforms(inUniformsCache);

	const eastl::weak_ptr<RHITexture2D> depthTexture = ForwardRenderer::Get().GetDepthTexture();
	
	if (eastl::find_if(WeakTextures.begin(), WeakTextures.end(), [depthTexture](const eastl::weak_ptr<RHITexture2D>& inPtr) {
		return depthTexture.lock() == inPtr.lock();
		}) != WeakTextures.end())
	{
		return;
	}

	WeakTextures.push_back(ForwardRenderer::Get().GetDepthTexture());
}

