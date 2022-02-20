#pragma once
#include "Window/OpenGLWindow.h"
#include "EASTL/unique_ptr.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/vector.h"
#include "EASTL/unordered_map.h"
#include "EASTL/string.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "Math/Transform.h"
#include "Renderer/SelfRegisteringUniforms/SelfRegisteringUniform.h"

/**
 * To make the renderer Generic, a base RHI class should be made, this should be renamed to OpenGLRHI
 */

class OpenGLRenderer
{
private:
	OpenGLRenderer(const WindowProperties& inDefaultWindowProperties);
	virtual ~OpenGLRenderer();

public:
	// Will create the base window and return the context for it
	static void Init(const WindowProperties& inDefaultWindowProperties = {});
	static void Terminate();
	void Draw();

	void SetupBaseUniforms();
	void UpdateUniforms();
	void RecursiveDrawObjects(const eastl::vector<eastl::shared_ptr<class Entity>>& inObjects);
	eastl::unique_ptr<OpenGLWindow> CreateWindow(const WindowProperties& inWindowProperties) const;
	void DestroyWindow(GLFWwindow* inWindowHandle) const;
	void SetVSyncEnabled(const bool inEnabled);
	inline class OpenGLWindow& GetMainWindow() { return *MainWindow; }

private:
	struct GLFWwindow* CreateNewWindowHandle(const WindowProperties& inWindowProperties) const;
	void CheckShouldCloseWindow(const OpenGLWindow& inWindow);

private:
	eastl::unique_ptr<class OpenGLWindow> MainWindow;
	eastl::unordered_map<eastl::string, SelfRegisteringUniform> UniformsCache;
};


extern OpenGLRenderer* RHI;