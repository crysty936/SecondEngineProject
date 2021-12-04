#pragma once
#include "EventSystem/EventSystem.h"
#include "GLFW/glfw3.h"
#include "Core/EngineUtils.h"

using KeyDelegate = MulticastDelegate<int32_t, int32_t, int32_t, int32_t>;

class OpenGLInputSystem
{
private:
	OpenGLInputSystem();
	~OpenGLInputSystem();

public:
	static void Init(const class OpenGLWindow& inWindow);
	static inline OpenGLInputSystem& Get() { ASSERT(Instance); return *Instance; }

public:
	// Callbacks section for others to tie into
	KeyDelegate OnKeysPressed;

private:
	void OnKeyPressedTest(int32_t keycode, int32_t scanCode, int32_t action, int32_t mods);

private:
	static OpenGLInputSystem* Instance;
};
