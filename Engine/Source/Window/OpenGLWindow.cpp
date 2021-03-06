#include "Window/OpenGLWindow.h"
#include <assert.h>
#include "Logger/Logger.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "GLFW/glfw3.h"

// Renderer should have the draw function adn get the window, while the window should contain it's context and other window related
// properties. Renderer should take the context and create use it to switch buffers, etc.
// Renderer should have most//all if possible OpenGL related functions.
// Maybe get the window when opening as well and just set it's properties after calling OpenGL related functions

// Maybe the renderer should return a new Window instance, returning the one that suits that specific renderer

// Window should be just the data holder and renderer should do all logic

OpenGLWindow::OpenGLWindow(GLFWwindow* inWindowHandle, const WindowProperties& inProperties /*= {}*/)
	: WindowHandle{inWindowHandle}, Properties{ inProperties }
{
	
}

OpenGLWindow::~OpenGLWindow()
{
	RHI->DestroyWindow(WindowHandle);
}


void OpenGLWindow::Open()
{
	LOG_INFO("Creating Window %s (%d, %d).", Properties.Title, Properties.Width, Properties.Height);



}

void OpenGLWindow::SetVSyncEnabled(const bool inEnabled)
{
	Properties.VSyncEnabled = inEnabled;
	RHI->SetVSyncEnabled(inEnabled);
}

