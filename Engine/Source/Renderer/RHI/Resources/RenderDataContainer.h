#pragma once
#include "Renderer/RHI/Resources/RHIVertexBuffer.h"

// Contains data required for drawing.
// For now just vertex with index buffer
class RenderDataContainer
{
public:
	RenderDataContainer();
	~RenderDataContainer();

	eastl::shared_ptr<class RHIVertexBuffer> VBuffer;
};