#pragma once
#include "Window/Window.h"

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
	virtual void Init();
	void Draw(const class Window& inWindowToUse);

	void* CreateWindow(const int32_t inWidth, const int32_t inHeight, const char* inWindowTitle);
	void DestroyWindow(void* inWindowHandle) const;

};

// Convert to pointer for generic RHI implementation
extern Renderer RHI;