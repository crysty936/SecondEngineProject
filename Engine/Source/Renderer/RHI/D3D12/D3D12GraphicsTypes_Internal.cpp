#include "D3D12GraphicsTypes_Internal.h"
#include "D3D12Utility.h"
#include <combaseapi.h>

DescriptorHeap::~DescriptorHeap()
{
	Heap->Release();
}

void DescriptorHeap::Init(bool inShaderVisible, uint32_t inNumPersistent, D3D12_DESCRIPTOR_HEAP_TYPE inHeapType)
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = inNumPersistent;
	rtvHeapDesc.Type = inHeapType;
	rtvHeapDesc.Flags = inShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	D3D12Utility::DXAssert(D3D12Globals::Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&Heap)));

	DescriptorSize = D3D12Globals::Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	
	CPUStart = Heap->GetCPUDescriptorHandleForHeapStart();
	GPUStart = Heap->GetGPUDescriptorHandleForHeapStart();
}


