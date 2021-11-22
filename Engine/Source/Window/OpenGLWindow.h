#pragma once
#include "WindowProperties.h"

class OpenGLWindow
{
public:
	OpenGLWindow(struct GLFWwindow* inWindowHandle, const WindowProperties& inProperties = {});
	~OpenGLWindow();
	void Open();

	inline struct GLFWwindow* GetHandle() const { return WindowHandle; }

	void SetVSyncEnabled(const bool inEnabled);

private:
	struct GLFWwindow* WindowHandle;
	WindowProperties Properties;
};