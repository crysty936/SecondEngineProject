#pragma once
#include "EASTL/shared_ptr.h"
#include "EASTL/string.h"
#include "EASTL/vector.h"
#include "Renderer/RenderingPrimitives.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"

class RHIBase
{
public:
	static void Init();
	static void Terminate();

public:
	virtual eastl::shared_ptr<class VertexBufferBase> CreateVertexBuffer(const class VertexBufferLayout& inLayout, const float* inVertices, const int32_t inCount) { return nullptr; }
	virtual eastl::shared_ptr<class VertexBufferBase> CreateVertexBuffer(const class VertexBufferLayout& inLayout, const eastl::vector<Vertex>& inVertices) { return nullptr; }

	virtual eastl::shared_ptr<class IndexBufferBase> CreateIndexBuffer(const uint32_t* inData, uint32_t inCount) { return nullptr; }

	virtual eastl::shared_ptr<class UniformBufferBase> CreateUniformBuffer(size_t inSize) { return nullptr; }

	virtual eastl::shared_ptr<class ShaderBase> CreateShaderFromSource(const eastl::string& inVertexSrc, const eastl::string& inPixelSrc) { return nullptr; }
	virtual eastl::shared_ptr<class ShaderBase> CreateShaderFromPath(const eastl::string& inVertexPath, const eastl::string& inPixelPath) { return nullptr; }

	virtual void SetViewportSize(const int32_t inWidth, const int32_t inHeight) {}
	virtual void ClearColor(const glm::vec4 inColor) {}

	virtual void SwapBuffers() {}

public:
	inline static class RHIBase* RHI = nullptr;
};