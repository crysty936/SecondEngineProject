#pragma once
#include <d3d12.h>
#include <stdint.h>

struct DescriptorHeap
{
	ID3D12DescriptorHeap* Heap = nullptr;
	uint32_t NumPersistentDescriptors = 0;

	uint32_t DescriptorSize = 0;
	D3D12_DESCRIPTOR_HEAP_TYPE HeapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	D3D12_CPU_DESCRIPTOR_HANDLE CPUStart = {};
	D3D12_GPU_DESCRIPTOR_HANDLE GPUStart = {};

	~DescriptorHeap();

	void Init(bool inShaderVisible, uint32_t inNumPersistent, D3D12_DESCRIPTOR_HEAP_TYPE inHeapType);

};




