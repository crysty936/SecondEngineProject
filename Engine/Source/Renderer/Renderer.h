#pragma once
#include "Window/Window.h"
#include "EASTL/shared_ptr.h"

/**
 * To make the renderer Generic, a base RHI class should be made, this should be renamed to OpenGLRHI,
 * A base class should be made for the WindowContext, present one renamed to OpenGL Context, same for Window then the new implementation defined.
 */

class Renderer
{
public:
	Renderer();
	virtual ~Renderer();

public:
	// Will create the base window and return the context for it
	virtual eastl::shared_ptr<Window> Init(const WindowProperties& inDefaultWindowProperties);
	void Draw(const class Window& inWindowToUse);

	eastl::shared_ptr<Window> CreateWindow(const WindowProperties& inWindowProperties) const;
	void DestroyWindow(GLFWwindow* inWindowHandle) const;

private:
	struct GLFWwindow* CreateNewWinowHandle(const WindowProperties& inWindowProperties) const;

};

// Convert to pointer for generic RHI implementation
extern Renderer RHI;