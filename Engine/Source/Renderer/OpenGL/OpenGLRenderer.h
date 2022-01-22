#pragma once
#include "Window/OpenGLWindow.h"
#include "EASTL/unique_ptr.h"
#include "EASTL/shared_ptr.h"
#include "EASTL/vector.h"
#include "glm/ext/matrix_float4x4.hpp"

/**
 * To make the renderer Generic, a base RHI class should be made, this should be renamed to OpenGLRHI,
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

	void RecursiveDrawObjects(eastl::vector<eastl::shared_ptr<class Entity>>& inObjects, const glm::mat4 inParentModel);
	eastl::unique_ptr<OpenGLWindow> CreateWindow(const WindowProperties& inWindowProperties) const;
	void DestroyWindow(GLFWwindow* inWindowHandle) const;
	void SetVSyncEnabled(const bool inEnabled);
	inline class OpenGLWindow& GetMainWindow() { return *MainWindow; }

private:
	struct GLFWwindow* CreateNewWindowHandle(const WindowProperties& inWindowProperties) const;
	void CheckShouldCloseWindow(const OpenGLWindow& inWindow);

private:
	eastl::unique_ptr<class OpenGLWindow> MainWindow;
	glm::mat4 Perspective;
};


extern OpenGLRenderer* RHI;