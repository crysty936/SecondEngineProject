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

}

struct Sphere
{
	glm::vec3 Origin = glm::vec3(0.f, 0.f, 0.f);
	float Radius = 0.f;
};

struct PathTracingRay
{
	glm::vec3 Origin = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 Direction = glm::vec3(0.f, 0.f, 0.f);
};


float focalLength = 1.f;

uint32_t PathTracingRenderer::PerPixel(const uint32_t x, const uint32_t y, const WindowProperties& inProps, const glm::mat4& inInvProj, const glm::mat4& inInvView, const glm::vec3& inCamPos)
{
	static eastl::vector<Sphere> spheres = { {glm::vec3(0.f, 0.f, 0.f), 5.f} };

	glm::vec2 normalizedCoords = glm::vec2(float(x) / float(inProps.Width) , float(y) / float(inProps.Height) );
	normalizedCoords = normalizedCoords * 2.f - 1.f; // 0..1 -> -1..1

	//normalizedCoords.y /= inProps.AspectRatio;

	const glm::vec3 pixelPos = glm::vec3(normalizedCoords.x , normalizedCoords.y, 0.f);
	//glm::vec3 focalPointPos = glm::vec3(0.f, 0.f, -focalLength);

	//const glm::vec3 rayDir = glm::normalize(pixelPos - focalPointPos);


	glm::vec4 worldSpace = inInvProj * glm::vec4(normalizedCoords.x, normalizedCoords.y, 1.f, 1.f);
	worldSpace /= worldSpace.w;

	glm::vec3 rayDir = glm::normalize(glm::vec3(worldSpace) - pixelPos);

	rayDir = glm::vec3(inInvView * glm::vec4(rayDir.x, rayDir.y, rayDir.z, 0.f));
	


	PathTracingRay tempRay = { inCamPos, rayDir };

	glm::vec4 color;

	// o + mt -> ray equation with o = origin and m is direction
	// x^2 + y^2 + z^2 - r^2 = 0 -> circle equation
	// 
	// => (ox + mxt)^2 + (oy + myt)^2 + (oz + mzt)^2 - r^2 = 0
	// =>...=>
	// (mx^2 + my^2 + mz^2)t^2 + (2 * (oxmx + oymy + ozmz))t + (ox^2 + oy^2 + oz^2 - r^2)

	// => quadratic equating at^2 + bt + c
	// with result = (-b +- sqrt(b^2 - 4ac)) / 2a
	// and discriminant = sqrt(b^2 - 4ac))
	// if discriminant 
	// > 0 -> 2 solutions	(2 hits)
	// = 0 -> 1 solution	(1 hit)
	// < 0 -> no solutions	(0 hits)


	int32_t closestSphere = -1;
	float closestDistance = std::numeric_limits<float>::max();
	for (int32_t i = 0; i < spheres.size(); ++i) 
	{
		const Sphere& currentSphere = spheres[i];
		const glm::vec3 origin = tempRay.Origin - currentSphere.Origin;

		const float a = glm::dot(tempRay.Direction, tempRay.Direction);
		const float b = 2 * glm::dot(tempRay.Origin, tempRay.Direction);
		const float c = glm::dot(tempRay.Origin, tempRay.Origin) - (currentSphere.Radius * currentSphere.Radius);

		const float discriminant = sqrt((b * b) - (4 * a * c));

		if (discriminant > 0.f)
		{
			const float firstHit = (-b - glm::sqrt(discriminant)) / (2.f * a);
			//float secondhit = (+b - glm::sqrt(discriminant)) / (2.f * a);

			if (firstHit > 0.f && firstHit < closestDistance)
			{
				closestDistance = firstHit;
				closestSphere = i;
			}
		}
	}

	if (closestSphere == -1)
	{
		float a = 0.5f * (rayDir.y + 1.f);
		const glm::vec3 skyColor = (1.f - a) * glm::vec3(1.f, 1.f, 1.f) + a * glm::vec3(0.5f, 0.7f, 1.f);
		color = glm::vec4(skyColor.x, skyColor.y, skyColor.z, 1.f);
	}
	else
	{
		color = glm::vec4(1.f, 0.f, 0.f, 1.f);
	}

	return ConvertToRGBA(color);
}




void PathTracingRenderer::Draw()
{
	ImGui::Begin("Renderer settings");

	ImGui::DragFloat("Focal Length", &focalLength);

	const WindowsWindow& currentWindow = GEngine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();

	const glm::mat4 projection = glm::perspectiveRH_ZO(glm::radians(CAMERA_FOV), props.AspectRatio, CAMERA_NEAR, CAMERA_FAR);
	const glm::mat4 invProj = glm::inverse(projection);

	const glm::mat4 view = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetLookAt();
	const glm::mat4 invView = glm::inverse(view);
	const glm::vec3 camPos = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetAbsoluteTransform().Translation;

	for (uint32_t i = 0; i < props.Height; ++i)
	{
		for (uint32_t j = 0; j < props.Width; ++j)
		{
			FinalImageData[(props.Width * i) + j] = PerPixel(j, i, props, invProj, invView, camPos);
		}
	}

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

void PathTracingRenderer::AddCommand(const RenderCommand& inCommand)
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
