#pragma once
#include "Renderer.h"
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

class PathTracingRenderer : public Renderer
{
protected:
	PathTracingRenderer(const WindowProperties& inMainWindowProperties);
	virtual ~PathTracingRenderer();

public:
	virtual void Draw() override;
	virtual void Present() override;

	virtual void AddCommand(const RenderCommand& inCommand) override;
	virtual void AddDecalCommand(const RenderCommand& inCommand) override;
	virtual void AddCommands(eastl::vector<RenderCommand> inCommands) override;

	/**
	 * return: bool, true if the RenderDataContainer was already present and is initialized, false otherwise
	 * outContainer: newly created or existing cached Container
	 */
	virtual bool GetOrCreateContainer(const eastl::string& inInstanceName, OUT eastl::shared_ptr<MeshDataContainer>& outContainer) override;


	eastl::string GetMaterialsDirPrefix() override;



protected:
	void InitInternal() override;

	__forceinline uint32_t PerPixel(const uint32_t x, const uint32_t y, const WindowProperties& inProps, const glm::mat4& inInvProj, const glm::mat4& inInvView, const glm::vec3& inCamPos);
	void DrawCommand(const RenderCommand& inCommand);
	void SetViewportSizeToMain();

private:
	eastl::vector<RenderCommand> MainCommands;
	eastl::vector<RenderCommand> DecalCommands;
	eastl::unordered_map<eastl::string, eastl::shared_ptr<class MeshDataContainer>> RenderDataContainerMap;

	friend Renderer;
};

