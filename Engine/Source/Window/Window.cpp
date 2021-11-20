#include "Window/Window.h"
#include <assert.h>
#include "Logger/Logger.h"
#include "Renderer/Renderer.h"

// Renderer should have the draw function adn get the window, while the window should contain it's context and other window related
// properties. Renderer should take the context and create use it to switch buffers, etc.
// Renderer should have most//all if possible OpenGL related functions.
// Maybe get the window when opening as well and just set it's properties after calling OpenGL related functions

Window::Window(const WindowProperties& inProperties /*= {}*/)
	: Properties{ inProperties }
{

// 	WindowHandle = RHI.CreateWindow(inProperties.Width, inProperties.Height, inProperties.Title.data());
// 	assert(WindowHandle);
}

Window::~Window()
{
	RHI.DestroyWindow(WindowHandle);
}


void Window::Open()
{
	Logger::Get().Print("Creating Window %s (%d, %d).", Properties.Title, Properties.Width, Properties.Height);






}

