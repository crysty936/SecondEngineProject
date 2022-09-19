#pragma once
#include "Window/OpenGLWindow.h"
#include "EASTL/unique_ptr.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/vector.h"
#include "EASTL/unordered_map.h"
#include "EASTL/string.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "Math/Transform.h"
#include "Renderer/SelfRegisteringUniform/SelfRegisteringUniform.h"
#include "RenderCommand.h"
#include "EventSystem/EventSystem.h"
#include "EASTL/queue.h"
#include "Core/EngineUtils.h"
#include "Entity/TransformObject.h"

/**
 * TODO: A Renderer should be made and that should call whatever RHI is present.
 * Also, Renderers should be differentiated between 2D and 3D because optimizations can be made for 2D only renderers
 */

using LoadRenderResourceDelegate = Delegate<void, eastl::string, eastl::shared_ptr<class TransformObject>>;

struct RenderingLoadCommand
{
	LoadRenderResourceDelegate LoadDel;
	eastl::string ModelPath;
	eastl::shared_ptr<class TransformObject> Parent;
};

extern class OpenGLRenderer* RHI;

enum class EDrawMode : uint8_t
{
	NORMAL,
	DEPTH,
	DEPTH_VISUALIZE,
	OUTLINE
};

class OpenGLRenderer
{
private:
	OpenGLRenderer(const WindowProperties& inMainWindowProperties);
	virtual ~OpenGLRenderer();

public:
	// Will create the base window and return the context for it
	static void Init(const WindowProperties& inMainWindowProperties = {});
	static void Terminate();
	void Draw();

	void DrawSkybox();
	void DrawShadowMap();
	void SetupBaseUniforms();
	void UpdateUniforms();
	void DrawCommands(const eastl::vector<RenderCommand>& inCommands);
	void DrawCommand(const RenderCommand& inCommand);
	eastl::shared_ptr<RenderMaterial> GetMaterial(const RenderCommand& inCommand) const;
	void SetVSyncEnabled(const bool inEnabled);
	inline class OpenGLWindow& GetMainWindow() { return *MainWindow; }
	static void LoadTexture();
	void AddCommand(const RenderCommand& inCommand);
	void AddCommands(eastl::vector<RenderCommand> inCommands);
	inline void SetDrawMode(const EDrawMode inDrawMode) { DrawMode = inDrawMode; }
	void AddRenderLoadCommand(const RenderingLoadCommand& inCommand);
	inline eastl::queue<RenderingLoadCommand>& GetLoadQueue() { return LoadQueue; }

	//************************************
	// Returns:   bool, true if the VAO was already present and is initialized, false otherwise
	// Parameter: eastl::shared_ptr<VertexArrayObject> & outVAO, newly created or cached existing VAO
	//************************************
	bool GetOrCreateVAO(const eastl::string& inVAOId, OUT eastl::shared_ptr<VertexArrayObject>& outVAO);
	void AddMirrorCommand(const RenderCommand& inCommand);
	inline void SetSkyboxCommand(RenderCommand inSkyboxCommand) { MainSkyboxCommand = inSkyboxCommand; }

private:
	void SetViewportSize(const int32_t inWidth, const int32_t inHeight);
	void SetViewportSizeToMain();
	void CheckShouldCloseWindow(const OpenGLWindow& inWindow);

	eastl::unique_ptr<class WindowsWindow> NewWindow;
private:
	eastl::unique_ptr<class OpenGLWindow> MainWindow;
	eastl::unordered_map<eastl::string, SelfRegisteringUniform> UniformsCache;
	eastl::vector<RenderCommand> MainCommands;
	EDrawMode DrawMode{ EDrawMode::NORMAL };
	eastl::queue<RenderingLoadCommand> LoadQueue;
	eastl::unordered_map<eastl::string, eastl::shared_ptr<class VertexArrayObject>> VAOs;
	uint32_t AuxiliarFrameBuffer;
	uint32_t ShadowMapBuffer;
	eastl::shared_ptr<class OpenGLDepthMap> ShadowBufferTex;
	RenderCommand MainSkyboxCommand;
};

extern GLFWwindow* LoadingThreadContext;
extern const uint32_t SHADOW_WIDTH;
extern const uint32_t SHADOW_HEIGHT;