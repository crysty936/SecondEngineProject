#include "Renderer/PathTracingRenderer.h"
#include <assert.h>
#include "Core/EngineUtils.h"
#include "Core/EngineCore.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"
#include "Entity/Entity.h"
#include "Renderer/Drawable/Drawable.h"
#include "EASTL/shared_ptr.h"
#include "Renderer/Material/RenderMaterial.h"
#include "Renderer/RHI/Resources/MeshDataContainer.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Core/SceneHelper.h"
#include "Renderer/Material/EngineMaterials/DepthMaterial.h"
#include "Core/WindowsPlatform.h"
#include "glm/gtc/integer.hpp"

#include "InputSystem/InputType.h"
#include "Window/WindowsWindow.h"
#include "InputSystem/InputSystem.h"

#include "Renderer/RHI/RHI.h"
#include "Renderer/RHI/Resources/RHIShader.h"
#include "Renderer/RHI/Resources/RHITexture.h"
#include "Drawable/ShapesUtils/BasicShapes.h"
#include "EASTL/stack.h"
#include "Material/EngineMaterials/RenderMaterial_Debug.h"
#include "DrawDebugHelpers.h"
#include "RenderUtils.h"
#include "Math/AABB.h"
#include "imgui.h"
#include "ShaderTypes.h"
#include "Utils/ImageLoading.h"


PathTracingRenderer::PathTracingRenderer(const WindowProperties& inMainWindowProperties)
	: Renderer(inMainWindowProperties)
{
}

PathTracingRenderer::~PathTracingRenderer() = default;
//
//static eastl::shared_ptr<RHIFrameBuffer> GBuffer = nullptr;
//static eastl::shared_ptr<RHITexture2D> GlobalRenderTexture = nullptr;
//static eastl::shared_ptr<RHITexture2D> GBufferDepth = nullptr;
//static eastl::shared_ptr<RHITexture2D> GBufferNormal = nullptr;
//static eastl::shared_ptr<RHITexture2D> GBufferAlbedo = nullptr;
//static eastl::shared_ptr<RHITexture2D> GBufferMetallicRoughness = nullptr;
//
//eastl::shared_ptr<VisualizeDepthQuad> VisualizeDepthUtil;
//eastl::shared_ptr<GBufferVisualizeQuad> VisualizeNormalsUtil;
//eastl::shared_ptr<GBufferVisualizeQuad> VisualizeAlbedoUtil;
//eastl::shared_ptr<GBufferVisualizeQuad> VisualizeRoughnessUtil;
//eastl::shared_ptr<DefaultPBRLightingModelQuad> DefaultPBRShadingModelQuad;

eastl::shared_ptr<FullScreenQuad> VisualizeQuad;
eastl::shared_ptr<RHITexture2D> RHITexture;


static uint32_t ConvertToRGBA(const glm::vec4& color)
{
	uint8_t r = (uint8_t)(color.r * 255.0f);
	uint8_t g = (uint8_t)(color.g * 255.0f);
	uint8_t b = (uint8_t)(color.b * 255.0f);
	uint8_t a = (uint8_t)(color.a * 255.0f);

	uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
	return result;
}

uint32_t* FinalImageData;

void PathTracingRenderer::InitInternal()
{
	const WindowsWindow& currentWindow = GEngine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();

	VisualizeQuad = SceneHelper::CreateObject<FullScreenQuad>("Quad");
	VisualizeQuad->CreateCommand();
	RHITexture = RHI::Get()->CreateTexture2D(props.Width, props.Height);

	VisualizeQuad->GetCommand().Material->ExternalTextures.push_back(RHITexture);
	FinalImageData = new uint32_t[props.Width * props.Height];

	const uint32_t color = ConvertToRGBA(glm::vec4(1.f, 0.f, 0.f, 1.f));
	const uint32_t color2 = ConvertToRGBA(glm::vec4(0.f, 0.f, 1.f, 1.f));
	//const uint32_t color = 0xFFFFFFFF;

	for (int32_t i = 0; i < props.Height; ++i)
	{
		for (int32_t j = 0; j < props.Width; ++j)
		{
			FinalImageData[(props.Width * i) + j] = i % 50 == 0 ? color : color2;
		}
	}

	//GBuffer = RHI::Get()->CreateEmptyFrameBuffer();
	//GlobalRenderTexture = RHI::Get()->CreateRenderTexture(props.Width, props.Height, ERHITexturePrecision::Float16, ERHITextureFilter::Nearest);
	////RHI::Get()->AttachTextureToFramebufferColor(*GBuffer, *GlobalRenderTexture);

	//GBufferDepth = RHI::Get()->CreateDepthMap(props.Width, props.Height);

	//RHI::Get()->AttachTextureToFramebufferDepth(*GBuffer, GBufferDepth);

	//GBufferNormal = RHI::Get()->CreateRenderTexture(props.Width, props.Height, ERHITexturePrecision::Float16, ERHITextureFilter::Nearest);
	//RHI::Get()->AttachTextureToFramebufferColor(*GBuffer, GBufferNormal);

 //	GBufferAlbedo = RHI::Get()->CreateRenderTexture(props.Width, props.Height, ERHITexturePrecision::UnsignedByte, ERHITextureFilter::Nearest);
 //	RHI::Get()->AttachTextureToFramebufferColor(*GBuffer, GBufferAlbedo);

	//GBufferMetallicRoughness = RHI::Get()->CreateRenderTexture(props.Width, props.Height, ERHITexturePrecision::UnsignedByte, ERHITextureFilter::Nearest);
	//RHI::Get()->AttachTextureToFramebufferColor(*GBuffer, GBufferMetallicRoughness);

	//VisualizeDepthUtil = SceneHelper::CreateObject<VisualizeDepthQuad>("VisualizeDepth");
	//VisualizeDepthUtil->CreateCommand();
	//VisualizeDepthUtil->GetCommand().Material->ExternalTextures.push_back(GBufferDepth);
	//VisualizeDepthUtil->SetScale(glm::vec3(0.33f, 0.33f, 1.f));
	//VisualizeDepthUtil->SetRelativeLocation(glm::vec3(-0.67, 0.65f, 0.0f));


	//VisualizeNormalsUtil = SceneHelper::CreateObject<GBufferVisualizeQuad>("VisualizeNormalsTex");
	//VisualizeNormalsUtil->CreateCommand();
	//VisualizeNormalsUtil->GetCommand().Material->ExternalTextures.push_back(GBufferNormal);
	//VisualizeNormalsUtil->SetScale(glm::vec3(0.33f, 0.33f, 1.f));
	//VisualizeNormalsUtil->SetRelativeLocation(glm::vec3(0.f, 0.65f, 0.0f));

 //	VisualizeAlbedoUtil = SceneHelper::CreateObject<GBufferVisualizeQuad>("VisualizeAlbedoTex");
 //	VisualizeAlbedoUtil->CreateCommand();
 //	VisualizeAlbedoUtil->GetCommand().Material->ExternalTextures.push_back(GBufferAlbedo);
 //	VisualizeAlbedoUtil->SetScale(glm::vec3(0.33f, 0.33f, 1.f));
 //	VisualizeAlbedoUtil->SetRelativeLocation(glm::vec3(0.67f, 0.65f, 0.0f));

	//VisualizeRoughnessUtil = SceneHelper::CreateObject<GBufferVisualizeQuad>("VisualizeRoughnessTex");
	//VisualizeRoughnessUtil->CreateCommand();
	//VisualizeRoughnessUtil->GetCommand().Material->ExternalTextures.push_back(GBufferAlbedo);
	//VisualizeRoughnessUtil->SetScale(glm::vec3(0.33f, 0.33f, 1.f));
	//VisualizeRoughnessUtil->SetRelativeLocation(glm::vec3(0.67f, -0.15f, 0.0f));

	//DefaultPBRShadingModelQuad = SceneHelper::CreateObject<DefaultPBRLightingModelQuad>("DefaultLightingModelQuad");
	//DefaultPBRShadingModelQuad->CreateCommand();
	//DefaultPBRShadingModelQuad->GetCommand().Material->ExternalTextures.push_back(GBufferAlbedo);
	//DefaultPBRShadingModelQuad->GetCommand().Material->ExternalTextures.push_back(GBufferNormal);
	//DefaultPBRShadingModelQuad->GetCommand().Material->ExternalTextures.push_back(GBufferMetallicRoughness);
	//DefaultPBRShadingModelQuad->GetCommand().Material->ExternalTextures.push_back(GBufferDepth);
	//DefaultPBRShadingModelQuad->CreateCommand();


}

void PathTracingRenderer::Draw()
{
	ImGui::Begin("Renderer settings");

	const WindowsWindow& currentWindow = GEngine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();

	ImageData data;
	data.NrChannels = 4;
	data.RawData = FinalImageData;
	data.Width = props.Width;
	data.Height = props.Height;

	RHI::Get()->UploadDataToTexture(*RHITexture, data, false);

	RHI::Instance->BindDefaultFrameBuffer();
	RHI::Get()->ClearBuffers();

	DrawCommand(VisualizeQuad->GetCommand());

	ImGui::End();
}

void PathTracingRenderer::DrawCommand(const RenderCommand& inCommand)
{
	const bool parentValid = !inCommand.Parent.expired();
	if (!ENSURE(parentValid))
	{
		return;
	}

	const eastl::shared_ptr<const DrawableObject> parent = inCommand.Parent.lock();
	const eastl::shared_ptr<RenderMaterial> material = inCommand.Material;
	const eastl::shared_ptr<MeshDataContainer>& dataContainer = inCommand.DataContainer;

	if (!parent->IsVisible() || !material)
	{
		return;
	}

	RHI::Get()->BindVertexBuffer(*(dataContainer->VBuffer));

	// Additional vertex data buffers
	for (const eastl::shared_ptr<RHIVertexBuffer>& additionalBuffer : dataContainer->AdditionalBuffers)
	{
		constexpr bool bindIndexBuffer = false;
		RHI::Get()->BindVertexBuffer(*(additionalBuffer), bindIndexBuffer);
	}

	RHI::Get()->BindShader(*(material->Shader));

	material->ResetUniforms();

	//UniformsCache["model"] = parent->GetModelMatrix();
	//UniformsCache["ObjPos"] = parent->GetAbsoluteTransform().Translation;

	{
		int texNr = 0;
		for (const eastl::shared_ptr<RHITexture2D>& tex : material->OwnedTextures)
		{
			RHI::Get()->BindTexture2D(*tex, texNr);
			++texNr;
		}

		for (const eastl::weak_ptr<RHITexture2D>& tex : material->ExternalTextures)
		{
			if (tex.expired())
			{
				continue;
			}

			eastl::shared_ptr<RHITexture2D>& sharedTex = tex.lock();

			RHI::Get()->BindTexture2D(*sharedTex, texNr);
			++texNr;
		}
	}

	const uint32_t indicesCount = dataContainer->VBuffer->GetIndicesCount();

	//parent->UpdateCustomUniforms(UniformsCache);
	//material->SetUniformsValue(UniformsCache);
	material->BindBuffers();

	switch (inCommand.DrawType)
	{
	case EDrawType::DrawElements:
	{
		RHI::Get()->DrawElements(indicesCount);

		break;
	}
	case EDrawType::DrawArrays:
	{
		//glDrawArrays(GL_TRIANGLES, 0, indicesCount);
		break;
	}
	case EDrawType::DrawInstanced:
	{
		RHI::Get()->DrawInstanced(indicesCount, inCommand.InstancesCount);
		break;
	}
	}

	// Additional vertex data buffers
//  	for (const eastl::shared_ptr<RHIVertexBuffer>& additionalBuffer : dataContainer->AdditionalBuffers)
//  	{
//  		constexpr bool unbindIndexBuffer = false;
//  		RHI::Instance->UnbindVertexBuffer(*(additionalBuffer), unbindIndexBuffer);
//  	}

	RHI::Get()->UnbindVertexBuffer(*(dataContainer->VBuffer));

	{
		int texNr = 0;
		for (const eastl::shared_ptr<RHITexture2D>& tex : material->OwnedTextures)
		{
			RHI::Get()->UnbindTexture2D(*tex, texNr);
			++texNr;
		}

		for (const eastl::weak_ptr<RHITexture2D>& tex : material->ExternalTextures)
		{
			if (tex.expired())
			{
				continue;
			}

			eastl::shared_ptr<RHITexture2D>& sharedTex = tex.lock();
			RHI::Get()->BindTexture2D(*sharedTex, texNr);
			++texNr;
		}
	}

	material->UnbindBuffers();
	RHI::Get()->UnbindShader(*(material->Shader));
}

void PathTracingRenderer::Present()
{
	RHI::Get()->SwapBuffers();
}

void PathTracingRenderer::AddCommand(const RenderCommand & inCommand)
{
	MainCommands.push_back(inCommand);
}

void PathTracingRenderer::AddCommands(eastl::vector<RenderCommand> inCommands)
{
#ifdef _DEBUG
	for (RenderCommand& command : inCommands)
	{
		ASSERT(command.DataContainer);
		ASSERT(command.Material);
	}
#endif

	MainCommands.insert(MainCommands.end(), inCommands.begin(), inCommands.end());
}

bool PathTracingRenderer::GetOrCreateContainer(const eastl::string& inInstanceName, OUT eastl::shared_ptr<MeshDataContainer>& outContainer)
{
	ASSERT(!inInstanceName.empty());

	using iterator = const eastl::unordered_map<eastl::string, eastl::shared_ptr<MeshDataContainer>>::iterator;
	const iterator& containerIter = RenderDataContainerMap.find(inInstanceName);
	const bool materialExists = containerIter != RenderDataContainerMap.end();

	if (materialExists)
	{
		outContainer = (*containerIter).second;

		return true;
	}

	eastl::shared_ptr<MeshDataContainer> newContainer = eastl::make_shared<MeshDataContainer>();
	RenderDataContainerMap[inInstanceName] = newContainer;
	outContainer = newContainer;

	return false;
}

eastl::string PathTracingRenderer::GetMaterialsDirPrefix()
{
	return "Deferred";
}

void PathTracingRenderer::AddDecalCommand(const RenderCommand& inCommand)
{
	//// TODO: Hack
	//if (inCommand.Material->bUsesSceneTextures)
	//{
	//	inCommand.Material->ExternalTextures.push_back(GBufferDepth);
	//}
	////

	DecalCommands.push_back(inCommand);
}

void PathTracingRenderer::SetViewportSizeToMain()
{
	const WindowsWindow& currentWindow = GEngine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();
	RHI::Get()->SetViewportSize(props.Width, props.Height);
}
