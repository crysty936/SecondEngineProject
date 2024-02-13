#pragma once

#include "Renderer/RHI/Resources/RHIIndexBuffer.h"
#include <dxgiformat.h>
#include <d3d12.h>

class D3D12IndexBuffer : public RHIIndexBuffer
{
public:
	D3D12IndexBuffer();

public:
	struct ID3D12Resource* Handle = nullptr;
	DXGI_FORMAT IBFormat = DXGI_FORMAT_UNKNOWN;
	uint64_t GPUAddress = 0;
	uint64_t Size = 0;

	D3D12_INDEX_BUFFER_VIEW IBView() const;

};


