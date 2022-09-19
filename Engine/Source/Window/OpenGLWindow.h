#pragma once
#include "WindowProperties.h"

class OpenGLWindow
{
public:
	OpenGLWindow(struct GLFWwindow* inWindowHandle = nullptr, const WindowProperties& inProperties = {});
	~OpenGLWindow();

	inline struct GLFWwindow* GetHandle() const { return WindowHandle; }

	void SetVSyncEnabled(const bool inEnabled);
	const WindowProperties& GetProperties() const { return Properties; }
	bool ShouldClose() const { return CloseRequested; }
	void RequestClose() { CloseRequested = true; }

private:
	struct GLFWwindow* WindowHandle;
	WindowProperties Properties;
	bool CloseRequested = false;
};