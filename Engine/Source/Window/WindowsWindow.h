#pragma once
#include "WindowProperties.h"
#include "InputSystem/CursorMode.h"
#include "InputSystem/InputKeys.h"
#include "InputSystem/InputEventType.h"

class WindowsWindow
{
public:
	WindowsWindow(const WindowProperties& inProperties = {});
	~WindowsWindow();

	void OnKeyInput(const EInputKey inKey, const InputEventType inType);
	void SetVSyncEnabled(const bool inEnabled);
	inline void* GetHandle() const { return WindowHandle; }
	const WindowProperties& GetProperties() const { return Properties; }
	bool ShouldClose() const { return CloseRequested; }
	void RequestClose() { CloseRequested = true; }
	void SetCursorMode(const ECursorMode inMode);

private:
	void* WindowHandle;
	WindowProperties Properties;
	bool CloseRequested = false;
};