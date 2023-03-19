#pragma once
#include "Renderer/RHI/RHI.h"
#include "EASTL/string.h"

class OpenGLRHI : public RHI
{
public:
	OpenGLRHI();
	~OpenGLRHI();

	eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexInputLayout& inLayout, const float* inVertices, const int32_t inCount, eastl::shared_ptr<class RHIIndexBuffer> inIndexBuffer = nullptr) override;
	eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexInputLayout& inLayout, const eastl::vector<Vertex>& inVertices, eastl::shared_ptr<class RHIIndexBuffer> inIndexBuffer = nullptr) override;
	virtual eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexInputLayout& inLayout, const void* inData, const size_t inSize, eastl::shared_ptr<class RHIIndexBuffer> inIndexBuffer = nullptr) override;

	virtual void ClearVertexBuffer(class RHIVertexBuffer& inBuffer) override;
	virtual void UpdateVertexBufferData(class RHIVertexBuffer& inBuffer, const void* inData, const size_t inSize) override;

	eastl::shared_ptr<class RHIIndexBuffer> CreateIndexBuffer(const uint32_t* inData, uint32_t inCount) override;

	eastl::shared_ptr<class RHIUniformBuffer> CreateUniformBuffer(size_t inSize) override;

	eastl::shared_ptr<class RHIShader> CreateShaderFromSource(const eastl::vector<ShaderSourceInput> inShaderSources, const VertexInputLayout& inInputLayout, const eastl::string& inVSName = "VS", const eastl::string& inPSName = "PS") override;
	eastl::shared_ptr<class RHIShader> CreateShaderFromPath(const eastl::vector<ShaderSourceInput> inPathShaderSources, const VertexInputLayout& inInputLayout) override;

	virtual eastl::shared_ptr<class RHITexture2D> CreateTexture2D(const eastl::string& inDataPath) override;
	virtual eastl::shared_ptr<class RHITexture2D> CreateRenderTexture() override;
	virtual eastl::shared_ptr<class RHITexture2D> CreateDepthMap(const int32_t inWidth, const int32_t inHeight) override;
	virtual eastl::shared_ptr<class RHITexture2D> CreateArrayDepthMap(const int32_t inDepthMapWidth, const int32_t inDepthMapHeight, const int32_t inSize) override;
	virtual eastl::shared_ptr<class RHIFrameBuffer> CreateDepthStencilFrameBuffer() override;
	virtual eastl::shared_ptr<class RHIFrameBuffer> CreateEmptyFrameBuffer() override;


	virtual void BindVertexBuffer(const RHIVertexBuffer& inBuffer, const bool inBindIndexBuffer = true) override;
	virtual void BindIndexBuffer(const RHIIndexBuffer& inBuffer) override;
	virtual void BindShader(const RHIShader& inShader) override;
	virtual void BindUniformBuffer(const RHIUniformBuffer& inBuffer) override;
	virtual void BindTexture2D(const RHITexture2D& inTex, const int32_t inTexId) override;

	virtual void UnbindVertexBuffer(const RHIVertexBuffer& inBuffer, const bool inUnbindIndexBuffer = true) override;
	virtual void UnbindIndexBuffer(const RHIIndexBuffer& inBuffer) override;
	virtual void UnbindShader(const RHIShader& inShader) override;
	virtual void UnbindUniformBuffer(const RHIUniformBuffer& inBuffer) override;
	virtual void UnbindTexture2D(const RHITexture2D& inTex, const int32_t inTexId) override;

	virtual void UniformBufferUpdateData(RHIUniformBuffer& inBuffer, const void* inData, const size_t inDataSize, const int32_t inBufferNr) override;
	virtual void AttachTextureToFramebufferColor(RHIFrameBuffer& inFrameBuffer, RHITexture2D& inTex) override;
	virtual void AttachTextureToFramebufferDepth(RHIFrameBuffer& inFrameBuffer, RHITexture2D& inTex) override;


	void SetViewportSize(const int32_t inWidth, const int32_t inHeight) override;


	void ClearColor(const glm::vec4 inColor) override;


	void SwapBuffers() override;


	virtual void ClearBuffers() override;

	virtual void PrepareProjectionForRendering(glm::mat4& inProj) override;

	virtual void DrawElements(const int32_t inElementsCount) override;
	virtual void DrawInstanced(const int32_t inElementsCount, const int32_t inInstancesCount) override;
	virtual void DrawPoints(const int32_t inCount) override;


	virtual void ClearTexture(const RHITexture2D& inTexture, const glm::vec4& inColor) override;


	virtual void BindFrameBuffer(const class RHIFrameBuffer& inFrameBuffer) override;


	virtual void UnbindFrameBuffer(const class RHIFrameBuffer& inFrameBuffer) override;


	virtual void SetFaceCullMode(const EFaceCullMode inMode) override;

	void ReadBufferData(const RHIBufferBase& inBuffer, const size_t inOffset, const size_t inSize, void* outData) override;

	// ImGui
	void ImGuiInit() override;
	void ImGuiBeginFrame() override;
	void ImGuiRenderDrawData() override;



private:
	void LoadImageToTextureFromPath(RHITexture2D& inTexture, const eastl::string& inPath);

	void* GLContext = nullptr;
};