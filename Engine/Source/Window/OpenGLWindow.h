#pragma once
#include "WindowProperties.h"

class OpenGLWindow
{
public:
	OpenGLWindow(struct GLFWwindow* inWindowHandle, const WindowProperties& inProperties = {});
	~OpenGLWindow();

	inline struct GLFWwindow* GetHandle() const { return WindowHandle; }

	void SetVSyncEnabled(const bool inEnabled);
	const WindowProperties& GetProperties() const { return Properties; }

private:
	struct GLFWwindow* WindowHandle;
	WindowProperties Properties;
};