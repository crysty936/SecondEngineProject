#include "Window/WindowsWindow.h"
#include "Logger/Logger.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include <windows.h>
#include "Core/WindowsPlatform.h"
#include "InputSystem/InputSystem.h"

// Renderer should have the draw function adn get the window, while the window should contain it's context and other window related
// properties. Renderer should take the context and create use it to switch buffers, etc.
// Renderer should have most//all if possible OpenGL related functions.
// Maybe get the window when opening as well and just set it's properties after calling OpenGL related functions

// Maybe the renderer should return a new Window instance, returning the one that suits that specific renderer

// Window should be just the data holder and renderer should do all logic

WindowsWindow::WindowsWindow(const WindowProperties& inProperties /*= {}*/)
	: Properties{ inProperties }
{
	WindowHandle = WindowsPlatform::CreateWindowsWindow(inProperties.Width, inProperties.Height);

	ShowWindow(reinterpret_cast<HWND>(WindowHandle), SW_SHOW);
	UpdateWindow(reinterpret_cast<HWND>(WindowHandle));

	InputSystem::Get().OnKeyInput().BindRaw(this, &WindowsWindow::OnKeyInput);
}

WindowsWindow::~WindowsWindow()
{
	RemovePropW(reinterpret_cast<HWND>(WindowHandle), L"WindowProp");
	DestroyWindow(reinterpret_cast<HWND>(WindowHandle));
}

void WindowsWindow::OnKeyInput(const EInputKey inKey, const InputEventType inType)
{
	if (inKey == EInputKey::Escape)
	{
		CloseRequested = true;
	}
}

void WindowsWindow::SetVSyncEnabled(const bool inEnabled)
{
	Properties.VSyncEnabled = inEnabled;
	RHI->SetVSyncEnabled(inEnabled);
}

void WindowsWindow::SetCursorMode(const ECursorMode inMode)
{
	WindowsPlatform::SetCursorMode(WindowHandle, inMode);
}

