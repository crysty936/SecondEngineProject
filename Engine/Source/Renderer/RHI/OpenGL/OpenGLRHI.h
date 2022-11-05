#pragma once
#include "Renderer/RHI/RHI.h"
#include "EASTL/string.h"

class OpenGLRHI : public RHI
{
public:
	OpenGLRHI();
	~OpenGLRHI();

	eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexBufferLayout& inLayout, const float* inVertices, const int32_t inCount, eastl::shared_ptr<class IndexBufferBase> inIndexBuffer) override;
	eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexBufferLayout& inLayout, const eastl::vector<Vertex>& inVertices, eastl::shared_ptr<class IndexBufferBase> inIndexBuffer) override;

	eastl::shared_ptr<class IndexBufferBase> CreateIndexBuffer(const uint32_t* inData, uint32_t inCount) override;

	eastl::shared_ptr<class RHIUniformBuffer> CreateUniformBuffer(size_t inSize) override;

	void SetViewportSize(const int32_t inWidth, const int32_t inHeight) override;


	void ClearColor(const glm::vec4 inColor) override;

	virtual void DrawElements(const int32_t inElementsCount) override;

	void SwapBuffers() override;


	eastl::shared_ptr<class RHIShader> CreateShaderFromSource(const eastl::string& inVertexSrc, const eastl::string& inPixelSrc) override;
	eastl::shared_ptr<class RHIShader> CreateShaderFromPath(const eastl::string& inVertexPath, const eastl::string& inPixelPath) override;


};