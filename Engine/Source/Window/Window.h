#pragma once
#include "WindowProperties.h"

class Window
{
public:
	Window(struct GLFWwindow* inWindowHandle, const WindowProperties& inProperties = {});
	~Window();
	void Open();

	inline struct GLFWwindow* GetHandle() const { return WindowHandle; }


private:
	// Handle for what type the RHI requires
	struct GLFWwindow* WindowHandle;
	WindowProperties Properties;
};