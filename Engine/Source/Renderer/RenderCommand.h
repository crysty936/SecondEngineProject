#pragma once
#include "Renderer/DrawType.h"
#include "EASTL/shared_ptr.h"

struct RenderCommand
{
	eastl::shared_ptr<class RenderDataContainer> DataContainer;
	eastl::shared_ptr<class RenderMaterial> Material;
	eastl::weak_ptr<const class DrawableObject> Parent;
	EDrawCallType DrawType{ EDrawCallType::DrawElements };
	
	bool test = false;
};