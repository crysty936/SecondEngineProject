#pragma once
#include <stdint.h>
#include "InputSystem/InputKeys.h"
#include "InputSystem/CursorMode.h"

enum class CLITextColor
{
	Red,
	Yellow,
	White
};

// Wrapper for windows.h to not pollute the dev environment
namespace WindowsPlatform
{
	void InitCycles();
	double GetTime();
	void Sleep(uint32_t inMilliseconds);
	void SetCLITextColor(CLITextColor inColor);
	EInputKey WindowsKeyToInternal(const int16_t inWindowsKey);
	void PoolMessages();
	void* CreateWindowsWindow(const int32_t desiredWidth, const int32_t desiredHeight);
	void SetCursorMode(void* inWindowHandle, const ECursorMode inMode);
}
