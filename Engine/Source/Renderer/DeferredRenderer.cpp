#include <assert.h>
#include "Renderer/DeferredRenderer.h"
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

static std::mutex RenderCommandsMutex;
static std::mutex GetVAOMutex;

DeferredRenderer::DeferredRenderer(const WindowProperties& inMainWindowProperties)
	: Renderer(inMainWindowProperties)
{
}

DeferredRenderer::~DeferredRenderer() = default;

static eastl::shared_ptr<RHIFrameBuffer> GBuffer = nullptr;
static eastl::shared_ptr<RHITexture2D> GlobalRenderTexture = nullptr;
static eastl::shared_ptr<RHITexture2D> GBufferDepth = nullptr;
static eastl::shared_ptr<RHITexture2D> GBufferNormal = nullptr;
static eastl::shared_ptr<RHITexture2D> GBufferColorSpec = nullptr;

eastl::shared_ptr<VisualizeDepthQuad> VisualizeDepthUtil;
eastl::shared_ptr<GBufferVisualizeQuad> VisualizeNormalsUtil;
eastl::shared_ptr<GBufferVisualizeQuad> VisualizeAlbedoUtil;
eastl::shared_ptr<DefaultLightingModelQuad> DefaultModelQuad;

void DeferredRenderer::InitInternal()
{
	const WindowsWindow& currentWindow = Engine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();

	GBuffer = RHI::Get()->CreateEmptyFrameBuffer();
	GlobalRenderTexture = RHI::Get()->CreateRenderTexture(props.Width, props.Height, ERHITexturePrecision::Float16, ERHITextureFilter::Nearest);
	//RHI::Get()->AttachTextureToFramebufferColor(*GBuffer, *GlobalRenderTexture);

	GBufferDepth = RHI::Get()->CreateDepthMap(props.Width, props.Height);

	RHI::Get()->AttachTextureToFramebufferDepth(*GBuffer, GBufferDepth);

	GBufferNormal = RHI::Get()->CreateRenderTexture(props.Width, props.Height, ERHITexturePrecision::Float16, ERHITextureFilter::Nearest);
	RHI::Get()->AttachTextureToFramebufferColor(*GBuffer, GBufferNormal);

 	GBufferColorSpec = RHI::Get()->CreateRenderTexture(props.Width, props.Height, ERHITexturePrecision::UnsignedByte, ERHITextureFilter::Nearest);
 	RHI::Get()->AttachTextureToFramebufferColor(*GBuffer, GBufferColorSpec);

	VisualizeDepthUtil = SceneHelper::CreateObject<VisualizeDepthQuad>("VisualizeDepth");
	VisualizeDepthUtil->CreateCommand();
	VisualizeDepthUtil->GetCommand().Material->ExternalTextures.push_back(GBufferDepth);
	VisualizeDepthUtil->SetScale(glm::vec3(0.33f, 0.33f, 1.f));
	VisualizeDepthUtil->SetRelativeLocation(glm::vec3(-0.67, 0.65f, 0.0f));


	VisualizeNormalsUtil = SceneHelper::CreateObject<GBufferVisualizeQuad>("VisualizeNormalsTex");
	VisualizeNormalsUtil->CreateCommand();
	VisualizeNormalsUtil->GetCommand().Material->ExternalTextures.push_back(GBufferNormal);
	VisualizeNormalsUtil->SetScale(glm::vec3(0.33f, 0.33f, 1.f));
	VisualizeNormalsUtil->SetRelativeLocation(glm::vec3(0.f, 0.65f, 0.0f));

 	VisualizeAlbedoUtil = SceneHelper::CreateObject<GBufferVisualizeQuad>("VisualizeAlbedoTex");
 	VisualizeAlbedoUtil->CreateCommand();
 	VisualizeAlbedoUtil->GetCommand().Material->ExternalTextures.push_back(GBufferColorSpec);
 	VisualizeAlbedoUtil->SetScale(glm::vec3(0.33f, 0.33f, 1.f));
 	VisualizeAlbedoUtil->SetRelativeLocation(glm::vec3(0.67f, 0.65f, 0.0f));

	DefaultModelQuad = SceneHelper::CreateObject<DefaultLightingModelQuad>("DefaultLightingModelQuad");
	DefaultModelQuad->CreateCommand();
	DefaultModelQuad->GetCommand().Material->ExternalTextures.push_back(GBufferColorSpec);
	DefaultModelQuad->GetCommand().Material->ExternalTextures.push_back(GBufferNormal);
	DefaultModelQuad->GetCommand().Material->ExternalTextures.push_back(GBufferDepth);
	DefaultModelQuad->CreateCommand();
}

void DeferredRenderer::Draw()
{
	ImGui::Begin("Renderer settings");

	SetBaseUniforms();
	UpdateUniforms();

	SetLightingConstants();


 	RHI::Instance->BindFrameBuffer(*GBuffer);
	RHI::Get()->ClearBuffers();
	RHI::Instance->ClearTexture(*GBufferDepth, glm::vec4(1.f, 1.f, 1.f, 1.f));


	// Clear default framebuffer buffers
	//RHI::Instance->BindDefaultFrameBuffer();
	//RHI::Get()->ClearBuffers();

    DrawCommands(MainCommands);

	RHI::Get()->SetDepthWrite(false);
	DrawCommands(PostProcessCommands);
	RHI::Get()->SetDepthWrite(true);

	RHI::Instance->BindDefaultFrameBuffer();
	RHI::Get()->ClearBuffers();


	DrawCommand(DefaultModelQuad->GetCommand());
	
	RHI::Get()->SetDepthTest(false);

 	DrawCommand(VisualizeDepthUtil->GetCommand());

 	//RHI::Instance->CopyRenderTextureRegion(*GBufferNormal, *GlobalRenderTexture, 300, 100, 640, 480);
 
 	VisualizeNormalsUtil->GetCommand().Material->ExternalTextures.clear();
 	VisualizeNormalsUtil->GetCommand().Material->ExternalTextures.push_back(GBufferNormal);
 	DrawCommand(VisualizeNormalsUtil->GetCommand());
 
  	VisualizeAlbedoUtil->GetCommand().Material->ExternalTextures.clear();
  	VisualizeAlbedoUtil->GetCommand().Material->ExternalTextures.push_back(GBufferColorSpec);
  	DrawCommand(VisualizeAlbedoUtil->GetCommand());



	// Draw debug primitives
	DrawDebugManager::Draw();

	RHI::Get()->SetDepthTest(true);

	ImGui::End();
}

void DeferredRenderer::Present()
{
	RHI::Get()->SwapBuffers();
}

void DeferredRenderer::SetLightingConstants()
{
	const eastl::vector<eastl::shared_ptr<LightSource>>& lights = SceneManager::Get().GetCurrentScene().GetLights();

	eastl::vector<eastl::shared_ptr<LightSource>> dirLights;
	eastl::vector<eastl::shared_ptr<LightSource>> pointLights;

	for (const eastl::shared_ptr<LightSource>& light : lights)
	{
		switch (light->LData.Type)
		{
		case ELightType::Directional:
		{
			dirLights.push_back(light);
			break;
		}
		case ELightType::Point:
		{
			pointLights.push_back(light);
			break;
		}
		}
	}

	ASSERT(dirLights.size() <= 1);

	////////////////////////////////////////////////////////////////////////////
	//// ImGui
	////////////////////////////////////////////////////////////////////////////

	ImGui::SeparatorText("Lighting");

	static bool bUpdateViewPosDir = true;
	ImGui::Checkbox("Update View Pos Dir", &bUpdateViewPosDir);

	//ImGui::Checkbox("Visualize Normals", &bNormalVisualizeMode);
	//UniformsCache["bNormalVisualizeMode"] = bNormalVisualizeMode ? 1 : 0;

	//ImGui::Checkbox("Use Normal Mapping", &bUseNormalMapping);
	//UniformsCache["bUseNormalMapping"] = bUseNormalMapping ? 1 : 0;

	//static float exposure = 1.f;
	//ImGui::DragFloat("Exposure", &exposure, 0.01f, 0.f, 10.f);
	//UniformsCache["Exposure"] = exposure;

	////////////////////////////////////////////////////////////////////////////
	////
	////////////////////////////////////////////////////////////////////////////

	const glm::vec3 cameraPos = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetAbsoluteTransform().Translation;

	//constexpr glm::vec3 forward = glm::vec3(0.f, 0.f, 1.f);
	//const glm::vec3 cameraForward = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetAbsoluteTransform().Rotation * forward;

	if (bUpdateViewPosDir)
	{
		UniformsCache["ViewPos"] = cameraPos;
		//UniformsCache["ViewDir"] = cameraForward;
	}

	const bool useDirLight = dirLights.size() > 0;
	UniformsCache["bUseDirLight"] = (int32_t)useDirLight;

	if (useDirLight)
	{
		const eastl::shared_ptr<LightSource>& dirLight = dirLights[0];

		const glm::vec3 dir = dirLight->GetAbsoluteTransform().Rotation * glm::vec3(0.f, 0.f, 1.f);
		UniformsCache["DirectionalLightDirection"] = glm::normalize(dir);
	}
	else
	{
		UniformsCache["DirectionalLightDirection"] = glm::vec3(0.f, 0.f, 0.f);
	}


	//eastl::vector<SPointLight> shaderPointLightData;
	//for (const eastl::shared_ptr<LightSource>& light : pointLights)
	//{
	//	SPointLight pointLight;

	//	const Transform& lightTransf = light->GetAbsoluteTransform();
	//	pointLight.position = glm::vec4(lightTransf.Translation.x, lightTransf.Translation.y, lightTransf.Translation.z, 0.f);

	//	const PointLightData& pointData = light->LData.TypeData.PointData;
	//	
	//	pointLight.linear = pointData.Linear;
	//	pointLight.quadratic = pointData.Quadratic;
	//	pointLight.color = glm::vec4(light->LData.Color.x, light->LData.Color.y, light->LData.Color.z, 0.f);

	//	shaderPointLightData.push_back(pointLight);
	//}

	//UniformsCache["NumPointLights"] =  static_cast<int32_t>(shaderPointLightData.size());
	//UniformsCache["PointLights"] = shaderPointLightData;
}

void DeferredRenderer::UpdateUniforms()
{
 	const glm::mat4 view = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetLookAt();
 	UniformsCache["view"] = view;
}

void DeferredRenderer::DrawCommands(const eastl::vector<RenderCommand>& inCommands)
{
	// TODO: Fix this
	if (CurrentDrawMode & EDrawMode::Default)
	{
		const int32_t builtInPassesCount = glm::log2((int)EDrawMode::Count);
		for (int i = 0; i < builtInPassesCount; ++i)
		{
			const EDrawMode::Type currentMode = static_cast<EDrawMode::Type>(1 << i);
			SetDrawMode(currentMode);

			for (const RenderCommand& renderCommand : inCommands)
			{
				if (renderCommand.DrawPasses & currentMode)
				{
					DrawCommand(renderCommand);
				}
			}

			SetDrawMode(EDrawMode::Default);
		}
	}
	else
	{
		for (const RenderCommand& renderCommand : inCommands)
		{
			if (CurrentDrawMode & EDrawMode::DEPTH && !renderCommand.Material->bCastShadow)
			{
				continue;
			}

			DrawCommand(renderCommand);
		}
	}
}

void DeferredRenderer::DrawCommand(const RenderCommand& inCommand)
{
	const bool parentValid = !inCommand.Parent.expired();
	if (!ENSURE(parentValid))
	{
		return;
	}

	const eastl::shared_ptr<const DrawableObject> parent = inCommand.Parent.lock();
	const eastl::shared_ptr<RenderMaterial> material = GetMaterial(inCommand);
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

	UniformsCache["model"] = parent->GetModelMatrix();
	UniformsCache["ObjPos"] = parent->GetAbsoluteTransform().Translation;

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

	parent->UpdateCustomUniforms(UniformsCache);
	material->SetUniformsValue(UniformsCache);
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

eastl::shared_ptr<RenderMaterial> DeferredRenderer::GetMaterial(const RenderCommand& inCommand) const
{
	switch (CurrentDrawMode)
	{
	case EDrawMode::Default:
	{
		return inCommand.Material;
	}
	case EDrawMode::DEPTH:
	{
		MaterialsManager& matManager = MaterialsManager::Get();
		eastl::shared_ptr<RenderMaterial> depthMaterial;


		///!! Do shader features/variants and variables with defines, example:
		// #define SHADOW_ENABLED
		// #define NUM_CASCADES
		// #define CASCADED_SHADOWS

		// Or some buffer uniforms could be based on defines
		
		// if shadow_enabled then do shadow mapping based on if Cascaded_shadows or other is defined
		// similarly, use NUM_CASCADES or define a default if that is not defined(or Assert to signal that it's missing)
		// This means that those shader variants need to be compiled and cached first and then used, they can't be compiled any time a variable is changed

		switch (inCommand.DrawType)
		{
		case EDrawType::DrawElements:
		{
			bool materialExists = false;
			depthMaterial = matManager.GetOrAddMaterial<DepthMaterial>("depth_material", materialExists);

			if (!materialExists)
			{
				VertexInputLayout inputLayout;
				inputLayout.Push<float>(3, VertexInputType::Position);
				inputLayout.Push<float>(3, VertexInputType::Normal);
				inputLayout.Push<float>(2, VertexInputType::TexCoords);

				eastl::vector<ShaderSourceInput> shaders = {
				//{ "VS-Pos-Normal-UV_Instanced_Depth", EShaderType::Vertex },
				{ "VS_Pos-Normal-UV_Depth_Cascaded", EShaderType::Vertex },
				{ "GS_Depth_Cascaded", EShaderType::Geometry },
				{ "PS_Empty", EShaderType::Fragment } };

				depthMaterial->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
			}
			break;
		}
		case EDrawType::DrawArrays:
		{
			ASSERT(false);
			break;
		}
		case EDrawType::DrawInstanced:
		{
			bool materialExists = false;
			depthMaterial = matManager.GetOrAddMaterial<DepthMaterial>("depth_material_instanced", materialExists);

			if (!materialExists)
			{
				VertexInputLayout inputLayout;
				inputLayout.Push<float>(3, VertexInputType::Position);
				inputLayout.Push<float>(3, VertexInputType::Normal);
				inputLayout.Push<float>(2, VertexInputType::TexCoords);

				eastl::vector<ShaderSourceInput> shaders = {
				//{ "Depth_VS-Pos-Normal-UV_Instanced", EShaderType::Vertex },
				{ "VS_Pos-Normal-UV_Depth_Cascaded_Instanced", EShaderType::Vertex },
				{ "GS_Depth_Cascaded", EShaderType::Geometry },
				{ "PS_Empty", EShaderType::Fragment } };

				depthMaterial->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
			}
			break;
		}
		default:
		{
			ASSERT(false);
		}

		}

		return depthMaterial;
	}
	case EDrawMode::DEPTH_VISUALIZE:
	{
		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> depthMaterial = matManager.GetOrAddMaterial<RenderMaterial>("visualise_depth_material", materialExists);

		if (!materialExists)
		{
			VertexInputLayout inputLayout;
			inputLayout.Push<float>(3, VertexInputType::Position);
			inputLayout.Push<float>(3, VertexInputType::Normal);
			inputLayout.Push<float>(2, VertexInputType::TexCoords);

			eastl::vector<ShaderSourceInput> shaders = {
			{ "VS_Pos-UV_UnchangedPosition", EShaderType::Vertex },
			{ "PS_VisualiseDepth", EShaderType::Fragment } };

			depthMaterial->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
			//depthMaterial->OwnedTextures.push_back(DirectionalLightCascadedShadowTexture);
		}

		return depthMaterial;
	}
// 	case EDrawMode::OUTLINE:
// 	{ 
// 		MaterialsManager& matManager = MaterialsManager::Get();
// 		bool materialExists = false;
// 		eastl::shared_ptr<RenderMaterial> outlineMaterial = matManager.GetOrAddMaterial("outline_material", materialExists);
// 
// 		if (!materialExists)
// 		{
// 			outlineMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/OutlineFragmentShader.glsl");
// 		}
// 
// 		return outlineMaterial;
	//}
	case EDrawMode::NORMAL_VISUALIZE:
 	{
 		MaterialsManager& matManager = MaterialsManager::Get();
 		bool materialExists = false;
 		eastl::shared_ptr<RenderMaterial> visNormalMat = matManager.GetOrAddMaterial("normal_visualize_material", materialExists);
 
 		if (!materialExists)
 		{
			VertexInputLayout inputLayout;
			inputLayout.Push<float>(3, VertexInputType::Position);
			inputLayout.Push<float>(3, VertexInputType::Normal);
			inputLayout.Push<float>(2, VertexInputType::TexCoords);

			eastl::vector<ShaderSourceInput> shaders = {
			{ "VisualizeNormalsGeometry/VS_Pos-UV-Normal_Geometry_NormalVisualize", EShaderType::Vertex },
			{ "VisualizeNormalsGeometry/GS_VisualizeNormals", EShaderType::Geometry },
			{ "VisualizeNormalsGeometry/PS_FlatColor", EShaderType::Fragment } };

			visNormalMat->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
 		}
 
 		return visNormalMat;
	}
	}

	return { nullptr };
}

void DeferredRenderer::AddCommand(const RenderCommand & inCommand)
{
	std::lock_guard<std::mutex> lock(RenderCommandsMutex);

	MainCommands.push_back(inCommand);
}

void DeferredRenderer::AddCommands(eastl::vector<RenderCommand> inCommands)
{
	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
#ifdef _DEBUG
	for (RenderCommand& command : inCommands)
	{
		ASSERT(command.DataContainer);
		ASSERT(command.Material);
	}
#endif

	MainCommands.insert(MainCommands.end(), inCommands.begin(), inCommands.end());
}

void DeferredRenderer::SetDrawMode(const EDrawMode::Type inDrawMode)
{
	CurrentDrawMode = inDrawMode;
}

bool DeferredRenderer::GetOrCreateContainer(const eastl::string& inInstanceName, OUT eastl::shared_ptr<MeshDataContainer>& outContainer)
{
	ASSERT(!inInstanceName.empty());
	std::lock_guard<std::mutex> uniqueMutex(GetVAOMutex);
	//GetVAOMutex.lock(); // TODO: Why does this not work?

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

	//GetVAOMutex.unlock();

	return false;
}

eastl::string DeferredRenderer::GetMaterialsDirPrefix()
{
	return "Deferred";
}

void DeferredRenderer::AddPostProcessCommand(const RenderCommand& inCommand)
{
	// TODO: Hack
	if (inCommand.Material->bUsesSceneTextures)
	{
		//inCommand.Material->ExternalTextures.push_back(GBufferColorSpec);
		//inCommand.Material->ExternalTextures.push_back(GBufferNormal);
		inCommand.Material->ExternalTextures.push_back(GBufferDepth);
	}
	//

	PostProcessCommands.push_back(inCommand);
}

void DeferredRenderer::SetViewportSizeToMain()
{
	const WindowsWindow& currentWindow = Engine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();
	RHI::Get()->SetViewportSize(props.Width, props.Height);
}
