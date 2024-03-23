#pragma once

#include "Renderer/RHI/Resources/RHIIndexBuffer.h"
#include "Renderer/RHI/Resources/RHIVertexBuffer.h"
#include <dxgiformat.h>
#include <d3d12.h>
#include "Renderer/RHI/Resources/RHITexture.h"

class D3D12IndexBuffer : public RHIIndexBuffer
{
public:
	D3D12IndexBuffer();

	D3D12_INDEX_BUFFER_VIEW IBView() const;

public:
	struct ID3D12Resource* Resource = nullptr;
	DXGI_FORMAT IBFormat = DXGI_FORMAT_UNKNOWN;
	uint64_t GPUAddress = 0;
	uint32_t Size = 0;


};


class D3D12VertexBuffer : public RHIVertexBuffer
{
public:
	D3D12VertexBuffer();

	D3D12_VERTEX_BUFFER_VIEW VBView() const;

public:
	struct ID3D12Resource* Resource = nullptr;
	uint64_t GPUAddress = 0;
	uint64_t NumElements = 0;


};

class D3D12Texture2D : public RHITexture2D
{
public:
	ID3D12Resource* Resource = nullptr;
	uint32_t SRVIndex = -1;
};