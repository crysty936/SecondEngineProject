#pragma once
#include "EASTL/unique_ptr.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/vector.h"
#include "EASTL/unordered_map.h"
#include "EASTL/string.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "Math/Transform.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "EventSystem/EventSystem.h"
#include "EASTL/queue.h"
#include "Core/EngineUtils.h"
#include "Entity/TransformObject.h"
#include "Renderer/RHI/Resources/MeshDataContainer.h"
#include "Window/WindowProperties.h"
#include "RenderCommand.h"

/**
 * TODO: Renderers should be differentiated between 2D and 3D because optimizations can be made for 2D only renderers (pixel perfect view)
 */

using LoadRenderResourceDelegate = Delegate<void, eastl::string, eastl::shared_ptr<class TransformObject>>;

struct RenderingLoadCommand
{
	LoadRenderResourceDelegate LoadDel;
	eastl::string ModelPath;
	eastl::shared_ptr<class TransformObject> Parent;
};

class ForwardRenderer
{
private:
	ForwardRenderer(const WindowProperties& inMainWindowProperties);
	virtual ~ForwardRenderer();

public:
	// Will create the base window and return the context for it
	static void Init(const WindowProperties& inMainWindowProperties = {});
	static void Terminate();
	void Draw();
	void Present();

	void DrawSkybox();
	void DrawShadowMap();
	void SetBaseUniforms();
	void UpdateUniforms();
	void DrawCommands(const eastl::vector<RenderCommand>& inCommands);
	void DrawCommand(const RenderCommand& inCommand);
	eastl::shared_ptr<RenderMaterial> GetMaterial(const RenderCommand& inCommand) const;
	void SetVSyncEnabled(const bool inEnabled);
	static void LoadTexture();
	void AddCommand(const RenderCommand& inCommand);
	void AddCommands(eastl::vector<RenderCommand> inCommands);
	void SetDrawMode(const EDrawMode::Type inDrawMode);
	void AddRenderLoadCommand(const RenderingLoadCommand& inCommand);

	//inline eastl::queue<RenderingLoadCommand>& GetLoadQueue() { return LoadQueue; }

	/**
	 * return: bool, true if the RenderDataContainer was already present and is initialized, false otherwise
	 * outContainer: newly created or existing cached Container
	 */
	bool GetOrCreateContainer(const eastl::string& inInstanceName, OUT eastl::shared_ptr<MeshDataContainer>& outContainer);
	void AddMirrorCommand(const RenderCommand& inCommand);
	//inline void SetSkyboxCommand(RenderCommand inSkyboxCommand) { MainSkyboxCommand = inSkyboxCommand; }

	inline static ForwardRenderer& Get() { ASSERT(Instance); return *Instance; }

	// Todo: Resolve hack
	eastl::weak_ptr<class RHITexture2D> GetDepthTexture() const;

	bool UpdateShadowMatrices = true;
	bool bCascadeVisualizeMode = false;

private:
	inline static ForwardRenderer* Instance = nullptr;

private:
	glm::mat4 CreateCascadeMatrix(const glm::mat4& inCameraProj, const glm::mat4& inCameraView, const glm::vec3& inLightDir);
	eastl::vector<glm::mat4> CreateCascadesMatrices();
	void SetViewportSizeToMain();
	void DrawDebugPoints();

	friend void DrawDebugPoint(const glm::vec3& inPointLoc);

private:
	mutable eastl::unordered_map<eastl::string, SelfRegisteringUniform> UniformsCache;
	eastl::vector<RenderCommand> MainCommands;
	EDrawMode::Type CurrentDrawMode = EDrawMode::Default;
	//eastl::queue<RenderingLoadCommand> LoadQueue;
	eastl::unordered_map<eastl::string, eastl::shared_ptr<class MeshDataContainer>> RenderDataContainerMap;
	int32_t CascadesCount = 3;
};

extern const uint32_t SHADOW_WIDTH;
extern const uint32_t SHADOW_HEIGHT;