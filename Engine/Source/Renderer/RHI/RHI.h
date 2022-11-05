#pragma once
#include "EASTL/shared_ptr.h"
#include "EASTL/string.h"
#include "EASTL/vector.h"
#include "Renderer/RenderingPrimitives.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"

class RHI
{
public:
	static void Init();
	static void Terminate();

public:
	virtual eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexInputLayout& inLayout, const float* inVertices, const int32_t inCount, eastl::shared_ptr<class RHIIndexBuffer> inIndexBuffer) { return nullptr; }
	virtual eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexInputLayout& inLayout, const eastl::vector<Vertex>& inVertices, eastl::shared_ptr<class RHIIndexBuffer> inIndexBuffer) { return nullptr; }

	virtual eastl::shared_ptr<class RHIIndexBuffer> CreateIndexBuffer(const uint32_t* inData, uint32_t inCount) { return nullptr; }

	virtual eastl::shared_ptr<class RHIUniformBuffer> CreateUniformBuffer(size_t inSize) { return nullptr; }

	virtual void BindVertexBuffer(const class RHIVertexBuffer& inBuffer, const bool inBindIndexBuffer = true) {};
	virtual void BindIndexBuffer(const class RHIIndexBuffer& inBuffer) {};
	virtual void BindShader(const class RHIShader& inShader) {};
	virtual void BindUniformBuffer(const RHIUniformBuffer& inBuffer) {};

	virtual void UnbindVertexBuffer(const class RHIVertexBuffer& inBuffer, const bool inUnbindIndexBuffer = true) {};
	virtual void UnbindIndexBuffer(const class RHIIndexBuffer& inBuffer) {};
	virtual void UnbindShader(const class RHIShader& inShader) {};
	virtual void UnbindUniformBuffer(const RHIUniformBuffer& inBuffer) {};

	virtual void UniformBufferUpdateData(RHIUniformBuffer& inBuffer, const void* inData, const size_t inDataSize) {};

	virtual eastl::shared_ptr<class RHIShader> CreateShaderFromSource(const eastl::string& inVertexSrc, const eastl::string& inPixelSrc, const VertexInputLayout& inInputLayout) { return nullptr; }
	virtual eastl::shared_ptr<class RHIShader> CreateShaderFromPath(const eastl::string& inVertexPath, const eastl::string& inPixelPath, const VertexInputLayout& inInputLayout) { return nullptr; }

	virtual eastl::shared_ptr<class RHITexture2D> CreateTexture2D() { return nullptr; }
	virtual void LoadTextureFromPath(RHITexture2D& inTexture, const eastl::string& inPath) {};

	virtual void SetViewportSize(const int32_t inWidth, const int32_t inHeight) {}
	virtual void ClearColor(const glm::vec4 inColor) {}

	virtual void DrawElements(const int32_t inElementsCount) {}

	virtual void SwapBuffers() {}

	virtual void ClearBuffers() {}

public:
	inline static class RHI* Instance = nullptr;
};