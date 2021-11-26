#pragma once
#include "EventSystem/EventSystem.h"
#include "GLFW/glfw3.h"
#include "Core/EngineUtils.h"

using KeyDelegate = MulticastDelegate<int, int, int, int>;

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
	void OnKeyPressedTest(int keycode, int scanCode, int action, int mods);

private:
	static OpenGLInputSystem* Instance;
};
