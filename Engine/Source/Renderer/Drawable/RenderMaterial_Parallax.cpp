#include "Renderer/Drawable/RenderMaterial_Parallax.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "Core/EngineUtils.h"
#include "Renderer/Material/MaterialHelpers.h"
#include "Renderer/ForwardRenderer.h"

RenderMaterial_Parallax::RenderMaterial_Parallax() = default;
RenderMaterial_Parallax::~RenderMaterial_Parallax() = default;

void RenderMaterial_Parallax::SetRequiredUniforms()
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

	UBuffers.push_back({ defaultUniforms, ConstantBufferBinding::Vertex });
}

