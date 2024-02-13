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

class TestRenderer : public Renderer
{
protected:
	TestRenderer(const WindowProperties& inMainWindowProperties);
	virtual ~TestRenderer();

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

//private:
	//void UpdateUniforms();
//	void SetViewportSizeToMain();

private:
	//eastl::vector<RenderCommand> MainCommands;
	//eastl::vector<RenderCommand> DecalCommands;
	//EDrawMode::Type CurrentDrawMode = EDrawMode::Default;
	//eastl::unordered_map<eastl::string, eastl::shared_ptr<class MeshDataContainer>> RenderDataContainerMap;


	friend Renderer;
};

