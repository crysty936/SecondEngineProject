#pragma once
#include "Renderer/RHI/RHI.h"
#include "EASTL/string.h"

class D3D11RHI : public RHI
{
public:
	D3D11RHI();
	~D3D11RHI();

	eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexInputLayout& inLayout, const float* inVertices, const int32_t inCount, eastl::shared_ptr<class RHIIndexBuffer> inIndexBuffer) override;
	eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexInputLayout& inLayout, const eastl::vector<Vertex>& inVertices, eastl::shared_ptr<class RHIIndexBuffer> inIndexBuffer) override;

 	eastl::shared_ptr<class RHIIndexBuffer> CreateIndexBuffer(const uint32_t* inData, uint32_t inCount) override;

 	eastl::shared_ptr<class RHIUniformBuffer> CreateUniformBuffer(size_t inSize) override;
 
// 	virtual eastl::shared_ptr<class RHITexture2D> CreateTexture2D() override;

	virtual void BindVertexBuffer(const RHIVertexBuffer& inBuffer, const bool inBindIndexBuffer = true) override;
	virtual void BindIndexBuffer(const RHIIndexBuffer& inBuffer) override;
	virtual void BindShader(const class RHIShader& inShader) override;
	virtual void BindUniformBuffer(const RHIUniformBuffer& inBuffer) override;

	virtual void UnbindVertexBuffer(const RHIVertexBuffer& inBuffer, const bool inUnbindIndexBuffer = true) override;
	virtual void UnbindIndexBuffer(const RHIIndexBuffer& inBuffer) override;
	virtual void UnbindShader(const class RHIShader& inShader) override;
	virtual void UnbindUniformBuffer(const RHIUniformBuffer& inBuffer) override;

	virtual void UniformBufferUpdateData(RHIUniformBuffer& inBuffer, const void* inData, const size_t inDataSize) override;

	eastl::shared_ptr<class RHIShader> CreateShaderFromSource(const eastl::string& inVertexSrc, const eastl::string& inPixelSrc, const VertexInputLayout& inInputLayout) override;
	eastl::shared_ptr<class RHIShader> CreateShaderFromPath(const eastl::string& inVertexPath, const eastl::string& inPixelPath, const VertexInputLayout& inInputLayout) override;


// 	virtual void LoadTextureFromPath(RHITexture2D& inTexture, const eastl::string& inPath) override;
// 
// 	void SetViewportSize(const int32_t inWidth, const int32_t inHeight) override;
// 
// 	void ClearColor(const glm::vec4 inColor) override;
// 
// 	virtual void DrawElements(const int32_t inElementsCount) override;
// 
// 	void SwapBuffers() override;
// 
// 

	virtual void ClearBuffers() override;


	virtual void DrawElements(const int32_t inElementsCount) override;


	virtual void SwapBuffers() override;


	virtual void ClearColor(const glm::vec4 inColor) override;


	virtual void PrepareProjectionForRendering(glm::mat4& inProj) override;

private:
	glm::vec4 CurrentClearColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

};