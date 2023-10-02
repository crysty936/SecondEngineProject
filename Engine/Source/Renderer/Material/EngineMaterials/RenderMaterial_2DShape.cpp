#include "Renderer/Material/EngineMaterials/RenderMaterial_2DShape.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "Core/EngineUtils.h"
#include "Renderer/Material/MaterialHelpers.h"
#include "Renderer/RHI/RHI.h"
#include "imgui.h"

RenderMaterial_2DShape::RenderMaterial_2DShape() = default;
RenderMaterial_2DShape::~RenderMaterial_2DShape() = default;

void RenderMaterial_2DShape::SetUniformsValue(eastl::unordered_map<eastl::string, struct SelfRegisteringUniform>& inUniformsCache, const EShaderType inShaderTypes)
{
	// 2D Shapes don't need the z axis scale, it screws things up

	glm::mat4& model = inUniformsCache["model"].GetValue<glm::mat4>();
	glm::vec4 thirdColumn = model[2];
	model[2] = glm::vec4(0.f, 0.f, 0.f, thirdColumn.w);

	__super::SetUniformsValue(inUniformsCache, inShaderTypes);
}

