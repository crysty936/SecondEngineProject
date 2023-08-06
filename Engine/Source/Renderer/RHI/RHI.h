#pragma once
#include "EASTL/shared_ptr.h"
#include "EASTL/string.h"
#include "EASTL/vector.h"
#include "Renderer/RenderingPrimitives.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "Renderer/DrawType.h"
#include "Resources/RHITexture.h"

enum EShaderType : uint8_t
{
	Sh_Vertex = 1 << 0,
	Sh_Fragment = 1 << 1,
	Sh_Geometry = 1 << 2,

	Sh_Universal = Sh_Vertex | Sh_Fragment | Sh_Geometry
};

enum class ERasterizerState
{
	CW,
	CCW
};

enum class EDepthOp
{
	Never,
	Less,
	Equal,
	LessOrEqual,
	Greater,
	NotEqual,
	GreaterOrEqual,
	Always
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
	virtual eastl::shared_ptr<class RHIVertexBuffer> CreateVertexBuffer(const class VertexInputLayout& inLayout, const void* inData, const size_t inSize, eastl::shared_ptr<class RHIIndexBuffer> inIndexBuffer = nullptr) { return nullptr; }

	virtual void ClearVertexBuffer(class RHIVertexBuffer& inBuffer) {}
	virtual void UpdateVertexBufferData(class RHIVertexBuffer& inBuffer, const void* inData, const size_t inSize) {}

	virtual eastl::shared_ptr<class RHIIndexBuffer> CreateIndexBuffer(const uint32_t* inData, uint32_t inCount) { return nullptr; }

	virtual eastl::shared_ptr<class RHIUniformBuffer> CreateUniformBuffer(size_t inSize) { return nullptr; }

	virtual void BindVertexBuffer(const class RHIVertexBuffer& inBuffer, const bool inBindIndexBuffer = true) {};
	virtual void BindIndexBuffer(const class RHIIndexBuffer& inBuffer) {};
	virtual void BindShader(const class RHIShader& inShader) {};
	virtual void BindUniformBuffer(const RHIUniformBuffer& inBuffer) {};
	virtual void BindTexture2D(const class RHITexture2D& inTex, const int32_t inTexId) {};
	virtual void BindFrameBuffer(const class RHIFrameBuffer& inFrameBuffer) {};
	virtual void BindDefaultFrameBuffer() {};

	virtual void UnbindVertexBuffer(const class RHIVertexBuffer& inBuffer, const bool inUnbindIndexBuffer = true) {};
	virtual void UnbindIndexBuffer(const class RHIIndexBuffer& inBuffer) {};
	virtual void UnbindShader(const class RHIShader& inShader) {};
	virtual void UnbindUniformBuffer(const RHIUniformBuffer& inBuffer) {};
	virtual void UnbindTexture2D(const class RHITexture2D& inTex, const int32_t inTexId) {};
	virtual void UnbindFrameBuffer(const class RHIFrameBuffer& inFrameBuffer) {};

	virtual void UniformBufferUpdateData(class RHIUniformBuffer& inBuffer, const void* inData, const size_t inDataSize, const int32_t inBufferNr) {};

	/** Attach a texture to the color ouput of a framebuffer */
	virtual void AttachTextureToFramebufferColor(class RHIFrameBuffer& inFrameBuffer, eastl::shared_ptr<class RHITexture2D>& inTex) {}

	/** Attach a texture to the depth output of a framebuffer. Preferrably a texture with only depth component */
	virtual void AttachTextureToFramebufferDepth(class RHIFrameBuffer& inFrameBuffer, eastl::shared_ptr<class RHITexture2D>& inTex) {}

	virtual void ClearTexture(const RHITexture2D& inTexture, const glm::vec4& inColor) {}

	virtual eastl::shared_ptr<class RHIShader> CreateShaderFromSource(const eastl::vector<ShaderSourceInput> inShaderSources, const VertexInputLayout& inInputLayout, const eastl::string& inVSName = "VS", const eastl::string& inPSName = "PS") { return nullptr; }

	virtual eastl::shared_ptr<class RHIShader> CreateShaderFromPath(const eastl::vector<ShaderSourceInput> inPathShaderSources, const VertexInputLayout& inInputLayout) { return nullptr; }

	virtual eastl::shared_ptr<class RHITexture2D> CreateAndLoadTexture2D(const eastl::string& inDataPath, const bool inSRGB) { return nullptr; }

	/**  A frame buffer that already has Depth Stencil attachments, can be used with texture color attachment */
	virtual eastl::shared_ptr<class RHIFrameBuffer> CreateDepthStencilFrameBuffer() { return nullptr; }

	virtual eastl::shared_ptr<class RHIFrameBuffer> CreateEmptyFrameBuffer() { return nullptr; }

	/** Render texture with all channels */
	virtual eastl::shared_ptr<class RHITexture2D> CreateRenderTexture(const int32_t inWidth, const int32_t inHeight, const ERHITexturePrecision inPrecision = ERHITexturePrecision::UnsignedByte, const ERHITextureFilter inFilter = ERHITextureFilter::Linear) { return nullptr; }

	virtual void CopyRenderTexture(class RHITexture2D& inSrc, class RHITexture2D& inTrg) {}
	virtual void CopyRenderTextureRegion(class RHITexture2D& inSrc, class RHITexture2D& inTrg, const int32_t inOffsetX, const int32_t inOffsetY, const int32_t inRegionWidth, const int32_t inRegionHeight) {}

	/** Texture with only one channel for depth */
	virtual eastl::shared_ptr<class RHITexture2D> CreateDepthMap(const int32_t inWidth, const int32_t inHeight) { return nullptr; }
	virtual eastl::shared_ptr<class RHITexture2D> CreateArrayDepthMap(const int32_t inDepthMapWidth, const int32_t inDepthMapHeight, const int32_t inSize) { return nullptr; }

	virtual void SetViewportSize(const int32_t inWidth, const int32_t inHeight) {}

	// Specify clear color
	virtual void SetClearColor(const glm::vec4 inColor) {}

	virtual void DrawElements(const int32_t inElementsCount) {}
	virtual void DrawInstanced(const int32_t inElementsCount, const int32_t inInstancesCount) {}
	virtual void DrawPoints(const int32_t inCount) {}

	virtual void SwapBuffers() {}

	virtual void ClearBuffers() {}

	virtual void SetFaceCullMode(const EFaceCullMode inMode) {}

	virtual void PrepareProjectionForRendering(glm::mat4& inProj) {}

	virtual void ReadBufferData(const class RHIBufferBase& inBuffer, const size_t inOffset, const size_t inSize, void* outData) {}

	virtual void SetDepthWrite(const bool inValue) {}
	virtual void SetDepthTest(const bool inValue) {}
	virtual void SetDepthOp(EDepthOp inValue) {}

	virtual void SetRasterizerState(const ERasterizerState inState) {}
	virtual void SetCullState(const bool inValue) {};

	virtual void TestStencilBufferStuff(class RHIFrameBuffer& inFrameBuffer) {}

	// Used for pre-stencil
	virtual eastl::shared_ptr<class RHIShader> GetVertexOnlyShader(const class RenderMaterial& inFullMaterial) { return nullptr; }

	// ImGui
	virtual void ImGuiInit() {}
	virtual void ImGuiBeginFrame() {}
	virtual void ImGuiRenderDrawData() {}

public:
	inline static class RHI* Instance = nullptr;
	static class RHI* Get() { return Instance; }
};