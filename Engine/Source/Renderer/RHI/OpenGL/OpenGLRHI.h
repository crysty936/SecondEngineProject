#pragma once
#include "Renderer/RHI/RHIBase.h"
#include "EASTL/string.h"

class OpenGLRHI : public RHIBase
{
public:
	OpenGLRHI();
	~OpenGLRHI();

	virtual eastl::shared_ptr<class VertexBufferBase> CreateVertexBuffer(const class VertexBufferLayout& inLayout, const float* inVertices, const int32_t inCount) override;
	virtual eastl::shared_ptr<class VertexBufferBase> CreateVertexBuffer(const class VertexBufferLayout& inLayout, const eastl::vector<Vertex>& inVertices) override;


	virtual eastl::shared_ptr<class IndexBufferBase> CreateIndexBuffer(const uint32_t* inData, uint32_t inCount) override;


	virtual void SetViewportSize(const int32_t inWidth, const int32_t inHeight) override;


	virtual void ClearColor(const glm::vec4 inColor) override;


	virtual void SwapBuffers() override;


	virtual eastl::shared_ptr<class ShaderBase> CreateShaderFromSource(const eastl::string& inVertexSrc, const eastl::string& inPixelSrc) override;
	virtual eastl::shared_ptr<class ShaderBase> CreateShaderFromPath(const eastl::string& inVertexPath, const eastl::string& inPixelPath) override;
};