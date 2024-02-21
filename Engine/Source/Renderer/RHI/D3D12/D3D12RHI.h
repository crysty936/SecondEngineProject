#pragma once
#include "Renderer/RHI/RHI.h"
#include "EASTL/string.h"

class D3D12RHI : public RHI
{
public:
	D3D12RHI();
	~D3D12RHI();

	void CreateTextureStuff();
	void WaitForPreviousFrame();
	void MoveToNextFrame();
	void Test() override;

	void ImGuiInit() override;

	void ImGuiBeginFrame() override;


	void ImGuiRenderDrawData() override;


	void SwapBuffers() override;

	eastl::shared_ptr<class RHIIndexBuffer> CreateIndexBuffer(const uint32_t* inData, uint32_t inCount) override;

	eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexInputLayout& inLayout, const float* inVertices, const int32_t inCount, eastl::shared_ptr<class RHIIndexBuffer> inIndexBuffer = nullptr) override;




	eastl::shared_ptr<class RHITexture2D> CreateAndLoadTexture2D(const eastl::string& inDataPath, const bool inSRGB) override;


};