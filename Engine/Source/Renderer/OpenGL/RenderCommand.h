#pragma once
#include "Renderer/OpenGL/DrawType.h"

struct RenderCommand
{
	eastl::shared_ptr<class RenderDataContainer> DataContainer{ nullptr };
	eastl::shared_ptr<class RenderMaterial> Material{ nullptr };
	eastl::weak_ptr<const class DrawableObject> Parent;
	EDrawCallType DrawType{ EDrawCallType::DrawElements };
};