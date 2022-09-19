#include "Core/WindowsPlatform.h"
#include <windows.h>
#include <WinUser.h>
#include "Logger/Logger.h"
#include "InputSystem/InputEventType.h"
#include "InputSystem/InputSystem.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Window/WindowsWindow.h"
#include <atlbase.h>
#include <atlwin.h>
#include "EASTL/string.h"

// Wrapper over Windows.h functionality
namespace WindowsPlatform
{
	 // Timing

	static double SecondsPerCycle = 0;

	void InitCycles()
	{
		// Set the main thread to work only on one core to avoid timing synchrionization problems between multiple cores
		// Source: https://docs.microsoft.com/en-us/windows/win32/dxtecharts/game-timing-and-multicore-processors
		DWORD_PTR threadMask = static_cast<DWORD_PTR>(1);
		::SetThreadAffinityMask(::GetCurrentThread(), threadMask);

		// Initialise required frequency for seconds calc
		::LARGE_INTEGER Frequency;
		::QueryPerformanceFrequency(&Frequency);
		SecondsPerCycle = 1.0 / Frequency.QuadPart;
	}

	double GetTime()
	{
		// Making use of Windows Platform functions for getting the time
		::LARGE_INTEGER cycles;
		::QueryPerformanceCounter(&cycles);

		return cycles.QuadPart * SecondsPerCycle + 16777216.0;
	}

 	void Sleep(uint32_t inMilliseconds)
 	{
 		::Sleep(inMilliseconds);
 	}

	// CLI

	void SetCLITextColor(CLITextColor inColor)
	{
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		WORD color = 0;

		switch (inColor)
		{
		case CLITextColor::Red:
		{
			color = FOREGROUND_RED;
			break;
		}
		case CLITextColor::Yellow:
		{
			color = FOREGROUND_RED | FOREGROUND_GREEN;
			break;
		}
		case CLITextColor::White:
		{
			color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
			break;
		}
		}

		SetConsoleTextAttribute(hStdOut, color);
	}

	// Input

	class WindowsKeysContainer
	{
	public:
		WindowsKeysContainer()
		{
			Codes[0x00B] = +EInputKey::D0;
			Codes[0x002] = +EInputKey::D1;
			Codes[0x003] = +EInputKey::D2;
			Codes[0x004] = +EInputKey::D3;
			Codes[0x005] = +EInputKey::D4;
			Codes[0x006] = +EInputKey::D5;
			Codes[0x007] = +EInputKey::D6;
			Codes[0x008] = +EInputKey::D7;
			Codes[0x009] = +EInputKey::D8;
			Codes[0x00A] = +EInputKey::D9;
			Codes[0x01E] = +EInputKey::A;
			Codes[0x030] = +EInputKey::B;
			Codes[0x02E] = +EInputKey::C;
			Codes[0x020] = +EInputKey::D;
			Codes[0x012] = +EInputKey::E;
			Codes[0x021] = +EInputKey::F;
			Codes[0x022] = +EInputKey::G;
			Codes[0x023] = +EInputKey::H;
			Codes[0x017] = +EInputKey::I;
			Codes[0x024] = +EInputKey::J;
			Codes[0x025] = +EInputKey::K;
			Codes[0x026] = +EInputKey::L;
			Codes[0x032] = +EInputKey::M;
			Codes[0x031] = +EInputKey::N;
			Codes[0x018] = +EInputKey::O;
			Codes[0x019] = +EInputKey::P;
			Codes[0x010] = +EInputKey::Q;
			Codes[0x013] = +EInputKey::R;
			Codes[0x01F] = +EInputKey::S;
			Codes[0x014] = +EInputKey::T;
			Codes[0x016] = +EInputKey::U;
			Codes[0x02F] = +EInputKey::V;
			Codes[0x011] = +EInputKey::W;
			Codes[0x02D] = +EInputKey::X;
			Codes[0x015] = +EInputKey::Y;
			Codes[0x02C] = +EInputKey::Z;

			Codes[0x028] = +EInputKey::Apostrophe;
			Codes[0x02B] = +EInputKey::Backslash;
			Codes[0x033] = +EInputKey::Comma;
			Codes[0x00D] = +EInputKey::Equal;
			Codes[0x029] = +EInputKey::GraveAccent;
			Codes[0x01A] = +EInputKey::LeftBracket;
			Codes[0x00C] = +EInputKey::Minus;
			Codes[0x034] = +EInputKey::Period;
			Codes[0x01B] = +EInputKey::RightBracket;
			Codes[0x027] = +EInputKey::Semicolon;
			Codes[0x035] = +EInputKey::Slash;
			Codes[0x056] = +EInputKey::World2;

			Codes[0x00E] = +EInputKey::Backspace;
			Codes[0x153] = +EInputKey::Delete;
			Codes[0x14F] = +EInputKey::End;
			Codes[0x01C] = +EInputKey::Enter;
			Codes[0x001] = +EInputKey::Escape;
			Codes[0x147] = +EInputKey::Home;
			Codes[0x152] = +EInputKey::Insert;
			Codes[0x15D] = +EInputKey::Menu;
			Codes[0x151] = +EInputKey::PageDown;
			Codes[0x149] = +EInputKey::PageUp;
			Codes[0x045] = +EInputKey::Pause;
			Codes[0x146] = +EInputKey::Pause;
			Codes[0x039] = +EInputKey::Space;
			Codes[0x00F] = +EInputKey::Tab;
			Codes[0x03A] = +EInputKey::CapsLock;
			Codes[0x145] = +EInputKey::NumLock;
			Codes[0x046] = +EInputKey::ScrollLock;
			Codes[0x03B] = +EInputKey::F1;
			Codes[0x03C] = +EInputKey::F2;
			Codes[0x03D] = +EInputKey::F3;
			Codes[0x03E] = +EInputKey::F4;
			Codes[0x03F] = +EInputKey::F5;
			Codes[0x040] = +EInputKey::F6;
			Codes[0x041] = +EInputKey::F7;
			Codes[0x042] = +EInputKey::F8;
			Codes[0x043] = +EInputKey::F9;
			Codes[0x044] = +EInputKey::F10;
			Codes[0x057] = +EInputKey::F11;
			Codes[0x058] = +EInputKey::F12;
			Codes[0x064] = +EInputKey::F13;
			Codes[0x065] = +EInputKey::F14;
			Codes[0x066] = +EInputKey::F15;
			Codes[0x067] = +EInputKey::F16;
			Codes[0x068] = +EInputKey::F17;
			Codes[0x069] = +EInputKey::F18;
			Codes[0x06A] = +EInputKey::F19;
			Codes[0x06B] = +EInputKey::F20;
			Codes[0x06C] = +EInputKey::F21;
			Codes[0x06D] = +EInputKey::F22;
			Codes[0x06E] = +EInputKey::F23;
			Codes[0x076] = +EInputKey::F24;
			Codes[0x038] = +EInputKey::LeftAlt;
			Codes[0x01D] = +EInputKey::LeftControl;
			Codes[0x02A] = +EInputKey::LeftShift;
			Codes[0x15B] = +EInputKey::LeftSuper;
			Codes[0x137] = +EInputKey::PrintScreen;
			Codes[0x138] = +EInputKey::RightAlt;
			Codes[0x11D] = +EInputKey::RightControl;
			Codes[0x036] = +EInputKey::RightShift;
			Codes[0x15C] = +EInputKey::RightSuper;
			Codes[0x150] = +EInputKey::Down;
			Codes[0x14B] = +EInputKey::Left;
			Codes[0x14D] = +EInputKey::Right;
			Codes[0x148] = +EInputKey::Up;

			Codes[0x052] = +EInputKey::KP0;
			Codes[0x04F] = +EInputKey::KP1;
			Codes[0x050] = +EInputKey::KP2;
			Codes[0x051] = +EInputKey::KP3;
			Codes[0x04B] = +EInputKey::KP4;
			Codes[0x04C] = +EInputKey::KP5;
			Codes[0x04D] = +EInputKey::KP6;
			Codes[0x047] = +EInputKey::KP7;
			Codes[0x048] = +EInputKey::KP8;
			Codes[0x049] = +EInputKey::KP9;
			Codes[0x04E] = +EInputKey::KPAdd;
			Codes[0x053] = +EInputKey::KPDecimal;
			Codes[0x135] = +EInputKey::KPDivide;
			Codes[0x11C] = +EInputKey::KPEnter;
			Codes[0x059] = +EInputKey::KPEqual;
			Codes[0x037] = +EInputKey::KPMultiply;
			Codes[0x04A] = +EInputKey::KPSubtract;
		}

		int16_t Codes[512];

		int16_t GetCode(const int16_t inCode)
		{
			return Codes[inCode];
		}
	};

	WindowsKeysContainer KeysContainer;

	EInputKey WindowsKeyToInternal(const int16_t inWindowsKey)
	{
		return static_cast<EInputKey>(KeysContainer.GetCode(inWindowsKey));
	}


	void UpdateClipRect(HWND inWindowHandle)
	{
		if (inWindowHandle)
		{
			RECT clipRect;
			GetClientRect(inWindowHandle, &clipRect);
			ClientToScreen(inWindowHandle, (POINT*)&clipRect.left);
			ClientToScreen(inWindowHandle, (POINT*)&clipRect.right);
			ClipCursor(&clipRect);
		}
		else
		{
			ClipCursor(nullptr);
		}
	}

	bool IsWindowFocused(HWND inWindowHandle)
	{
		return inWindowHandle == GetActiveWindow();
	}

	void GetCursorPos(HWND inWindowHandle, OUT glm::vec<2, int>& outPos)
	{
		POINT pos;

		if (GetCursorPos(&pos))
		{
			ScreenToClient(inWindowHandle, &pos);

			outPos.x = pos.x;
			outPos.y = pos.y;
		}
	}

	void SetCursorPos(HWND inWindowHandle, const glm::vec<2, int>& inPos)
	{
		POINT pos = { inPos.x, inPos.y };

		// Store the new position so it can be recognized later
		InputSystem::Get().LastCursorPos.x = pos.x;
		InputSystem::Get().LastCursorPos.y = pos.y;
	// 	window->win32.lastCursorPosX = pos.x;
	// 	window->win32.lastCursorPosY = pos.y;

		ClientToScreen(inWindowHandle, &pos);
		::SetCursorPos(pos.x, pos.y);
	}

	void DisableCursor(HWND inWindowHandle)
	{
		GetCursorPos(inWindowHandle, InputSystem::Get().CurrentCursorPos);
		SetCursor(NULL);

		RECT area;
		GetClientRect(inWindowHandle, &area);
		int width, height;
		width = area.right;
		height = area.bottom;

		POINT pos = { (int)width / 2, (int)height / 2 };
		ClientToScreen(inWindowHandle, &pos);
		::SetCursorPos(pos.x, pos.y);

		UpdateClipRect(inWindowHandle);
	}

	void EnableCursor(HWND inWindowHandle)
	{
		UpdateClipRect(nullptr);

		SetCursorPos(inWindowHandle, InputSystem::Get().CurrentCursorPos);
		SetCursor(nullptr);
	}

	void SetCursorMode(void* inWindowHandle, const ECursorMode inMode)
	{
		if (inMode == ECursorMode::Disabled)
		{
			if (IsWindowFocused(reinterpret_cast<HWND>(inWindowHandle)))
			{
				DisableCursor(reinterpret_cast<HWND>(inWindowHandle));
			}
		}
		//else if (_glfw.win32.disabledCursorWindow == window)
			//enableCursor(window);
		//else if (cursorInContentArea(window))
			//updateCursorImage(window);
	}

	void SetWindowsWindowText(const eastl::string& inText)
	{
		SetWindowText(reinterpret_cast<HWND>(RHI->GetMainWindow().GetHandle()), inText.c_str());
	}
	// Message Loop

	void PoolMessages()
	{
		MSG msg = {};

		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

#define LOG_WINMSG(MsgType) LOG_INFO("Windows Message: %s", #MsgType)

	LRESULT WindowMessageLoop(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
	{
		// Forward hwnd on because we can get messages (e.g., WM_CREATE)
		// before CreateWindow returns, and thus before mhMainWnd is valid.

		// This is set as 1 after the window's creation
		void* windowValid = GetPropW(hwnd, L"WindowProp");

		if (!windowValid)
		{
			// This is the message handling for the dummy window
			// and for a regular window during its initial creation

			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		// Whole glfw message loop but use only what's needed
		switch (msg)
		{
		case WM_MOUSEACTIVATE:
		{
			LOG_WINMSG(WM_MOUSEACTIVATE);

			// HACK: Postpone cursor disabling when the window was activated by
			// clicking a caption button
			if (HIWORD(lParam) == WM_LBUTTONDOWN)
			{
				if (LOWORD(lParam) != HTCLIENT)
				{
					InputSystem::Get().OngoingFrameAction = true;
				}
			}

			break;
		}

		case WM_CAPTURECHANGED:
		{
			LOG_WINMSG(WM_CAPTURECHANGED);
			// HACK: Disable the cursor once the caption button action has been
			// completed or cancelled
			if (lParam == 0 && InputSystem::Get().OngoingFrameAction)
			{
				if (InputSystem::Get().CurrentCursorMode == ECursorMode::Disabled)
				{
					DisableCursor(hwnd);
				}

				InputSystem::Get().OngoingFrameAction = false;
			}

			break;
		}

		case WM_SETFOCUS:
		{
			LOG_WINMSG(WM_SETFOCUS);
			// _glfwInputWindowFocus(window, GLFW_TRUE);
			// HACK: Do not disable cursor while the user is interacting with
			//       a caption button
			if (InputSystem::Get().OngoingFrameAction)
				break;

			if (InputSystem::Get().CurrentCursorMode == ECursorMode::Disabled)
			{
				DisableCursor(hwnd);
			}


			return 0;
		}

		case WM_KILLFOCUS:
		{
			LOG_WINMSG(WM_KILLFOCUS);
			if (InputSystem::Get().CurrentCursorMode == ECursorMode::Disabled)
			{
				EnableCursor(hwnd);
			}
			// 
			//             if (window->monitor && window->autoIconify)
			//                 _glfwPlatformIconifyWindow(window);
			// 
			//             _glfwInputWindowFocus(window, GLFW_FALSE);

			return 0;
		}

		case WM_SYSCOMMAND:
		{
			LOG_WINMSG(WM_SYSCOMMAND);
			switch (wParam & 0xfff0)
			{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
			{
				LOG_WINMSG(SC_SCREENSAVE - SC_MONITORPOWER);
//				if (window->monitor)
//				{
//					// We are running in full screen mode, so disallow
//					// screen saver and screen blanking
//					return 0;
//              }
				break;
			}

			// User trying to access application menu using ALT?
			case SC_KEYMENU:
			{
				LOG_WINMSG(SC_KEYMENU);

				return 0;
			}
			}
			break;
		}

		case WM_CLOSE:
		{
			LOG_WINMSG(WM_CLOSE);
			RHI->GetMainWindow().RequestClose();

			return 0;
		}

		case WM_INPUTLANGCHANGE:
		{
			LOG_WINMSG(WM_INPUTLANGCHANGE);
			//_glfwUpdateKeyNamesWin32();
			break;
		}

		case WM_CHAR:
		case WM_SYSCHAR:
		{
			LOG_WINMSG(WM_CHAR - WM_SYSCHAR);
			// if (wParam >= 0xd800 && wParam <= 0xdbff)
//                 window->win32.highSurrogate = (WCHAR) wParam;
//             else
//             {
//                 unsigned int codepoint = 0;
// 
//                 if (wParam >= 0xdc00 && wParam <= 0xdfff)
//                 {
//                     if (window->win32.highSurrogate)
//                     {
//                         codepoint += (window->win32.highSurrogate - 0xd800) << 10;
//                         codepoint += (WCHAR) wParam - 0xdc00;
//                         codepoint += 0x10000;
//                     }
//                 }
//                 else
//                     codepoint = (WCHAR) wParam;
// 
//                 window->win32.highSurrogate = 0;
//                 _glfwInputChar(window, codepoint, getKeyMods(), uMsg != WM_SYSCHAR);
//             }

			return 0;
		}

		case WM_UNICHAR:
		{
			LOG_WINMSG(WM_UNICHAR);
			if (wParam == UNICODE_NOCHAR)
			{
				// WM_UNICHAR is not sent by Windows, but is sent by some
				// third-party input method engine
				// Returning TRUE here announces support for this message
				return TRUE;
			}

			//_glfwInputChar(window, (unsigned int) wParam, getKeyMods(), GLFW_TRUE);
			return 0;
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			LOG_WINMSG(WM_KEYDOWN - WM_SYSKEYDOWN - WM_KEYUP - WM_SYSKEYUP);
			int32_t scancode = 0;
			EInputKey key = EInputKey::None;
			const InputEventType action = (HIWORD(lParam) & KF_UP) ? InputEventType::InputRelease : InputEventType::InputPress;
			//             const int mods = getKeyMods();
			// 
			scancode = (HIWORD(lParam) & (KF_EXTENDED | 0xff));
			if (!scancode)
			{
				// NOTE: Some synthetic key messages have a scancode of zero
				// HACK: Map the virtual key back to a usable scancode
				scancode = MapVirtualKeyW((UINT)wParam, MAPVK_VK_TO_VSC);
			}

			key = WindowsPlatform::WindowsKeyToInternal(scancode);

			// The Ctrl keys require special handling
			if (wParam == VK_CONTROL)
			{
				if (HIWORD(lParam) & KF_EXTENDED)
				{
					// Right side keys have the extended key bit set
					key = EInputKey::RightControl;
				}
				else
				{
					// NOTE: Alt Gr sends Left Ctrl followed by Right Alt
					// HACK: We only want one event for Alt Gr, so if we detect
					//       this sequence we discard this Left Ctrl message now
					//       and later report Right Alt normally
					MSG next;
					const DWORD time = GetMessageTime();

					if (PeekMessageW(&next, NULL, 0, 0, PM_NOREMOVE))
					{
						if (next.message == WM_KEYDOWN ||
							next.message == WM_SYSKEYDOWN ||
							next.message == WM_KEYUP ||
							next.message == WM_SYSKEYUP)
						{
							if (next.wParam == VK_MENU &&
								(HIWORD(next.lParam) & KF_EXTENDED) &&
								next.time == time)
							{
								// Next message is Right Alt down so discard this
								break;
							}
						}
					}

					// This is a regular Left Ctrl message
					key = EInputKey::LeftControl;
				}
			}
			else if (wParam == VK_PROCESSKEY)
			{
				// IME notifies that keys have been filtered by setting the
				// virtual key-code to VK_PROCESSKEY
				break;
			}

			if (action == InputEventType::InputRelease && wParam == VK_SHIFT)
			{
				// HACK: Release both Shift keys on Shift up event, as when both
				//       are pressed the first release does not emit any event
				// NOTE: The other half of this is in _glfwPlatformPollEvents
				//_glfwInputKey(window, GLFW_KEY_LEFT_SHIFT, scancode, action, mods);
				//_glfwInputKey(window, GLFW_KEY_RIGHT_SHIFT, scancode, action, mods);
			}
			else if (wParam == VK_SNAPSHOT)
			{
				// HACK: Key down is not reported for the Print Screen key
				//_glfwInputKey(window, key, scancode, GLFW_PRESS, mods);
				//_glfwInputKey(window, key, scancode, GLFW_RELEASE, mods);
			}
			else
			{
				InputForwarder::ForwardKey(key, action);
			}

			break;
		}

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
		{
			LOG_WINMSG(WM_LBUTTONDOWN - WM_RBUTTONDOWN - WM_MBUTTONDOWN - WM_XBUTTONDOWN - WM_LBUTTONUP - WM_RBUTTONUP - WM_MBUTTONUP - WM_XBUTTONUP);
			//             int i, button, action;
// 
//             if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP)
//                 button = GLFW_MOUSE_BUTTON_LEFT;
//             else if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP)
//                 button = GLFW_MOUSE_BUTTON_RIGHT;
//             else if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONUP)
//                 button = GLFW_MOUSE_BUTTON_MIDDLE;
//             else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
//                 button = GLFW_MOUSE_BUTTON_4;
//             else
//                 button = GLFW_MOUSE_BUTTON_5;
// 
//             if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN ||
//                 uMsg == WM_MBUTTONDOWN || uMsg == WM_XBUTTONDOWN)
//             {
//                 action = GLFW_PRESS;
//             }
//             else
//                 action = GLFW_RELEASE;
// 
//             for (i = 0;  i <= GLFW_MOUSE_BUTTON_LAST;  i++)
//             {
//                 if (window->mouseButtons[i] == GLFW_PRESS)
//                     break;
//             }
// 
//             if (i > GLFW_MOUSE_BUTTON_LAST)
//                 SetCapture(hWnd);
// 
//             _glfwInputMouseClick(window, button, action, getKeyMods());
// 
//             for (i = 0;  i <= GLFW_MOUSE_BUTTON_LAST;  i++)
//             {
//                 if (window->mouseButtons[i] == GLFW_PRESS)
//                     break;
//             }
// 
//             if (i > GLFW_MOUSE_BUTTON_LAST)
//                 ReleaseCapture();
// 
//             if (uMsg == WM_XBUTTONDOWN || uMsg == WM_XBUTTONUP)
//                 return TRUE;

			return 0;
		}

		case WM_MOUSEMOVE:
		{
			//LOG_WINMSG(WM_MOUSEMOVE);

			const int x = GET_X_LPARAM(lParam);
			const int y = GET_Y_LPARAM(lParam);
// 
			if (!InputSystem::Get().CursorsTracked)
			{
				TRACKMOUSEEVENT tme;
				ZeroMemory(&tme, sizeof(tme));
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hwnd;
				TrackMouseEvent(&tme);

				//window->win32.cursorTracked = GLFW_TRUE;
				InputSystem::Get().CursorsTracked = true;
				//_glfwInputCursorEnter(window, GLFW_TRUE);
			}

			const InputSystem& inputSys = InputSystem::Get();
			const ECursorMode cursorMode = inputSys.CurrentCursorMode;
			if (cursorMode == ECursorMode::Disabled)
			{
				const int dx = x - InputSystem::Get().LastCursorPos.x;
				const int dy = y - InputSystem::Get().LastCursorPos.y;

				InputForwarder::ForwardMouseMove(dx, dy);

// 				_glfwInputCursorPos(window,
// 					window->virtualCursorPosX + dx,
// 					window->virtualCursorPosY + dy);
			}
			//             else
			//                 _glfwInputCursorPos(window, x, y);
			// 
			//             window->win32.lastCursorPosX = x;
			//             window->win32.lastCursorPosY = y;

			return 0;
		}

		case WM_INPUT:
		{
			LOG_WINMSG(WM_INPUT);
			//             UINT size = 0;
//             HRAWINPUT ri = (HRAWINPUT) lParam;
//             RAWINPUT* data = NULL;
//             int dx, dy;
// 
//             if (_glfw.win32.disabledCursorWindow != window)
//                 break;
//             if (!window->rawMouseMotion)
//                 break;
// 
//             GetRawInputData(ri, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
//             if (size > (UINT) _glfw.win32.rawInputSize)
//             {
//                 free(_glfw.win32.rawInput);
//                 _glfw.win32.rawInput = calloc(size, 1);
//                 _glfw.win32.rawInputSize = size;
//             }
// 
//             size = _glfw.win32.rawInputSize;
//             if (GetRawInputData(ri, RID_INPUT,
//                                 _glfw.win32.rawInput, &size,
//                                 sizeof(RAWINPUTHEADER)) == (UINT) -1)
//             {
//                 _glfwInputError(GLFW_PLATFORM_ERROR,
//                                 "Win32: Failed to retrieve raw input data");
//                 break;
//             }
// 
//             data = _glfw.win32.rawInput;
//             if (data->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
//             {
//                 dx = data->data.mouse.lLastX - window->win32.lastCursorPosX;
//                 dy = data->data.mouse.lLastY - window->win32.lastCursorPosY;
//             }
//             else
//             {
//                 dx = data->data.mouse.lLastX;
//                 dy = data->data.mouse.lLastY;
//             }
// 
//             _glfwInputCursorPos(window,
//                                 window->virtualCursorPosX + dx,
//                                 window->virtualCursorPosY + dy);
// 
//             window->win32.lastCursorPosX += dx;
//             window->win32.lastCursorPosY += dy;
			break;
		}

		case WM_MOUSELEAVE:
		{
			LOG_WINMSG(WM_MOUSELEAVE);
			InputSystem::Get().CursorsTracked = false;
			//             window->win32.cursorTracked = GLFW_FALSE;
//             _glfwInputCursorEnter(window, GLFW_FALSE);
			return 0;
		}

		case WM_MOUSEWHEEL:
		{
			LOG_WINMSG(WM_MOUSEWHEEL);
			//_glfwInputScroll(window, 0.0, (SHORT) HIWORD(wParam) / (double) WHEEL_DELTA);
			return 0;
		}

		case WM_MOUSEHWHEEL:
		{
			LOG_WINMSG(WM_MOUSEHWHEEL);
			// This message is only sent on Windows Vista and later
			// NOTE: The X-axis is inverted for consistency with macOS and X11
			//_glfwInputScroll(window, -((SHORT) HIWORD(wParam) / (double) WHEEL_DELTA), 0.0);
			return 0;
		}

		case WM_ENTERSIZEMOVE:
		case WM_ENTERMENULOOP:
		{
			LOG_WINMSG(WM_ENTERSIZEMOVE - WM_ENTERMENULOOP);
			if (InputSystem::Get().OngoingFrameAction)
			{
				break;
			}
			// 
			//             // HACK: Enable the cursor while the user is moving or
			//             //       resizing the window or using the window menu
			if (InputSystem::Get().CurrentCursorMode == ECursorMode::Disabled)
			{
				EnableCursor(hwnd);
			}

			break;
		}

		case WM_EXITSIZEMOVE:
		case WM_EXITMENULOOP:
		{
			LOG_WINMSG(WM_EXITSIZEMOVE - WM_EXITMENULOOP);
			if (InputSystem::Get().OngoingFrameAction)
				break;

			// HACK: Disable the cursor once the user is done moving or
			//       resizing the window or using the menu
			if (InputSystem::Get().CurrentCursorMode == ECursorMode::Disabled)
			{
				DisableCursor(hwnd);
			}

			break;
		}

		case WM_SIZE:
		{
			LOG_WINMSG(WM_SIZE);
			//             const int width = LOWORD(lParam);
			//             const int height = HIWORD(lParam);
			//             const GLFWbool iconified = wParam == SIZE_MINIMIZED;
			//             const GLFWbool maximized = wParam == SIZE_MAXIMIZED ||
			//                                        (window->win32.maximized &&
			//                                         wParam != SIZE_RESTORED);
			// 
			//             if (_glfw.win32.disabledCursorWindow == window)
			//                 updateClipRect(window);
			// 
			//             if (window->win32.iconified != iconified)
			//                 _glfwInputWindowIconify(window, iconified);
			// 
			//             if (window->win32.maximized != maximized)
			//                 _glfwInputWindowMaximize(window, maximized);
			// 
			//             if (width != window->win32.width || height != window->win32.height)
			//             {
			//                 window->win32.width = width;
			//                 window->win32.height = height;
			// 
			//                 _glfwInputFramebufferSize(window, width, height);
			//                 _glfwInputWindowSize(window, width, height);
			//             }
			// 
			//             if (window->monitor && window->win32.iconified != iconified)
			//             {
			//                 if (iconified)
			//                     releaseMonitor(window);
			//                 else
			//                 {
			//                     acquireMonitor(window);
			//                     fitToMonitor(window);
			//                 }
			//             }
			// 
			//             window->win32.iconified = iconified;
			//             window->win32.maximized = maximized;
			return 0;
		}

		case WM_MOVE:
		{
			LOG_WINMSG(WM_MOVE);

			//             if (_glfw.win32.disabledCursorWindow == window)
//                 updateClipRect(window);
// 


//             // NOTE: This cannot use LOWORD/HIWORD recommended by MSDN, as
//             // those macros do not handle negative window positions correctly
//             _glfwInputWindowPos(window,
//                                 GET_X_LPARAM(lParam),
//                                 GET_Y_LPARAM(lParam));
			return 0;
		}

		case WM_SIZING:
		{
			LOG_WINMSG(WM_SIZING);
			//if (window->numer == GLFW_DONT_CARE || window->denom == GLFW_DONT_CARE)
			{
				break;
			}

			//             applyAspectRatio(window, (int) wParam, (RECT*) lParam);
			//             return TRUE;
		}

		case WM_GETMINMAXINFO:
		{
			LOG_WINMSG(WM_GETMINMAXINFO);
			// 			int xoff, yoff;
			// 			UINT dpi = USER_DEFAULT_SCREEN_DPI;
			// 			MINMAXINFO* mmi = (MINMAXINFO*)lParam;
			// 
			// 			if (window->monitor)
			// 				break;
			// 
			// 			if (_glfwIsWindows10AnniversaryUpdateOrGreaterWin32())
			// 				dpi = GetDpiForWindow(window->win32.handle);
			// 
			//             getFullWindowSize(getWindowStyle(window), getWindowExStyle(window),
			//                               0, 0, &xoff, &yoff, dpi);
			// 
			//             if (window->minwidth != GLFW_DONT_CARE &&
			//                 window->minheight != GLFW_DONT_CARE)
			//             {
			//                 mmi->ptMinTrackSize.x = window->minwidth + xoff;
			//                 mmi->ptMinTrackSize.y = window->minheight + yoff;
			//             }
			// 
			//             if (window->maxwidth != GLFW_DONT_CARE &&
			//                 window->maxheight != GLFW_DONT_CARE)
			//             {
			//                 mmi->ptMaxTrackSize.x = window->maxwidth + xoff;
			//                 mmi->ptMaxTrackSize.y = window->maxheight + yoff;
			//             }
			// 
			//             if (!window->decorated)
			//             {
			//                 MONITORINFO mi;
			//                 const HMONITOR mh = MonitorFromWindow(window->win32.handle,
			//                                                       MONITOR_DEFAULTTONEAREST);
			// 
			//                 ZeroMemory(&mi, sizeof(mi));
			//                 mi.cbSize = sizeof(mi);
			//                 GetMonitorInfo(mh, &mi);
			// 
			//                 mmi->ptMaxPosition.x = mi.rcWork.left - mi.rcMonitor.left;
			//                 mmi->ptMaxPosition.y = mi.rcWork.top - mi.rcMonitor.top;
			//                 mmi->ptMaxSize.x = mi.rcWork.right - mi.rcWork.left;
			//                 mmi->ptMaxSize.y = mi.rcWork.bottom - mi.rcWork.top;
			//             }

			return 0;
		}

		case WM_PAINT:
		{
			LOG_WINMSG(WM_PAINT);

			//_glfwInputWindowDamage(window);


// 			PAINTSTRUCT ps;
// 			HDC hdc;
// 			hdc = BeginPaint(hwnd, &ps);
// 			EndPaint(hwnd, &ps);
			break;
		}

		case WM_ERASEBKGND:
		{
			LOG_WINMSG(WM_ERASEBKGND);
			return TRUE;
		}

		case WM_NCACTIVATE:
		case WM_NCPAINT:
		{
			LOG_WINMSG(WM_NCACTIVATE - WM_NCPAINT);
			// Prevent title bar from being drawn after restoring a minimized
			// undecorated window
			//if (!window->decorated)
				//return TRUE;

			break;
		}

		case WM_DWMCOMPOSITIONCHANGED:
		case WM_DWMCOLORIZATIONCOLORCHANGED:
		{
			LOG_WINMSG(WM_DWMCOMPOSITIONCHANGED - WM_DWMCOLORIZATIONCOLORCHANGED);
			//if (window->win32.transparent)
				//updateFramebufferTransparency(window);
			return 0;
		}

		case WM_GETDPISCALEDSIZE:
		{
			LOG_WINMSG(WM_GETDPISCALEDSIZE);
			//             if (window->win32.scaleToMonitor)
//                 break;

			// Adjust the window size to keep the content area size constant
			//if (_glfwIsWindows10CreatorsUpdateOrGreaterWin32())
			{
				//                 RECT source = {0}, target = {0};
				//                 SIZE* size = (SIZE*) lParam;
				// 
				//                 AdjustWindowRectExForDpi(&source, getWindowStyle(window),
				//                                          FALSE, getWindowExStyle(window),
				//                                          GetDpiForWindow(window->win32.handle));
				//                 AdjustWindowRectExForDpi(&target, getWindowStyle(window),
				//                                          FALSE, getWindowExStyle(window),
				//                                          LOWORD(wParam));
				// 
				//                 size->cx += (target.right - target.left) -
				//                             (source.right - source.left);
				//                 size->cy += (target.bottom - target.top) -
				//                             (source.bottom - source.top);
				return TRUE;
			}

			break;
		}

		case WM_DPICHANGED:
		{
			LOG_WINMSG(WM_DPICHANGED);

			// const float xscale = HIWORD(wParam) / (float) USER_DEFAULT_SCREEN_DPI;
//             const float yscale = LOWORD(wParam) / (float) USER_DEFAULT_SCREEN_DPI;
// 
//             // Resize windowed mode windows that either permit rescaling or that
//             // need it to compensate for non-client area scaling
//             if (!window->monitor &&
//                 (window->win32.scaleToMonitor ||
//                  _glfwIsWindows10CreatorsUpdateOrGreaterWin32()))
//             {
//                 RECT* suggested = (RECT*) lParam;
//                 SetWindowPos(window->win32.handle, HWND_TOP,
//                              suggested->left,
//                              suggested->top,
//                              suggested->right - suggested->left,
//                              suggested->bottom - suggested->top,
//                              SWP_NOACTIVATE | SWP_NOZORDER);
//             }
// 
//             _glfwInputWindowContentScale(window, xscale, yscale);
			break;
		}

		case WM_SETCURSOR:
		{
			//LOG_WINMSG(WM_SETCURSOR);

			if (LOWORD(lParam) == HTCLIENT)
			{
				SetCursor(NULL);
				return true;
				//updateCursorImage(window);
				//return TRUE;
			}

			break;
		}

		case WM_DROPFILES:
		{
			LOG_WINMSG(WM_DROPFILES);
			//             HDROP drop = (HDROP) wParam;
//             POINT pt;
//             int i;
// 
//             const int count = DragQueryFileW(drop, 0xffffffff, NULL, 0);
//             char** paths = calloc(count, sizeof(char*));
// 
//             // Move the mouse to the position of the drop
//             DragQueryPoint(drop, &pt);
//             _glfwInputCursorPos(window, pt.x, pt.y);
// 
//             for (i = 0;  i < count;  i++)
//             {
//                 const UINT length = DragQueryFileW(drop, i, NULL, 0);
//                 WCHAR* buffer = calloc((size_t) length + 1, sizeof(WCHAR));
// 
//                 DragQueryFileW(drop, i, buffer, length + 1);
//                 paths[i] = _glfwCreateUTF8FromWideStringWin32(buffer);
// 
//                 free(buffer);
//             }
// 
//             _glfwInputDrop(window, count, (const char**) paths);
// 
//             for (i = 0;  i < count;  i++)
//                 free(paths[i]);
//             free(paths);
// 
//             DragFinish(drop);
			return 0;
		}
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	void* CreateWindowsWindow(const int32_t desiredWidth, const int32_t desiredHeight)
	{
		HINSTANCE instance = GetModuleHandle(NULL);
		WNDCLASS wc;
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WindowMessageLoop;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = instance;
		wc.hIcon = LoadIcon(0, IDI_APPLICATION);
		wc.hCursor = LoadCursor(0, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.lpszMenuName = 0;
		wc.lpszClassName = "D3DWndClassName";

		if (!RegisterClass(&wc))
		{
			LOG_ERROR("Failed to register window class");
			ASSERT(0);
			return HWND(0);
		}

		// Compute window rectangle dimensions based on requested client area dimensions.
		RECT R = { 0, 0, desiredWidth, desiredHeight };
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		int32_t width = R.right - R.left;
		int32_t height = R.bottom - R.top;

		HWND newWindow = CreateWindowExW(
			0,
			L"D3DWndClassName",
			L"Test window",
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, instance, 0);


		SetPropW(newWindow, L"WindowProp", (void*)1);

		return newWindow;
	}

	void InputForwarder::ForwardKey(const EInputKey inKey, const InputEventType inAction)
	{
		InputSystem::KeyCallback(inKey, inAction);
	}

	void InputForwarder::ForwardMouseMove(double inNewYaw, double inNewPitch)
	{
		InputSystem::MousePosChangedCallback(inNewYaw, inNewPitch);
	}

}