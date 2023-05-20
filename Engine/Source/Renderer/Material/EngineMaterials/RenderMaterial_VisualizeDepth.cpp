#include "Renderer/Material/EngineMaterials/RenderMaterial_VisualizeDepth.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "Core/EngineUtils.h"
#include "Renderer/Material/MaterialHelpers.h"
#include "Renderer/DrawDebugHelpers.h"

RenderMaterial_VisualizeDepth::RenderMaterial_VisualizeDepth() = default;
RenderMaterial_VisualizeDepth::~RenderMaterial_VisualizeDepth() = default;

void RenderMaterial_VisualizeDepth::SetRequiredUniforms()
{
	__super::SetRequiredUniforms();

	eastl::vector<UniformWithFlag> additionalUniforms = {
	{"CameraNearFar"},
	};

	UBuffers.push_back({ additionalUniforms, ConstantBufferBinding::Pixel });
}
