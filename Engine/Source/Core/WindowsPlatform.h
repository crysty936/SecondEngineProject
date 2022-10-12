#pragma once
#include <stdint.h>
#include "InputSystem/InputKeys.h"
#include "InputSystem/CursorMode.h"
#include "InputSystem/InputType.h"
#include "EASTL/string.h"

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
	void SetWindowsWindowText(const eastl::wstring& inText);

	class InputForwarder
	{
	public:
		static void ForwardKey(const EInputKey inKey, const EInputType inAction);
		static void ForwardMouseMove(double inNewYaw, double inNewPitch);
	};

}
