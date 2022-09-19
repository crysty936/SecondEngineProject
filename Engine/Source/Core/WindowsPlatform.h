#pragma once
#include <stdint.h>
#include "InputSystem/InputKeys.h"

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
}
