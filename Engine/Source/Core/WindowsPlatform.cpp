#include "Core/WindowsPlatform.h"
#include <windows.h>

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

		static int16_t Codes[512];
	};

	WindowsKeysContainer KeysContainer;

	EInputKey WindowsKeyToInternal(const int16_t inWindowsKey)
	{
		return static_cast<EInputKey>(KeysContainer.Codes[inWindowsKey]);
	}
}