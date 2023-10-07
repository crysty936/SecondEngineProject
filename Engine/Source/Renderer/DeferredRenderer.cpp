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
static eastl::shared_ptr<RHITexture2D> GBufferAlbedo = nullptr;
static eastl::shared_ptr<RHITexture2D> GBufferMetallicRoughness = nullptr;

eastl::shared_ptr<VisualizeDepthQuad> VisualizeDepthUtil;
eastl::shared_ptr<GBufferVisualizeQuad> VisualizeNormalsUtil;
eastl::shared_ptr<GBufferVisualizeQuad> VisualizeAlbedoUtil;
eastl::shared_ptr<GBufferVisualizeQuad> VisualizeRoughnessUtil;
eastl::shared_ptr<DefaultPBRLightingModelQuad> DefaultPBRShadingModelQuad;

void DeferredRenderer::InitInternal()
{
	const WindowsWindow& currentWindow = GEngine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();

	GBuffer = RHI::Get()->CreateEmptyFrameBuffer();
	GlobalRenderTexture = RHI::Get()->CreateRenderTexture(props.Width, props.Height, ERHITexturePrecision::Float16, ERHITextureFilter::Nearest);
	//RHI::Get()->AttachTextureToFramebufferColor(*GBuffer, *GlobalRenderTexture);

	GBufferDepth = RHI::Get()->CreateDepthMap(props.Width, props.Height);

	RHI::Get()->AttachTextureToFramebufferDepth(*GBuffer, GBufferDepth);

	GBufferNormal = RHI::Get()->CreateRenderTexture(props.Width, props.Height, ERHITexturePrecision::Float16, ERHITextureFilter::Nearest);
	RHI::Get()->AttachTextureToFramebufferColor(*GBuffer, GBufferNormal);

 	GBufferAlbedo = RHI::Get()->CreateRenderTexture(props.Width, props.Height, ERHITexturePrecision::UnsignedByte, ERHITextureFilter::Nearest);
 	RHI::Get()->AttachTextureToFramebufferColor(*GBuffer, GBufferAlbedo);

	GBufferMetallicRoughness = RHI::Get()->CreateRenderTexture(props.Width, props.Height, ERHITexturePrecision::UnsignedByte, ERHITextureFilter::Nearest);
	RHI::Get()->AttachTextureToFramebufferColor(*GBuffer, GBufferMetallicRoughness);

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
 	VisualizeAlbedoUtil->GetCommand().Material->ExternalTextures.push_back(GBufferAlbedo);
 	VisualizeAlbedoUtil->SetScale(glm::vec3(0.33f, 0.33f, 1.f));
 	VisualizeAlbedoUtil->SetRelativeLocation(glm::vec3(0.67f, 0.65f, 0.0f));

	VisualizeRoughnessUtil = SceneHelper::CreateObject<GBufferVisualizeQuad>("VisualizeRoughnessTex");
	VisualizeRoughnessUtil->CreateCommand();
	VisualizeRoughnessUtil->GetCommand().Material->ExternalTextures.push_back(GBufferAlbedo);
	VisualizeRoughnessUtil->SetScale(glm::vec3(0.33f, 0.33f, 1.f));
	VisualizeRoughnessUtil->SetRelativeLocation(glm::vec3(0.67f, -0.15f, 0.0f));

	DefaultPBRShadingModelQuad = SceneHelper::CreateObject<DefaultPBRLightingModelQuad>("DefaultLightingModelQuad");
	DefaultPBRShadingModelQuad->CreateCommand();
	DefaultPBRShadingModelQuad->GetCommand().Material->ExternalTextures.push_back(GBufferAlbedo);
	DefaultPBRShadingModelQuad->GetCommand().Material->ExternalTextures.push_back(GBufferNormal);
	DefaultPBRShadingModelQuad->GetCommand().Material->ExternalTextures.push_back(GBufferMetallicRoughness);
	DefaultPBRShadingModelQuad->GetCommand().Material->ExternalTextures.push_back(GBufferDepth);
	DefaultPBRShadingModelQuad->CreateCommand();
}

void DeferredRenderer::Draw()
{
	//RHI::Get()->SetCullEnabled(false);
	ImGui::Begin("Renderer settings");

	SetBaseUniforms();
	UpdateUniforms();

	SetLightingConstants();


 	RHI::Instance->BindFrameBuffer(*GBuffer);
	RHI::Get()->ClearBuffers();
	RHI::Instance->ClearTexture(*GBufferDepth, glm::vec4(1.f, 1.f, 1.f, 1.f));

	//static glm::vec3 viewOrigin;
	//static bool bUpdateViewOrigin = true;
	//ImGui::Checkbox("TestUpdateViewOrigin", &bUpdateViewOrigin);
	//if (bUpdateViewOrigin)
	//{
	//	viewOrigin = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetAbsoluteTransform().Translation;
	//}
	//DrawDebugHelpers::DrawDebugPoint(viewOrigin);


    DrawCommands(MainCommands);

	DrawDecals(DecalCommands);

	RHI::Instance->BindDefaultFrameBuffer();
	RHI::Get()->ClearBuffers();

	RHI::Get()->CopyFrameBufferDepth(GBuffer, nullptr);


	RHI::Get()->SetDepthWrite(false);

	DrawCommand(DefaultPBRShadingModelQuad->GetCommand());
	
	// Draw debug primitives
	DrawDebugManager::Draw();

	static bool bViewGBuffer = false;
	ImGui::Checkbox("View GBuffer", &bViewGBuffer);

	if (bViewGBuffer)
	{
		RHI::Get()->SetDepthTest(false);

 		DrawCommand(VisualizeDepthUtil->GetCommand());

 		VisualizeNormalsUtil->GetCommand().Material->ExternalTextures.clear();
 		VisualizeNormalsUtil->GetCommand().Material->ExternalTextures.push_back(GBufferNormal);
 		DrawCommand(VisualizeNormalsUtil->GetCommand());
 
  		VisualizeAlbedoUtil->GetCommand().Material->ExternalTextures.clear();
  		VisualizeAlbedoUtil->GetCommand().Material->ExternalTextures.push_back(GBufferAlbedo);
  		DrawCommand(VisualizeAlbedoUtil->GetCommand());

		VisualizeRoughnessUtil->GetCommand().Material->ExternalTextures.clear();
		VisualizeRoughnessUtil->GetCommand().Material->ExternalTextures.push_back(GBufferMetallicRoughness);
		DrawCommand(VisualizeRoughnessUtil->GetCommand());
	}

	//if (ImGui::Button("Do RenderDoc Capture"))
	//{
	//	RenderDocPlugin* renderDoc = GEngine->GetInternalPlugin<RenderDocPlugin>("RenderDocPlugin");

	//	if (renderDoc)
	//	{
	//		renderDoc->DoCapture();
	//	}
	//}

	RHI::Get()->SetDepthTest(true);
	RHI::Get()->SetDepthWrite(true);

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

void DeferredRenderer::RenderPreStencil(const RenderCommand& inCommand)
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

	eastl::shared_ptr<RHIShader> vertexOnlyShader = RHI::Get()->GetVertexOnlyShader(*material);
	RHI::Get()->BindShader(*vertexOnlyShader);

	material->ResetUniforms();

	UniformsCache["model"] = parent->GetModelMatrix();
	UniformsCache["ObjPos"] = parent->GetAbsoluteTransform().Translation;

	parent->UpdateCustomUniforms(UniformsCache);
	material->SetUniformsValue(UniformsCache, EShaderType::Sh_Vertex);
	material->BindBuffers(EShaderType::Sh_Vertex);

	const uint32_t indicesCount = dataContainer->VBuffer->GetIndicesCount();


	// Disable culling
	//RHI::Get()->SetCullEnabled(false);
	RHI::Get()->SetCullMode(ECullFace::Front);
	//RHI::Get()->SetCullMode(ECullFace::Back);
	RHI::Get()->SetBlendEnabled(true);

	// Make sure that the stencil draw(which draws just black because no fragment shader) doesn't modify the dest color or alpha
	// Rcolor = SourceFactorColor * SourceColor + DestFactorColor * DestColor
	// RAlpha = SourceFactorAlpha * SourceAlpha + DestFactorAlpha * DestAlpha
	BlendState preStencilBlendState;
	preStencilBlendState.SrcColorBlendFunc = EBlendFunc::One_Minus_Src_Alpha;
	preStencilBlendState.DestColorBlendFunc = EBlendFunc::Src_Alpha;
	preStencilBlendState.SrcAlphaBlendFunc = EBlendFunc::Zero;
	preStencilBlendState.DestAlphaBlendFunc = EBlendFunc::Zero;

	preStencilBlendState.ColorBlendEq = EBlendEquation::Add;
	preStencilBlendState.AlphaBlendEq = EBlendEquation::Add;

	RHI::Get()->SetBlendState(preStencilBlendState);

	// Carmack's reverse
	DepthStencilState preStencilDepthStencilState;

	// Mask that tells which bits can be written to in the stencil buffer
	preStencilDepthStencilState.FrontStencilMask = 0x01;
	preStencilDepthStencilState.BackStencilMask = 0x01;

	// This is one of the main things that allows it to work - it has to be opposite to what the other draws use
	preStencilDepthStencilState.DepthOperation = EDepthOp::GreaterOrEqual;

	// Ensure that we don't care about existing stencil test values
	preStencilDepthStencilState.FrontStencilFunc.StencilFunction = EStencilFunc::Always;
	preStencilDepthStencilState.BackStencilFunc.StencilFunction = EStencilFunc::Always;

	// Write stencil mask for values that fail or pass stencil test(all) and invert those that pass stencil and depth(values where depth is smaller than existing depth, 
	// thus setting ~0x01 to all values stencil fragments that are not touching an existing depth
	//glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
	preStencilDepthStencilState.FrontStencilOp.StencilOpStencilFail = EStencilOp::Keep;
	preStencilDepthStencilState.FrontStencilOp.StencilOpZFail = EStencilOp::Keep;
	preStencilDepthStencilState.FrontStencilOp.StencilOpZPass = EStencilOp::Invert;

	preStencilDepthStencilState.BackStencilOp.StencilOpStencilFail = EStencilOp::Keep;
	preStencilDepthStencilState.BackStencilOp.StencilOpZFail = EStencilOp::Keep;
	preStencilDepthStencilState.BackStencilOp.StencilOpZPass = EStencilOp::Invert;


	RHI::Get()->SetDepthStencilState(preStencilDepthStencilState);
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

	material->UnbindBuffers(EShaderType::Sh_Vertex);
	RHI::Get()->UnbindShader(*(material->Shader));

	BlendState defaultBlendState;
	RHI::Get()->SetBlendState(defaultBlendState);
	RHI::Get()->SetBlendEnabled(false);

	RHI::Get()->SetCullEnabled(true);
	RHI::Get()->SetCullMode(ECullFace::Back);
	RHI::Get()->SetDepthOp(EDepthOp::Less);

}

#define USE_DECAL_SPHERE_RADIUS_OPTIMIZATION 0

void DeferredRenderer::DrawDecals(const eastl::vector<RenderCommand>& inCommands)
{
	RHI::Get()->SetDepthWrite(false);

	SetDrawMode(EDrawMode::Default);
	for (const RenderCommand& renderCommand : inCommands)
	{
		const eastl::shared_ptr<const DrawableObject> parent = renderCommand.Parent.lock();

		//const glm::mat4 cameraMatrix = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetAbsoluteTransform().GetMatrix();
		//const glm::vec3 cameraMatrixPos = glm::vec3(cameraMatrix[3][0], cameraMatrix[3][1], cameraMatrix[3][2]);

		const glm::mat4 decalToWorld = parent->GetModelMatrix();

		eastl::array<glm::vec3, 8> projCorners = RenderUtils::GenerateSpaceCorners(glm::inverse(decalToWorld), -1.f, 1.f);
		DrawDebugHelpers::DrawBoxArray(projCorners, false);

		// Pre Stencil
		RHI::Get()->SetStencilTestEnabled(true);

		RenderPreStencil(renderCommand);

		// Loose optimization that compares camera against decal cube main diagonal(radius of sphere that encompasses bounding box)
		// Allows effiecent depth test and face cull when we know for sure that we are not nowhere near inside the decal
#if USE_DECAL_SPHERE_RADIUS_OPTIMIZATION
		// Is camera inside decal
		const glm::vec3 cameraPos = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetAbsoluteTransform().Translation;
		const glm::vec3 decalPos = parent->GetAbsoluteTransform().Translation;

		const float cameraDecalDistanceSquared = glm::lengthSquared(cameraPos - decalPos);

		const glm::vec3 decalXScaledAxis = glm::vec3(decalToWorld[0][0], decalToWorld[0][1], decalToWorld[0][2]);
		const glm::vec3 decalYScaledAxis = glm::vec3(decalToWorld[1][0], decalToWorld[1][1], decalToWorld[1][2]);
		const glm::vec3 decalZScaledAxis = glm::vec3(decalToWorld[2][0], decalToWorld[2][1], decalToWorld[2][2]);

		const float conservativeRadius = glm::sqrt(glm::lengthSquared(decalXScaledAxis) + glm::lengthSquared(decalYScaledAxis) + glm::lengthSquared(decalZScaledAxis));
		const float decalBoundingSphereRadius = (conservativeRadius + CAMERA_NEAR * 2.f) * (conservativeRadius + CAMERA_NEAR * 2.f);
		const bool bInsideDecal = cameraDecalDistanceSquared < decalBoundingSphereRadius;

		if (bInsideDecal)
		{
#endif
			// CW ensures that there's always a face of the decal cube that's visible(the inside one)
			//RHI::Get()->SetRasterizerFront(ERasterizerFront::CW);
			RHI::Get()->SetCullMode(ECullFace::Front);

			// This has to be combined with CW always because the pixels that will be used for drawing now will always be the ones *under* the surface that the decal intersects(ones above it are discarded 
			// always by the operation that discards pixels outside cube bounds)
			//RHI::Get()->SetDepthOp(EDepthOp::Always);
			RHI::Get()->SetDepthTest(false);

#if USE_DECAL_SPHERE_RADIUS_OPTIMIZATION
		}
		else
		{
			RHI::Get()->SetDepthOp(EDepthOp::Less);
			RHI::Get()->SetCullMode(ECullFace::Back);
		}
#endif

		DepthStencilState drawState;
		// Test against 0x01.
		drawState.FrontStencilFunc.StencilFunction = EStencilFunc::Equal;
		drawState.FrontStencilFunc.StencilRef = 0x01; // Test against 1
		drawState.FrontStencilFunc.StencilFuncMask = 0x01; // Sample the first bit

		drawState.BackStencilFunc = drawState.FrontStencilFunc;

		// Write 0 to the stencil test, we have used the result, just cleanup
		drawState.FrontStencilOp.StencilOpStencilFail = EStencilOp::Zero;
		drawState.FrontStencilOp.StencilOpZFail = EStencilOp::Zero;
		drawState.FrontStencilOp.StencilOpZPass = EStencilOp::Zero;
		drawState.BackStencilOp = drawState.FrontStencilOp;

		RHI::Get()->SetDepthStencilState(drawState);

		DrawCommand(renderCommand);
	}

	RHI::Get()->SetStencilTestEnabled(false);
	RHI::Get()->SetCullMode(ECullFace::Back);
	RHI::Get()->SetDepthOp(EDepthOp::Less);
	RHI::Get()->SetDepthTest(true);
	RHI::Get()->SetDepthWrite(true);
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

	glm::mat4 model = parent->GetModelMatrix();
	UniformsCache["model"] = model;
	UniformsCache["ObjPos"] = parent->GetAbsoluteTransform().Translation;

	// Path Tracing Debug

	if (inCommand.Triangles.size() != 0)
	{
		RenderCommand& nonConstCommand = const_cast<RenderCommand&>(inCommand);
		if (!nonConstCommand.AccStructure.IsValid())
		{
			eastl::vector<PathTraceTriangle> transformedTriangles = nonConstCommand.Triangles;
			for (PathTraceTriangle& triangle : transformedTriangles)
			{
				triangle.Transform(model);
			}

			nonConstCommand.AccStructure.Build(transformedTriangles);
		}

		nonConstCommand.AccStructure.Root->DebugDraw();
	}

	// Path Tracing Debug

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
				{ "VS_Pos-Normal-UV_Depth_Cascaded", EShaderType::Sh_Vertex },
				{ "GS_Depth_Cascaded", EShaderType::Sh_Geometry },
				{ "PS_Empty", EShaderType::Sh_Fragment } };

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
				{ "VS_Pos-Normal-UV_Depth_Cascaded_Instanced", EShaderType::Sh_Vertex },
				{ "GS_Depth_Cascaded", EShaderType::Sh_Geometry },
				{ "PS_Empty", EShaderType::Sh_Fragment } };

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
			{ "VS_Pos-UV_UnchangedPosition", EShaderType::Sh_Vertex },
			{ "PS_VisualiseDepth", EShaderType::Sh_Fragment } };

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
			{ "VisualizeNormalsGeometry/VS_Pos-UV-Normal_Geometry_NormalVisualize", EShaderType::Sh_Vertex },
			{ "VisualizeNormalsGeometry/GS_VisualizeNormals", EShaderType::Sh_Geometry },
			{ "VisualizeNormalsGeometry/PS_FlatColor", EShaderType::Sh_Fragment } };

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
	LOG_INFO("Get or Create Render Container.");
	ASSERT(!inInstanceName.empty());
	std::lock_guard<std::mutex> uniqueMutex(GetVAOMutex);
	//GetVAOMutex.lock(); // TODO: Why does this not work?

	using iterator = const eastl::unordered_map<eastl::string, eastl::shared_ptr<MeshDataContainer>>::iterator;
	const iterator& containerIter = RenderDataContainerMap.find(inInstanceName);
	const bool materialExists = containerIter != RenderDataContainerMap.end();

	if (materialExists)
	{
		LOG_INFO("Container already exists, returning.");

		outContainer = (*containerIter).second;

		return true;
	}

	LOG_INFO("Container not present, creating new.");
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

void DeferredRenderer::AddDecalCommand(const RenderCommand& inCommand)
{
	// TODO: Hack
	if (inCommand.Material->bUsesSceneTextures)
	{
		//inCommand.Material->ExternalTextures.push_back(GBufferColorSpec);
		//inCommand.Material->ExternalTextures.push_back(GBufferNormal);
		inCommand.Material->ExternalTextures.push_back(GBufferDepth);
	}
	//

	DecalCommands.push_back(inCommand);
}

void DeferredRenderer::SetViewportSizeToMain()
{
	const WindowsWindow& currentWindow = GEngine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();
	RHI::Get()->SetViewportSize(props.Width, props.Height);
}
