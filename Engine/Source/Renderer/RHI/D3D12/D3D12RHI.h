#pragma once
#include "Renderer/RHI/RHI.h"
#include "EASTL/string.h"

class D3D12RHI : public RHI
{
public:
	D3D12RHI();
	~D3D12RHI();


	void CreateTextureStuff(struct ID3D12Resource* inUploadHeap);
	void WaitForPreviousFrame();
	void MoveToNextFrame();
	void Test() override;


	void ImGuiInit() override;


	void ImGuiBeginFrame() override;


	void ImGuiRenderDrawData() override;


	void SwapBuffers() override;




	eastl::shared_ptr<class RHIIndexBuffer> CreateIndexBuffer(const uint32_t* inData, uint32_t inCount) override;

};