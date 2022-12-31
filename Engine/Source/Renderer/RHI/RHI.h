#pragma once
#include "EASTL/shared_ptr.h"
#include "EASTL/string.h"
#include "EASTL/vector.h"
#include "Renderer/RenderingPrimitives.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/ext/matrix_float4x4.hpp"

enum class EShaderType
{
	Vertex,
	Fragment,
	Geometry
};

struct ShaderSourceInput
{
	/** Path or code */
	eastl::string ShaderSource;

	/** How it should be compiled */
	EShaderType ShaderType;
};

class RHI
{
public:
	static void Init();
	static void Terminate();

public:
	virtual eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexInputLayout& inLayout, const float* inVertices, const int32_t inCount, eastl::shared_ptr<class RHIIndexBuffer> inIndexBuffer = nullptr) { return nullptr; }
	virtual eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexInputLayout& inLayout, const eastl::vector<Vertex>& inVertices, eastl::shared_ptr<class RHIIndexBuffer> inIndexBuffer = nullptr) { return nullptr; }
	virtual eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexInputLayout& inLayout, const void* inData, const int32_t inSize, eastl::shared_ptr<class RHIIndexBuffer> inIndexBuffer = nullptr) { return nullptr; }

	virtual eastl::shared_ptr<class RHIIndexBuffer> CreateIndexBuffer(const uint32_t* inData, uint32_t inCount) { return nullptr; }

	virtual eastl::shared_ptr<class RHIUniformBuffer> CreateUniformBuffer(size_t inSize) { return nullptr; }

	virtual void BindVertexBuffer(const class RHIVertexBuffer& inBuffer, const bool inBindIndexBuffer = true) {};
	virtual void BindIndexBuffer(const class RHIIndexBuffer& inBuffer) {};
	virtual void BindShader(const class RHIShader& inShader) {};
	virtual void BindUniformBuffer(const RHIUniformBuffer& inBuffer) {};
	virtual void BindTexture2D(const class RHITexture2D& inTex, const int32_t inTexId) {};

	virtual void UnbindVertexBuffer(const class RHIVertexBuffer& inBuffer, const bool inUnbindIndexBuffer = true) {};
	virtual void UnbindIndexBuffer(const class RHIIndexBuffer& inBuffer) {};
	virtual void UnbindShader(const class RHIShader& inShader) {};
	virtual void UnbindUniformBuffer(const RHIUniformBuffer& inBuffer) {};
	virtual void UnbindTexture2D(const class RHITexture2D& inTex, const int32_t inTexId) {};

	virtual void UniformBufferUpdateData(RHIUniformBuffer& inBuffer, const void* inData, const size_t inDataSize, const int32_t inBufferNr) {};

	virtual eastl::shared_ptr<class RHIShader> CreateShaderFromSource(const eastl::vector<ShaderSourceInput> inShaderSources, const VertexInputLayout& inInputLayout, const eastl::string& inVSName = "VS", const eastl::string& inPSName = "PS") { return nullptr; }
	virtual eastl::shared_ptr<class RHIShader> CreateShaderFromPath(const eastl::vector<ShaderSourceInput> inPathShaderSources, const VertexInputLayout& inInputLayout) { return nullptr; }

	virtual eastl::shared_ptr<class RHITexture2D> CreateTexture2D(const eastl::string& inDataPath) { return nullptr; }

	virtual void SetViewportSize(const int32_t inWidth, const int32_t inHeight) {}
	virtual void ClearColor(const glm::vec4 inColor) {}

	virtual void DrawElements(const int32_t inElementsCount) {}
	virtual void DrawInstanced(const int32_t inElementsCount, const int32_t inInstancesCount) {}

	virtual void SwapBuffers() {}

	virtual void ClearBuffers() {}

	virtual void PrepareProjectionForRendering(glm::mat4& inProj) {}

public:
	inline static class RHI* Instance = nullptr;
};