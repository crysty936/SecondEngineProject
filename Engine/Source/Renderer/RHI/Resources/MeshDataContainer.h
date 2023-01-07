#pragma once
#include "Renderer/RHI/Resources/RHIVertexBuffer.h"

// Contains data required for drawing.
// For now just vertex with index buffer
class MeshDataContainer
{
public:
	MeshDataContainer();
	~MeshDataContainer();

	eastl::shared_ptr<class RHIVertexBuffer> VBuffer;
	eastl::vector<eastl::shared_ptr<class RHIVertexBuffer>> AdditionalBuffers;
};