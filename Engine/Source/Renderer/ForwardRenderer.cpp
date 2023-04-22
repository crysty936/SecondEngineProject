#include <assert.h>
#include "Renderer/ForwardRenderer.h"
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
#include "Core/EntityHelper.h"
#include "Renderer/Drawable/DepthMaterial.h"
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
#include "Drawable/RenderMaterial_Debug.h"
#include "DrawDebugHelpers.h"
#include "RenderUtils.h"
#include "Math/AABB.h"
#include "imgui.h"
#include "ShaderTypes.h"

constexpr glm::vec4 ClearColor(0.3f, 0.5f, 1.f, 0.4f);

const uint32_t SHADOW_WIDTH = 1024;
const uint32_t SHADOW_HEIGHT = 1024;

constexpr float CAMERA_FOV = 45.f;
constexpr float CAMERA_NEAR = 0.1f;
constexpr float CAMERA_FAR = 50000.f;
constexpr int32_t MAX_CASCADES_COUNT = 3;
eastl::vector<float> shadowCascadeFarPlanes = { CAMERA_FAR / 10.0f, CAMERA_FAR / 2.0f, CAMERA_FAR};

static std::mutex RenderCommandsMutex;
static std::mutex LoadQueueMutex;
static std::mutex GetVAOMutex;
static std::condition_variable LoadQueueCondition;

// void LoaderFunc(GLFWwindow* inLoadingThreadContext)
// {
// 	while (Engine->IsRunning())
// 	{
// 		eastl::queue<RenderingLoadCommand>& loadQueue = OpenGLRenderer::GetRHI().GetLoadQueue();
// 		std::unique_lock<std::mutex> lock{ LoadQueueMutex };
// 		LoadQueueCondition.wait(lock, [&] {return !loadQueue.empty(); });
// 
// 		RenderingLoadCommand newCommand = loadQueue.front();
// 		loadQueue.pop();
// 
// 		lock.unlock();
// 
// 		// TODO: Make work without glfw
// 		//glfwMakeContextCurrent(inLoadingThreadContext);
// 		glEnable(GL_DEBUG_OUTPUT);
// 		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
// 		//glDebugMessageCallback(GLDebugCallback, nullptr);
// 
// 		newCommand.LoadDel.Execute(newCommand.ModelPath, newCommand.Parent);
// 
// 		//glfwMakeContextCurrent(nullptr);
// 	}
// }

ForwardRenderer::ForwardRenderer(const WindowProperties& inMainWindowProperties)
{
	SetViewportSizeToMain();

	RHI::Get()->ClearColor(ClearColor);

	// Set the default uniforms
	SetBaseUniforms();


// 	// Create the shadow map framebuffer
// 	glGenFramebuffers(1, &RHI->ShadowMapBuffer);
// 	glBindFramebuffer(GL_FRAMEBUFFER, RHI->ShadowMapBuffer);
// 
// 	RHI->ShadowBufferTex = eastl::make_shared<OpenGLDepthMap>("ShadowMap");
// 	RHI->ShadowBufferTex->Init();
// 
// 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, RHI->ShadowBufferTex->TexHandle, 0);
// 
// 	glDrawBuffer(GL_NONE);
// 	glReadBuffer(GL_NONE);
// 
// 	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
// 
// 	// Bind the default frame buffer
// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ForwardRenderer::~ForwardRenderer() = default;

eastl::shared_ptr<FullScreenQuad> ScreenQuad;

eastl::shared_ptr<RHIFrameBuffer> GlobalFrameBuffer = nullptr;
eastl::shared_ptr<RHITexture2D> GlobalRenderTexture = nullptr;

eastl::shared_ptr<RHIFrameBuffer> DepthFrameBuffer = nullptr;
eastl::shared_ptr<RHITexture2D> DirectionalLightCascadedShadowTexture = nullptr;

void ForwardRenderer::SetBaseUniforms()
{
	// By default, use a D3D11 projection matrix.
	// Note: glm is RH but uses a sneaky minus to change the handedness of the output to LH (how OpenGL actually is)
	const float windowWidth = static_cast<float>(Engine->GetMainWindow().GetProperties().Width);
	const float windowHeight = static_cast<float>(Engine->GetMainWindow().GetProperties().Height);

	glm::mat4 projection = glm::perspectiveRH_ZO(glm::radians(CAMERA_FOV), windowWidth / windowHeight, CAMERA_NEAR, CAMERA_FAR);
	//glm::mat4 lightProjection = glm::orthoRH_ZO(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	RHI::Get()->PrepareProjectionForRendering(projection);

	UniformsCache["projection"] = projection;
}

void ForwardRenderer::Init(const WindowProperties& inMainWindowProperties)
{
	Instance = new ForwardRenderer{ inMainWindowProperties };

	GlobalFrameBuffer = RHI::Get()->CreateDepthStencilFrameBuffer();
	GlobalRenderTexture = RHI::Get()->CreateRenderTexture();
	RHI::Get()->AttachTextureToFramebufferColor(*GlobalFrameBuffer, *GlobalRenderTexture);

	ScreenQuad = EntityHelper::CreateObject<FullScreenQuad>(GlobalRenderTexture, "Global Renderer Screen Quad");
	ScreenQuad->CreateCommand();
	ScreenQuad->GetCommand().Material->OwnedTextures.push_back(GlobalRenderTexture);

	DepthFrameBuffer = RHI::Get()->CreateEmptyFrameBuffer();
	//DepthRenderTexture = RHI::Instance->CreateDepthMap(SHADOW_WIDTH, SHADOW_HEIGHT);
	DirectionalLightCascadedShadowTexture = RHI::Get()->CreateArrayDepthMap(SHADOW_WIDTH, SHADOW_HEIGHT, MAX_CASCADES_COUNT);
	RHI::Get()->AttachTextureToFramebufferDepth(*DepthFrameBuffer, *DirectionalLightCascadedShadowTexture);
}

void ForwardRenderer::Terminate()
{
	ASSERT(Instance);
	delete Instance;
}

void ForwardRenderer::Draw()
{
	ImGui::Begin("Renderer settings");

	SetBaseUniforms();
	UpdateUniforms();

	DrawShadowMap();

	SetLightingConstants();

	// Clear default framebuffer buffers
	RHI::Get()->ClearBuffers();

	//RHI::Instance->BindFrameBuffer(*GlobalFrameBuffer);
	//RHI::Instance->ClearTexture(*GlobalRenderTexture, glm::vec4(0.f, 0.f, 0.f, 1.f));

	// Clear additional framebuffer buffers
	//RHI::Instance->ClearBuffers();
// 

    DrawCommands(MainCommands);

	//RHI::Instance->UnbindFrameBuffer(*GlobalFrameBuffer);



// To draw the depth map
//  	SetDrawMode(EDrawMode::DEPTH_VISUALIZE);
//  	ScreenQuad->GetCommand().Material->WeakTextures.clear();
//  	ScreenQuad->GetCommand().Material->WeakTextures.push_back(DepthRenderTexture);
//  	DrawCommand(ScreenQuad->GetCommand());


	// Draw debug primitives
	DrawDebugManager::Draw();

	ImGui::End();
}

void ForwardRenderer::Present()
{
	RHI::Get()->SwapBuffers();
}

void ForwardRenderer::SetLightingConstants()
{
	const eastl::vector<eastl::shared_ptr<LightSource>>& lights = SceneManager::Get().GetCurrentScene().GetLights();
	
	eastl::vector<eastl::shared_ptr<LightSource>> dirLights;
	eastl::vector<eastl::shared_ptr<LightSource>> pointLights;

	for (const eastl::shared_ptr<LightSource>& light : lights)
	{
		switch (light->Data.Type)
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

	//////////////////////////////////////////////////////////////////////////
	// ImGui
	//////////////////////////////////////////////////////////////////////////

	ImGui::SeparatorText("Lighting");

	static bool bUpdateViewPosDir = true;
	ImGui::Checkbox("Update View Pos Dir", &bUpdateViewPosDir);

	ImGui::Checkbox("Visualize Normals", &bNormalVisualizeMode);
	UniformsCache["bNormalVisualizeMode"] = bNormalVisualizeMode ? 1 : 0;

	ImGui::Checkbox("Use Normal Mapping", &bUseNormalMapping);
	UniformsCache["bUseNormalMapping"] = bUseNormalMapping ? 1 : 0;

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////

	const glm::vec3 cameraPos = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetAbsoluteTransform().Translation;

	constexpr glm::vec3 forward = glm::vec3(0.f, 0.f, 1.f);
	const glm::vec3 cameraForward = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetAbsoluteTransform().Rotation * forward;

	if (bUpdateViewPosDir)
	{
		UniformsCache["ViewPos"] = cameraPos;
		UniformsCache["ViewDir"] = cameraForward;
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


	eastl::vector<SPointLight> shaderPointLightData;
	for (const eastl::shared_ptr<LightSource>& light : pointLights)
	{
		SPointLight pointLight;

		const Transform& lightTransf = light->GetAbsoluteTransform();
		pointLight.position = glm::vec4(lightTransf.Translation.x, lightTransf.Translation.y, lightTransf.Translation.z, 0.f);

		const PointLightData& pointData = light->Data.TypeData.PointData;
		
		pointLight.linear = pointData.Linear;
		pointLight.quadratic = pointData.Quadratic;
		pointLight.color = glm::vec4(pointData.Color.x, pointData.Color.y, pointData.Color.z, 0.f);

		shaderPointLightData.push_back(pointLight);
	}

	UniformsCache["NumPointLights"] =  static_cast<int32_t>(shaderPointLightData.size());
	UniformsCache["PointLights"] = shaderPointLightData;
}

void ForwardRenderer::DrawSkybox()
{
// 	if (!MainSkyboxCommand.Parent.lock())
// 	{
// 		return;
// 	}
// 
// 	if (!MainSkyboxCommand.VAO->bReadyForDraw)
// 	{
// 		MainSkyboxCommand.VAO->SetupState();
// 	}
// 
// 	glDepthFunc(GL_LEQUAL);
// 	DrawCommand(MainSkyboxCommand);
// 	glDepthFunc(GL_LESS);
}

glm::mat4 CreateMyOrthoLH(float left, float right, float bottom, float top, float zNear, float zFar)
{
	glm::mat4 Result(1);
	Result[0][0] = 2.f / (right - left);
	Result[1][1] = 2.f / (top - bottom);
	Result[2][2] = 1.f / (zFar - zNear);
	Result[3][0] = (left + right) / (left - right);
	Result[3][1] = (top + bottom) / (bottom - top);
	Result[3][2] = zNear / (zNear - zFar);

	return Result;
}

glm::mat4 ForwardRenderer::CreateCascadeMatrix(const glm::mat4& inCameraProj, const glm::mat4& inCameraView, const glm::vec3& inLightDir)
{
	const glm::mat4 worldToCameraClip = inCameraProj * inCameraView;
 	const glm::vec3 cameraProjCenter = RenderUtils::GetProjectionCenter(worldToCameraClip);
 
 	// Create tight shadow projection around camera frustum
 	AABB projBox;
 	eastl::array<glm::vec3, 8> cameraProjPoints = RenderUtils::GenerateSpaceCorners(worldToCameraClip);
 
	// Point light at light dir relative to center of projection
	const glm::mat4 lightView = glm::lookAtLH(cameraProjCenter, cameraProjCenter + inLightDir, glm::vec3(0.0f, 1.0f, 0.0f));

 	for (const glm::vec3& point : cameraProjPoints)
 	{
 		const glm::vec4 lightSpacePoint = lightView * glm::vec4(point.x, point.y, point.z, 1.f);
 		projBox += glm::vec3(lightSpacePoint.x, lightSpacePoint.y, lightSpacePoint.z);
 	}
 
	const glm::mat4 lightProjection = glm::orthoLH_ZO(projBox.Min.x, projBox.Max.x, projBox.Min.y, projBox.Max.y, projBox.Min.z, projBox.Max.z);

	return lightProjection * lightView;
}

eastl::vector<glm::mat4> ForwardRenderer::CreateCascadesMatrices()
{
	const eastl::vector<eastl::shared_ptr<LightSource>>& lights = SceneManager::Get().GetCurrentScene().GetLights();
	eastl::vector<eastl::shared_ptr<LightSource>> dirLights;

	for (const eastl::shared_ptr<LightSource>& light : lights)
	{
		switch (light->Data.Type)
		{
		case ELightType::Directional:
		{
			dirLights.push_back(light);
			break;
		}
		}
	}

	UniformsCache["DirectionalLightCascadedShadowTexture"] = DirectionalLightCascadedShadowTexture;

	ASSERT(dirLights.size() <= 1);

	if (dirLights.size() == 0)
	{
		return {};
	}

	const glm::vec3 lightDir = dirLights[0]->GetAbsoluteTransform().Rotation * glm::vec3(0.f, 0.f, 1.f);

	eastl::vector<glm::mat4> cascades;
	cascades.reserve(CascadesCount);

	const glm::mat4& cameraView = UniformsCache["view"].GetValue<glm::mat4>();

	const float windowWidth = static_cast<float>(Engine->GetMainWindow().GetProperties().Width);
	const float windowHeight = static_cast<float>(Engine->GetMainWindow().GetProperties().Height);

	for (int32_t i = 0; i < CascadesCount; ++i)
	{
		const float near = i == 0 ? CAMERA_NEAR : shadowCascadeFarPlanes[i - 1];
		const float far = shadowCascadeFarPlanes[i];

		const glm::mat4 cameraProj = glm::perspectiveRH_ZO(glm::radians(CAMERA_FOV), windowWidth / windowHeight, near, far);
		const glm::mat4 worldToLightClip = CreateCascadeMatrix(cameraProj, cameraView, lightDir);

		cascades.push_back(worldToLightClip);
	}

	return cascades;
}

void ForwardRenderer::DrawShadowMap()
{
	ImGui::SeparatorText("Shadow");

	ImGui::Checkbox("Update Shadow Matrices", &UpdateShadowMatrices);
	ImGui::Checkbox("Visualize Cascades", &bCascadeVisualizeMode);
	ImGui::Checkbox("Use Shadows", &bUseShadows);
	UniformsCache["bUseShadows"] = bUseShadows;

	// Cull front face to solve Peter Panning
	//RHI::Instance->SetFaceCullMode(EFaceCullMode::Front);

	RHI::Get()->BindFrameBuffer(*DepthFrameBuffer);
	RHI::Get()->ClearBuffers();

 	SetDrawMode(EDrawMode::DEPTH);
 	RHI::Get()->SetViewportSize(SHADOW_WIDTH, SHADOW_HEIGHT);
	RHI::Get()->ClearTexture(*DirectionalLightCascadedShadowTexture, glm::vec4(1.f, 1.f, 1.f, 1.f));

	static glm::mat4 lightView;
	static glm::mat4 lightProjection;
	
	// Debug stuff
	static glm::mat4 debugMatrixCamera;
	static eastl::vector<glm::mat4> lsMatrices;
	static glm::mat4 ShadowCameraViewMatrix;

	if (UpdateShadowMatrices)
	{
		// Set cascades far
		shadowCascadeFarPlanes.clear();
		for (int32_t i = 0; i < CascadesCount; ++i)
		{
			float cascadeFar = CAMERA_FAR / (CascadesCount - i);

			// Hack: Keep first cascade with far = max 100 always
			// TODO: Fix this with a proper distribution function
			if (i == 0)
			{
				cascadeFar = glm::min(100.f, cascadeFar);
			}

			shadowCascadeFarPlanes.push_back(cascadeFar);
		}

		glm::mat4 cameraProj = UniformsCache["projection"].GetValue<glm::mat4>();
		glm::mat4 cameraView = UniformsCache["view"].GetValue<glm::mat4>();

		debugMatrixCamera = cameraProj * cameraView;
		lsMatrices = CreateCascadesMatrices();
		ShadowCameraViewMatrix = cameraView;
	}

	static bool DebugDrawPojections = false;
	ImGui::Checkbox("Visualize Shadow Projections", &DebugDrawPojections);

	if (DebugDrawPojections)
	{
		for (const glm::mat4& worldToLightClip : lsMatrices)
		{
			DrawDebugHelpers::DrawProjection(worldToLightClip);
		}
	}

	static bool DebugDrawShadowCameraProjection = false;
	ImGui::Checkbox("Visualize Shadow Camera Projection", &DebugDrawShadowCameraProjection);

	if (DebugDrawShadowCameraProjection)
	{
 		DrawDebugHelpers::DrawProjection(debugMatrixCamera);
	}

 	RHI::Get()->PrepareProjectionForRendering(lightProjection);

	UniformsCache["lsMatrices"] = lsMatrices;
	UniformsCache["ShadowCameraViewMatrix"] = ShadowCameraViewMatrix;
	UniformsCache["bShadowVisualizeMode"] = bCascadeVisualizeMode ? 1 : 0;

	ImGui::DragInt("Shadow Cascades", &CascadesCount, 0.02f, 0, 3, "%d", ImGuiSliderFlags_AlwaysClamp);

	UniformsCache["cascadesCount"] = CascadesCount;
	UniformsCache["shadowCascadeFarPlanes"] = shadowCascadeFarPlanes;
 
// 	RenderCommandsMutex.lock();
 	DrawCommands(MainCommands);
// 	RenderCommandsMutex.unlock();

	RHI::Get()->UnbindFrameBuffer(*DepthFrameBuffer);

 	SetViewportSizeToMain();
 	SetDrawMode(EDrawMode::Default);

	// Reset projection
	SetBaseUniforms();
	UpdateUniforms();

 	// Reset to default
	//RHI::Instance->SetFaceCullMode(EFaceCullMode::Back);

}

void ForwardRenderer::UpdateUniforms()
{
	const glm::mat4 view = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetLookAt();
	UniformsCache["view"] = view;
}

void ForwardRenderer::DrawCommands(const eastl::vector<RenderCommand>& inCommands)
{
	// TODO: Fix the draw mode thing with objects being able to tell which passes they are in
	// and the renderer being able to use certain passes
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

void ForwardRenderer::DrawCommand(const RenderCommand& inCommand)
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

eastl::shared_ptr<RenderMaterial> ForwardRenderer::GetMaterial(const RenderCommand& inCommand) const
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
		// similarly, use NUM_CASCADES or define a default if that is not defined(or crash to signal that it's missing)
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
			depthMaterial->OwnedTextures.push_back(DirectionalLightCascadedShadowTexture);
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
			{ "VS_Pos-UV-Normal_Geometry_NormalVisualize", EShaderType::Vertex },
			{ "GS_VisualizeNormals", EShaderType::Geometry },
			{ "PS_FlatColor", EShaderType::Fragment } };

			visNormalMat->Shader = RHI::Get()->CreateShaderFromPath(shaders, inputLayout);
 		}
 
 		return visNormalMat;
	}
	}

	return { nullptr };
}

void ForwardRenderer::SetVSyncEnabled(const bool inEnabled)
{
	//glfwSwapInterval(inEnabled);
}

void ForwardRenderer::AddMirrorCommand(const RenderCommand & inCommand)
{
	// 	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	// 	MirrorCommands.push_back(inCommand);
}

void ForwardRenderer::AddCommand(const RenderCommand & inCommand)
{
	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	MainCommands.push_back(inCommand);
}

void ForwardRenderer::AddCommands(eastl::vector<RenderCommand> inCommands)
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

void ForwardRenderer::SetDrawMode(const EDrawMode::Type inDrawMode)
{
	CurrentDrawMode = inDrawMode;
}

void ForwardRenderer::AddRenderLoadCommand(const RenderingLoadCommand & inCommand)
{
	std::unique_lock<std::mutex> lock{ LoadQueueMutex };

	//LoadQueue.push(inCommand);
	LoadQueueCondition.notify_one();
}

bool ForwardRenderer::GetOrCreateContainer(const eastl::string& inInstanceName, OUT eastl::shared_ptr<MeshDataContainer>& outContainer)
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

void ForwardRenderer::SetViewportSizeToMain()
{
	const WindowsWindow& currentWindow = Engine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();
	RHI::Get()->SetViewportSize(props.Width, props.Height);
}

