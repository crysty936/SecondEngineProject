#include "Renderer/Drawable/RenderMaterial_WithLighting.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "Core/EngineUtils.h"
#include "Renderer/Material/MaterialHelpers.h"
#include "Renderer/ForwardRenderer.h"

RenderMaterial_WithLighting::RenderMaterial_WithLighting() = default;
RenderMaterial_WithLighting::~RenderMaterial_WithLighting() = default;

void RenderMaterial_WithLighting::SetRequiredUniforms()
{
	__super::SetRequiredUniforms();
	eastl::vector<UniformWithFlag> defaultUniforms = {
	{"DirectionalLightDirection"},
	{"bNormalVisualizeMode"},
	{"bUseNormalMapping"},
	{"bUseParallaxMapping"},
	{"ParallaxHeightScale"},
	{"ViewPos"},
	};

	UBuffers.push_back({ defaultUniforms, ConstantBufferType::Vertex });
}

