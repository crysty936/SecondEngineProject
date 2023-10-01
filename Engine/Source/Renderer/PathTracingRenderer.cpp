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

#include <algorithm>
#include <execution>
#include <random>

inline float random_float() {
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline float random_float(float min, float max) {
	// Returns a random real in [min,max).
	return min + (max - min) * random_float();
}

static glm::vec3 randomVec3() {
	return glm::vec3(random_float(), random_float(), random_float());
}

static glm::vec3 randomVec3(float min, float max) {
	return glm::vec3(random_float(min, max), random_float(min, max), random_float(min, max));
}

inline glm::vec3 random_in_unit_sphere() {
	while (true) {
		auto p = randomVec3(-1, 1);
		if (glm::lengthSquared(p)< 1)
			return p;
	}
}

inline glm::vec3 random_unit_vector() {
	return glm::normalize(random_in_unit_sphere());
}

inline glm::vec3 random_on_hemisphere(const glm::vec3& normal) {
	glm::vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return on_unit_sphere;
	else
		return -on_unit_sphere;
}

bool near_zero(glm::vec3 inVec) {
	// Return true if the vector is close to zero in all dimensions.
	auto s = 1e-8;
	return (fabs(inVec.x) < s) && (fabs(inVec.y) < s) && (fabs(inVec.z) < s);
}

static uint32_t ConvertToRGBA(const glm::vec4& color)
{
	uint8_t r = (uint8_t)(color.r * 255.0f);
	uint8_t g = (uint8_t)(color.g * 255.0f);
	uint8_t b = (uint8_t)(color.b * 255.0f);
	uint8_t a = (uint8_t)(color.a * 255.0f);

	uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
	return result;
}

PathTracingRenderer::~PathTracingRenderer() = default;

eastl::shared_ptr<FullScreenQuad> VisualizeQuad;
eastl::shared_ptr<RHITexture2D> RHITexture;

glm::vec4* AccumulationData;
uint32_t* FinalImageData;
uint32_t AccumulatedFramesCount = 1;
bool bUseAccumulation = true;

void PathTracingRenderer::InitInternal()
{
	const WindowsWindow& currentWindow = GEngine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();

	VisualizeQuad = SceneHelper::CreateObject<FullScreenQuad>("Quad");
	VisualizeQuad->CreateCommand();
	RHITexture = RHI::Get()->CreateTexture2D(props.Width, props.Height);

	VisualizeQuad->GetCommand().Material->ExternalTextures.push_back(RHITexture);
	AccumulationData = new glm::vec4[props.Width * props.Height];
	FinalImageData = new uint32_t[props.Width * props.Height];
}

struct Sphere
{
	glm::vec3 Origin = glm::vec3(0.f, 0.f, 0.f);
	float Radius = 0.f;
	glm::vec3 Color = glm::vec3(0.f, 0.f, 0.f);
	bool bIsEmissive = false;
};

struct PathTracingRay
{
	glm::vec3 Origin = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 Direction = glm::vec3(0.f, 0.f, 0.f);
};

struct Payload
{
	int32_t SphereIndex = -1;
	glm::vec3 Location = glm::vec3(0.f, 0.f, 0.f);
	float distance = 0.f;
};

static eastl::vector<Sphere> spheres = {
	{glm::vec3(0.f, 0.f, 0.f), 5.f, glm::vec3(1.f, 0.f, 0.f), false},
	{glm::vec3(15.f, 0.f, 0.f), 5.f, glm::vec3(0.0f, 0.2f, 0.5f), true},
	{glm::vec3(0.f, -50.f, 0.f), 45.f, glm::vec3(0.f, 1.f, 0.f), false}
};

Payload Trace(const PathTracingRay& inRay)
{
	int32_t closestSphere = -1;
	float closestDistance = std::numeric_limits<float>::max();
	for (int32_t i = 0; i < spheres.size(); ++i)
	{
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

		const Sphere& currentSphere = spheres[i];
		const glm::vec3 finalPosition = inRay.Origin - currentSphere.Origin; // Account for camera(ray origin)

		const float a = glm::dot(inRay.Direction, inRay.Direction);
		const float b = 2 * glm::dot(finalPosition, inRay.Direction);
		const float c = glm::dot(finalPosition, finalPosition) - (currentSphere.Radius * currentSphere.Radius);

		const float discriminant = sqrt((b * b) - (4 * a * c));

		if (discriminant > 0.f)
		{
			const float firstHitDistance = (-b - discriminant) / (2.f * a);
			//float secondhitDistance = (-b + discriminant) / (2.f * a);

			if (firstHitDistance > 0.f && firstHitDistance < closestDistance)
			{
				closestDistance = firstHitDistance;
				closestSphere = i;
			}
		}
	}

	glm::vec3 worldPosition = glm::vec3(0.f, 0.f, 0.f);
	if (closestSphere != -1)
	{
		worldPosition = inRay.Origin + (inRay.Direction * closestDistance);
	}

	const Payload result = {closestSphere, worldPosition, closestDistance};

	return result;
}

glm::vec3 DirLightDir = glm::vec3(0.f, 1.f, 0.8f);
glm::vec3 NormalizedDirLightDir;

glm::vec4 PathTracingRenderer::PerPixel(const uint32_t x, const uint32_t y, const WindowProperties& inProps, const glm::mat4& inInvProj, const glm::mat4& inInvView, const glm::vec3& inCamPos)
{
	glm::vec2 normalizedCoords = glm::vec2(float(x) / float(inProps.Width) , float(y) / float(inProps.Height) );
	normalizedCoords = normalizedCoords * 2.f - 1.f; // 0..1 -> -1..1

	glm::vec4 worldSpace = inInvProj * glm::vec4(normalizedCoords.x, normalizedCoords.y, 1.f, 1.f);
	worldSpace /= worldSpace.w;

	glm::vec3 rayDir = glm::normalize(glm::vec3(worldSpace));
	//const glm::vec3 pixelPos = glm::vec3(normalizedCoords.x , normalizedCoords.y, 0.f);
	//glm::vec3 rayDir = glm::normalize(glm::vec3(worldSpace) - pixelPos); // Same thing

	rayDir = glm::normalize(glm::vec3(inInvView * glm::vec4(rayDir.x, rayDir.y, rayDir.z, 0.f)));

	glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 0.f);
	PathTracingRay traceRay = { inCamPos, rayDir };

	float multiplier = 1.f;

	glm::vec3 SourceSurfaceNormal = glm::vec3(0.f, 1.f, 0.f);
	const int32_t nrSamples = 5;
	for (int32_t i = 0; i < nrSamples; ++i)
	{
		Payload result = Trace(traceRay);

		if (result.SphereIndex== -1)
		{
			if (i == 0)
			{
				float a = 0.5f * (rayDir.y + 1.f);
				const glm::vec3 skyColor = (1.f - a) * glm::vec3(1.f, 1.f, 1.f) + a * glm::vec3(0.5f, 0.7f, 1.f);
				color = glm::vec4(skyColor.x, skyColor.y, skyColor.z, 1.f);
			}
			else
			{
				const float cosNLightDir = glm::clamp(glm::dot(SourceSurfaceNormal, NormalizedDirLightDir), 0.1f, 1.f);
				const float occlusionScale = 1.f / float(i);

				color *= cosNLightDir * occlusionScale;
			}

			color = glm::pow(glm::vec4(color.x, color.y, color.z, color.w), glm::vec4(1.f/2.2f)); // Gamma correction

			return color;
		}
		else
		{
			const glm::vec3 SourceSurfaceNormal = glm::normalize(result.Location - spheres[result.SphereIndex].Origin);
			//const glm::vec3 newRayDir = glm::reflect(traceRay.Direction, SourceSurfaceNormal);
			
			glm::vec3 newRayDir = SourceSurfaceNormal + random_unit_vector(); // Lambertian diffuse

			if (near_zero(newRayDir))
			{
				newRayDir = SourceSurfaceNormal;
			}

			traceRay.Origin = result.Location + SourceSurfaceNormal * 0.0001f;
			traceRay.Direction = newRayDir;


			//const glm::vec3 newRayDir = SourceSurfaceNormal;


			//glm::vec3 visualNormal = SourceSurfaceNormal * 0.5f + 0.5f;
			//color = glm::vec4(visualNormal.x, visualNormal.y, visualNormal.z, 1.f);
			//color = glm::vec4(glm::clamp(result.Location.x, 0.f, 1.f), glm::clamp(result.Location.y, 0.f, 1.f), glm::clamp(result.Location.z, 0.f, 1.f), 1.f);
			//color = glm::vec4(result.Location.x, result.Location.y, result.Location.z, 1.f);
			//color = glm::vec4(result.distance/100.f, result.distance / 100.f, result.distance / 100.f, 1.f);

			const glm::vec3& hitSphereColor = spheres[result.SphereIndex].Color;
			const float cosNLightDir = glm::clamp(glm::dot(SourceSurfaceNormal, NormalizedDirLightDir), 0.1f, 1.f);
			color += glm::vec4(hitSphereColor.x, hitSphereColor.y, hitSphereColor.z, 0.f) * cosNLightDir * multiplier;

			multiplier *= 0.5f;
		}
	}

	color = glm::vec4(0.f, 0.f, 0.f, 1.f);

	return color;
}

eastl::vector<uint32_t> m_ImageHorizontalIter, m_ImageVerticalIter;

PathTracingRenderer::PathTracingRenderer(const WindowProperties& inMainWindowProperties)
	: Renderer(inMainWindowProperties)
{
	m_ImageHorizontalIter.resize(inMainWindowProperties.Width);
	m_ImageVerticalIter.resize(inMainWindowProperties.Height);

	for (uint32_t i = 0; i < inMainWindowProperties.Width; i++)
	{
		m_ImageHorizontalIter[i] = i;
	}

	for (uint32_t i = 0; i < inMainWindowProperties.Height; i++)
	{
		m_ImageVerticalIter[i] = i;
	}
}

void PathTracingRenderer::Draw()
{
	ImGui::Begin("Renderer settings");

	ImGui::Checkbox("Use Accumulation", &bUseAccumulation);

	if (bUseAccumulation)
	{
		++AccumulatedFramesCount;
	}
	else
	{
		AccumulatedFramesCount = 1;
	}

	int32_t sphereNr = 0;
	for (Sphere& sphere : spheres)
	{
		ImGui::PushID(sphereNr);
		ImGui::Text((eastl::string("Sphere ") + eastl::string(eastl::to_string(sphereNr))).c_str());

		//ImGui::TreeNode((eastl::string("Sphere #") + eastl::string(eastl::to_string(sphereNr))).c_str());
		ImGui::DragFloat3("Position", &sphere.Origin.x, 0.5f);
		ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
		//ImGui::TreePop();

		++sphereNr;
	}

	ImGui::DragFloat3("DirLightDir", &DirLightDir.x, 0.1f);
	NormalizedDirLightDir = glm::normalize(DirLightDir);

	const WindowsWindow& currentWindow = GEngine->GetMainWindow();
	const WindowProperties& props = currentWindow.GetProperties();

	const glm::mat4 projection = glm::perspectiveRH_ZO(glm::radians(CAMERA_FOV), props.AspectRatio, CAMERA_NEAR, CAMERA_FAR);
	const glm::mat4 invProj = glm::inverse(projection);

	const glm::vec3 camPos = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetAbsoluteTransform().Translation;

	glm::mat4 invView = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetAbsoluteTransform().GetMatrix();

	//const glm::quat& rot = SceneManager::Get().GetCurrentScene().GetCurrentCamera()->GetAbsoluteTransform().Rotation;
	//const glm::vec3 forward = glm::normalize(rot * glm::vec3(0.f, 0.f, 1.f));
	//const glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), forward, glm::vec3(0, 1, 0));
	//const glm::mat4 invView = glm::inverse(view);



	std::for_each(std::execution::par, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
		[this, props, invProj, invView, camPos](uint32_t i)
		{
			std::for_each(std::execution::par, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
			[this, i, props, invProj, invView, camPos](uint32_t j)
			{
				if(AccumulatedFramesCount == 1)
				{
					AccumulationData[(props.Width * i) + j] = glm::vec4(0.f, 0.f, 0.f, 0.f);
				}

				AccumulationData[(props.Width * i) + j] += PerPixel(j, i, props, invProj, invView, camPos);

				glm::vec4 finalColor = AccumulationData[(props.Width * i) + j] / glm::vec4(float(AccumulatedFramesCount));

				FinalImageData[(props.Width * i) + j] = ConvertToRGBA(finalColor);
			});
		});


// 	for (uint32_t i = 0; i < props.Height; ++i)
// 	{
// 		for (uint32_t j = 0; j < props.Width; ++j)
// 		{
// 			FinalImageData[(props.Width * i) + j] = PerPixel(j, i, props, invProj, invView, camPos);
// 		}
// 	}

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
