#pragma once
#include "Renderer/RHI/Resources/VertexBufferBase.h"

// Contains data required for drawing.
// For now just vertex with index buffer
class RenderDataContainer
{
public:
	RenderDataContainer();
	~RenderDataContainer();

	eastl::shared_ptr<class VertexBufferBase> VBuffer;
};