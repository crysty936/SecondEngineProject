#pragma once
#include "Renderer/DrawType.h"
#include "EASTL/shared_ptr.h"


namespace EDrawMode
{
	enum Type : uint8_t
	{
		Default = 1 << 0,
		DEPTH_VISUALIZE = 1 << 2,
		OUTLINE = 1 << 3,
		NORMAL_VISUALIZE = 1 << 4,
		Count = 1 << 5
	};
}

struct RenderCommand
{
	eastl::shared_ptr<class MeshDataContainer> DataContainer;
	eastl::shared_ptr<class RenderMaterial> Material;
	eastl::weak_ptr<const class DrawableObject> Parent;
	EDrawCallType DrawType{ EDrawCallType::DrawElements };
	EDrawMode::Type DrawPasses = EDrawMode::Default;

	/** In case draw mode is DrawInstanced */
	int32_t InstancesCount = 0;
};

