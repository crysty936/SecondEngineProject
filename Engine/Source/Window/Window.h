#pragma once
#include <string>


struct WindowProperties
{
	std::string_view Title = "MainWindow";
	uint32_t Width = 1280;
	uint32_t Height = 720;
};

class Window
{
public:
	Window(const WindowProperties& inProperties = {});
	~Window();
	void Open();

	template<typename T>
	T* GetHandle() const { return reinterpret_cast<T*>(WindowHandle); }

private:
	WindowProperties Properties;

private:
	// Handle for what type the RHI requires
	void* WindowHandle;
};