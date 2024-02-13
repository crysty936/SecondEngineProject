#include <assert.h>
#include "Renderer/TestRenderer.h"
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
#include "Math/SphericalHarmonics.h"
#include "Math/MathUtils.h"

TestRenderer::TestRenderer(const WindowProperties& inMainWindowProperties)
	: Renderer(inMainWindowProperties)
{
}

TestRenderer::~TestRenderer() = default;

static eastl::shared_ptr<RHIFrameBuffer> GBuffer = nullptr;
static eastl::shared_ptr<RHITexture2D> GBufferDepth = nullptr;
static eastl::shared_ptr<RHITexture2D> GBufferNormal = nullptr;
static eastl::shared_ptr<RHITexture2D> GBufferAlbedo = nullptr;
static eastl::shared_ptr<RHITexture2D> GBufferMetallicRoughness = nullptr;

//eastl::shared_ptr<VisualizeDepthQuad> VisualizeDepthUtil;
//eastl::shared_ptr<GBufferVisualizeQuad> VisualizeNormalsUtil;
//eastl::shared_ptr<GBufferVisualizeQuad> VisualizeAlbedoUtil;
//eastl::shared_ptr<GBufferVisualizeQuad> VisualizeRoughnessUtil;
//eastl::shared_ptr<DefaultPBRLightingModelQuad> DefaultPBRShadingModelQuad;

void TestRenderer::InitInternal()
{
	const WindowsWindow& currentWindow = GEngine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();

	GBuffer = RHI::Get()->CreateEmptyFrameBuffer();

	//GBufferDepth = RHI::Get()->CreateDepthMap(props.Width, props.Height);

	//RHI::Get()->AttachTextureToFramebufferDepth(*GBuffer, GBufferDepth);

	//GBufferNormal = RHI::Get()->CreateRenderTexture(props.Width, props.Height, ERHITexturePrecision::Float16, ERHITextureFilter::Nearest);
	//RHI::Get()->AttachTextureToFramebufferColor(*GBuffer, GBufferNormal);

	//GBufferAlbedo = RHI::Get()->CreateRenderTexture(props.Width, props.Height, ERHITexturePrecision::UnsignedByte, ERHITextureFilter::Nearest);
	//RHI::Get()->AttachTextureToFramebufferColor(*GBuffer, GBufferAlbedo);

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

	//VisualizeAlbedoUtil = SceneHelper::CreateObject<GBufferVisualizeQuad>("VisualizeAlbedoTex");
	//VisualizeAlbedoUtil->CreateCommand();
	//VisualizeAlbedoUtil->GetCommand().Material->ExternalTextures.push_back(GBufferAlbedo);
	//VisualizeAlbedoUtil->SetScale(glm::vec3(0.33f, 0.33f, 1.f));
	//VisualizeAlbedoUtil->SetRelativeLocation(glm::vec3(0.67f, 0.65f, 0.0f));

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

void TestRenderer::Draw()
{
	//ImGui::Begin("Renderer settings");

	//DisplaySettings();

	//SetBaseUniforms();
	//UpdateUniforms();

	//SetLightingConstants();

	//RHI::Instance->BindFrameBuffer(*GBuffer);
	//RHI::Get()->ClearBuffers();
	//RHI::Instance->ClearTexture(*GBufferDepth, glm::vec4(1.f, 1.f, 1.f, 1.f));

	////static glm::vec3 viewOrigin;
	////static bool bUpdateViewOrigin = true;
	////ImGui::Checkbox("TestUpdateViewOrigin", &bUpdateViewOrigin);
	////if (bUpdateViewOrigin)
	////{
	////	viewOrigin = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetAbsoluteTransform().Translation;
	////}
	////DrawDebugHelpers::DrawDebugPoint(viewOrigin);


	//DrawCommands(MainCommands);

	//DrawDecals(DecalCommands);

	//RHI::Instance->BindDefaultFrameBuffer();
	//RHI::Get()->ClearBuffers();

	//RHI::Get()->CopyFrameBufferDepth(GBuffer, nullptr);


	//RHI::Get()->SetDepthWrite(false);

	//DrawCommand(DefaultPBRShadingModelQuad->GetCommand());

	//// Draw debug primitives
	//DrawDebugManager::Draw();

	//static bool bViewGBuffer = false;
	//ImGui::Checkbox("View GBuffer", &bViewGBuffer);

	//if (bViewGBuffer)
	//{
	//	RHI::Get()->SetDepthTest(false);

	//	DrawCommand(VisualizeDepthUtil->GetCommand());

	//	VisualizeNormalsUtil->GetCommand().Material->ExternalTextures.clear();
	//	VisualizeNormalsUtil->GetCommand().Material->ExternalTextures.push_back(GBufferNormal);
	//	DrawCommand(VisualizeNormalsUtil->GetCommand());

	//	VisualizeAlbedoUtil->GetCommand().Material->ExternalTextures.clear();
	//	VisualizeAlbedoUtil->GetCommand().Material->ExternalTextures.push_back(GBufferAlbedo);
	//	DrawCommand(VisualizeAlbedoUtil->GetCommand());

	//	VisualizeRoughnessUtil->GetCommand().Material->ExternalTextures.clear();
	//	VisualizeRoughnessUtil->GetCommand().Material->ExternalTextures.push_back(GBufferMetallicRoughness);
	//	DrawCommand(VisualizeRoughnessUtil->GetCommand());
	//}

	////if (ImGui::Button("Do RenderDoc Capture"))
	////{
	////	RenderDocPlugin* renderDoc = GEngine->GetInternalPlugin<RenderDocPlugin>("RenderDocPlugin");

	////	if (renderDoc)
	////	{
	////		renderDoc->DoCapture();
	////	}
	////}

	//RHI::Get()->SetDepthTest(true);
	//RHI::Get()->SetDepthWrite(true);

	//ImGui::End();
}

void TestRenderer::Present()
{
	//RHI::Get()->SwapBuffers();
}

//void TestRenderer::UpdateUniforms()
//{
//	const glm::mat4 view = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetLookAt();
//	UniformsCache["view"] = view;
//}

void TestRenderer::AddCommand(const RenderCommand& inCommand)
{
	//MainCommands.push_back(inCommand);
}

void TestRenderer::AddCommands(eastl::vector<RenderCommand> inCommands)
{
	//#ifdef _DEBUG
	//	for (RenderCommand& command : inCommands)
	//	{
	//		ASSERT(command.DataContainer);
	//		ASSERT(command.Material);
	//	}
	//#endif
	//
	//	MainCommands.insert(MainCommands.end(), inCommands.begin(), inCommands.end());
}

bool TestRenderer::GetOrCreateContainer(const eastl::string& inInstanceName, OUT eastl::shared_ptr<MeshDataContainer>& outContainer)
{
	// 	LOG_INFO("Get or Create Render Container.");
	// 	ASSERT(!inInstanceName.empty());
	// 
	// 	using iterator = const eastl::unordered_map<eastl::string, eastl::shared_ptr<MeshDataContainer>>::iterator;
	// 	const iterator& containerIter = RenderDataContainerMap.find(inInstanceName);
	// 	const bool materialExists = containerIter != RenderDataContainerMap.end();
	// 
	// 	if (materialExists)
	// 	{
	// 		LOG_INFO("Container already exists, returning.");
	// 
	// 		outContainer = (*containerIter).second;
	// 
	// 		return true;
	// 	}
	// 
	// 	LOG_INFO("Container not present, creating new.");
	// 	eastl::shared_ptr<MeshDataContainer> newContainer = eastl::make_shared<MeshDataContainer>();
	// 	RenderDataContainerMap[inInstanceName] = newContainer;
	// 	outContainer = newContainer;

	return false;
}

eastl::string TestRenderer::GetMaterialsDirPrefix()
{
	return "Test";
}

void TestRenderer::AddDecalCommand(const RenderCommand& inCommand)
{
	//// TODO: Hack
	//if (inCommand.Material->bUsesSceneTextures)
	//{
	//	//inCommand.Material->ExternalTextures.push_back(GBufferColorSpec);
	//	//inCommand.Material->ExternalTextures.push_back(GBufferNormal);
	//	inCommand.Material->ExternalTextures.push_back(GBufferDepth);
	//}
	////

	//DecalCommands.push_back(inCommand);
}

//void TestRenderer::SetViewportSizeToMain()
//{
//	const WindowsWindow& currentWindow = GEngine->GetMainWindow();
//	const WindowProperties& props = currentWindow.GetProperties();
//	RHI::Get()->SetViewportSize(props.Width, props.Height);
//}
