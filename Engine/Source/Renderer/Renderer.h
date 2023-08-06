#pragma once
#include "Window/WindowProperties.h"
#include "RenderCommand.h"
#include "SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "EASTL/unordered_map.h"
#include "EASTL/string.h"
#include "RHI/Resources/MeshDataContainer.h"

class Renderer
{
protected:
	Renderer(const WindowProperties& inMainWindowProperties);
	virtual ~Renderer();

public:
	// Will create the base window and return the context for it
	static void Init(const WindowProperties& inMainWindowProperties = {});
	static void Terminate();
	virtual void Draw() = 0;
	virtual void Present() = 0;

	inline static Renderer& Get() { ASSERT(Instance); return *Instance; }
	virtual void AddCommand(const RenderCommand& inCommand) = 0;
	virtual void AddDecalCommand(const RenderCommand& inCommand) = 0;
	virtual void AddCommands(eastl::vector<RenderCommand> inCommands) = 0;

	/**
	 * return: bool, true if the RenderDataContainer was already present and is initialized, false otherwise
	 * outContainer: newly created or existing cached Container
	 */
	virtual bool GetOrCreateContainer(const eastl::string& inInstanceName, OUT eastl::shared_ptr<MeshDataContainer>& outContainer) = 0;
	inline eastl::unordered_map<eastl::string, SelfRegisteringUniform>& GetUniformsCache() const { return UniformsCache; };

	virtual eastl::string GetMaterialsDirPrefix() = 0;

protected:
	virtual void InitInternal() = 0;
	void SetViewportSizeToMain();
	void SetBaseUniforms();


protected:
	inline static Renderer* Instance = nullptr;
	mutable eastl::unordered_map<eastl::string, SelfRegisteringUniform> UniformsCache;

	const float CAMERA_FOV = 45.f;
	const float CAMERA_NEAR = 0.1f;
	const float CAMERA_FAR = 10000.f;
};

