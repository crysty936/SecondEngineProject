#pragma once
#include "Window/OpenGLWindow.h"
#include "EASTL/shared_ptr.h"

/**
 * To make the renderer Generic, a base RHI class should be made, this should be renamed to OpenGLRHI,
 * A base class should be made for the WindowContext, present one renamed to OpenGL Context, same for Window then the new implementation defined.
 */

class OpenGLRenderer
{
public:
	OpenGLRenderer();
	virtual ~OpenGLRenderer();

public:
	// Will create the base window and return the context for it
	virtual void Init(const WindowProperties& inDefaultWindowProperties);
	void Terminate();
	void Draw();

	eastl::shared_ptr<OpenGLWindow> CreateWindow(const WindowProperties& inWindowProperties) const;
	void DestroyWindow(GLFWwindow* inWindowHandle) const;
	void SetVSyncEnabled(const bool inEnabled);
	inline class OpenGLWindow& GetMainWindow() { return *MainWindow; }


private:
	struct GLFWwindow* CreateNewWinowHandle(const WindowProperties& inWindowProperties) const;
	void CheckShouldCloseWindow(const OpenGLWindow& inWindow);

private:
	eastl::shared_ptr<class OpenGLWindow> MainWindow;

};

// Convert to pointer for generic RHI implementation
extern OpenGLRenderer RHI;