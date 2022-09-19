#include <assert.h>
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "OpenGLUtils.h"
#include "Core/EngineUtils.h"
#include "Core/EngineCore.h"
#include "Scene/Scene.h"
#include "Camera/Camera.h"
#include "Scene/SceneManager.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"
#include "Entity/Entity.h"
#include "Renderer/Drawable/Drawable.h"
#include "EASTL/shared_ptr.h"
#include "Renderer/Material/RenderMaterial.h"
#include "Buffer/VertexArrayObject.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/Drawable/MirrorQuad.h"
#include "Core/ObjectCreation.h"
#include "OpenGLDepthMap.h"
#include "OpenGLRenderTexture.h"
#include "Renderer/Drawable/DepthMaterial.h"
#include "Core/WindowsPlatform.h"

#include <windows.h>

#if !WITH_GLFW

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int* attribList);
wglCreateContextAttribsARB_type* wglCreateContextAttribsARB;

// See https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt for all values
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);

wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;

#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

HWND MainWindowHandle = {};
HINSTANCE openglInstance = {};
HDC gldc = {};
using glProc = void(*)();
bool CursorsTracked = false;
bool OngoingFrameAction = false;

enum class ECursorMode
{
	Enabled = 0,
	Hidden,
	Disabled
};

ECursorMode CurrentCursorMode;

static void init_opengl_extensions()
{
	// Need methods to create right context, need context to get methods.. so create dummy context

	// Before we can load extensions, we need a dummy OpenGL context, created using a dummy window.
	// We use a dummy window because you can only set the pixel format for a window once. For the
	// real window, we want to use wglChoosePixelFormatARB (so we can potentially specify options
	// that aren't available in PIXELFORMATDESCRIPTOR), but we can't load and use that before we
	// have a context.

	WNDCLASSW windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = DefWindowProcA;
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.lpszClassName = L"Dummy_Window_Class";

	if (!RegisterClassW(&windowClass)) {
		ASSERT_MSG(false, "Failed to register dummy OpenGL window.");
	}

	HWND dummyWindow = CreateWindowExW(
		0,
		windowClass.lpszClassName,
		L"Dummy OpenGL Window",
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		windowClass.hInstance,
		0);

	if (!dummyWindow) {
		ASSERT_MSG(false, "Failed to create dummy OpenGL window.");
	}

	HDC dummyDeviceContext = GetDC(dummyWindow);

	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixelFormat = ChoosePixelFormat(dummyDeviceContext, &pfd);
	if (!pixelFormat) {
		ASSERT_MSG(false, "Failed to find a suitable pixel format.");
	}
	if (!SetPixelFormat(dummyDeviceContext, pixelFormat, &pfd)) {
		ASSERT_MSG(false, "Failed to set the pixel format.");
	}

	HGLRC dummyGLContext = wglCreateContext(dummyDeviceContext);
	if (!dummyGLContext) {
		ASSERT_MSG(false, "Failed to create a dummy OpenGL rendering context.");
	}

	if (!wglMakeCurrent(dummyDeviceContext, dummyGLContext)) {
		ASSERT_MSG(false, "Failed to activate dummy OpenGL rendering context.");
	}

	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");

	wglMakeCurrent(dummyDeviceContext, 0);
	wglDeleteContext(dummyGLContext);
	ReleaseDC(dummyWindow, dummyDeviceContext);
	DestroyWindow(dummyWindow);
}

static HGLRC init_opengl(HDC real_dc)
{
	init_opengl_extensions();

	// Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
	int pixel_format_attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB,     true,
		WGL_SUPPORT_OPENGL_ARB,     true,
		WGL_DOUBLE_BUFFER_ARB,      true,
		WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,         32,
		WGL_DEPTH_BITS_ARB,         24,
		WGL_STENCIL_BITS_ARB,       8,
		0
	};

	int pixel_format;
	UINT num_formats;
	wglChoosePixelFormatARB(real_dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
	if (!num_formats) {
		ASSERT_MSG(false, "Failed to set the OpenGL 3.3 pixel format.");
	}

	PIXELFORMATDESCRIPTOR pfd;
	DescribePixelFormat(real_dc, pixel_format, sizeof(pfd), &pfd);
	if (!SetPixelFormat(real_dc, pixel_format, &pfd)) {
		ASSERT_MSG(false, "Failed to set the OpenGL 3.3 pixel format.");
	}

	// Specify that we want to create an OpenGL 3.3 core profile context
	int gl33_attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0,
	};

	HGLRC gl33_context = wglCreateContextAttribsARB(real_dc, 0, gl33_attribs);
	if (!gl33_context) {
		ASSERT_MSG(false, "Failed to create OpenGL 3.3 context.");
	}

	if (!wglMakeCurrent(real_dc, gl33_context)) {
		ASSERT_MSG(false, "Failed to activate OpenGL 3.3 rendering context.");
	}

	return gl33_context;
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

glm::vec<2, long> CursorPos = {};

void GetCursorPos(HWND inWindowHandle, OUT glm::vec<2, long>& outPos)
{
	POINT pos;

	if (GetCursorPos(&pos))
	{
		ScreenToClient(inWindowHandle, &pos);

		outPos.x = pos.x;
		outPos.y = pos.y;
	}
}

void SetCursorPos(HWND inWindowHandle, const glm::vec<2, long>& inPos)
{
	POINT pos = { inPos.x, inPos.y};

	// Store the new position so it can be recognized later
// 	window->win32.lastCursorPosX = pos.x;
// 	window->win32.lastCursorPosY = pos.y;

	ClientToScreen(inWindowHandle, &pos);
	SetCursorPos(pos.x, pos.y);
}

void DisableCursor()
{
	GetCursorPos(MainWindowHandle, CursorPos);
	SetCursor(NULL);

	RECT area;
	GetClientRect(MainWindowHandle, &area);
	int width, height;
	width = area.right;
	height = area.bottom;

	POINT pos = { (int)width / 2, (int)height / 2 };
	ClientToScreen(MainWindowHandle, &pos);
	SetCursorPos(pos.x, pos.y);

	UpdateClipRect(MainWindowHandle);
}

void EnableCursor()
{
	UpdateClipRect(nullptr);

	SetCursorPos(MainWindowHandle, CursorPos);
	SetCursor(nullptr);
}

void SetCursorMode(HWND inWindowHandle, ECursorMode inMode)
{
	if (inMode == ECursorMode::Disabled)
	{
		if (IsWindowFocused(MainWindowHandle))
		{
			DisableCursor();
		}
	}
	//else if (_glfw.win32.disabledCursorWindow == window)
		//enableCursor(window);
	//else if (cursorInContentArea(window))
		//updateCursorImage(window);
}


#define LOG_WINMSG(MsgType) LOG_INFO("Windows Message: %s", #MsgType)

LRESULT MainWndProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	//return gApplicationDX11->MsgProc(hwnd, msg, wParam, lParam);

	void* windowValid = GetPropA(MainWindowHandle, "MainWindow");

	if (!windowValid)
	{
		// This is the message handling for the dummy window
		// and for a regular window during its initial creation

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	// Whole glfw message loop but we use only what's needed
    switch (msg)
    {
        case WM_MOUSEACTIVATE:
        {
			LOG_WINMSG(WM_MOUSEACTIVATE);

//             // HACK: Postpone cursor disabling when the window was activated by
//             //       clicking a caption button
			if (HIWORD(lParam) == WM_LBUTTONDOWN)
			{
				if (LOWORD(lParam) != HTCLIENT)
					OngoingFrameAction = true;
			}

            break;
        }

		case WM_CAPTURECHANGED:
        {
			LOG_WINMSG(WM_CAPTURECHANGED);
			// HACK: Disable the cursor once the caption button action has been
            //       completed or cancelled
			if (lParam == 0 && OngoingFrameAction)
			{
				if (CurrentCursorMode == ECursorMode::Disabled)
				{
					DisableCursor();
				}

				OngoingFrameAction = false;
             }

            break;
        }

        case WM_SETFOCUS:
        {
			LOG_WINMSG(WM_SETFOCUS);
			//             _glfwInputWindowFocus(window, GLFW_TRUE);
// 
//             // HACK: Do not disable cursor while the user is interacting with
//             //       a caption button
             if (OngoingFrameAction)
                 break;

			 if (CurrentCursorMode == ECursorMode::Disabled)
			 {
				DisableCursor();
			 }


            return 0;
        }

        case WM_KILLFOCUS:
        {
			LOG_WINMSG(WM_KILLFOCUS);
			if (CurrentCursorMode == ECursorMode::Disabled)
				EnableCursor();
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
					LOG_WINMSG(SC_SCREENSAVE-SC_MONITORPOWER);
					//                     if (window->monitor)
//                     {
//                         // We are running in full screen mode, so disallow
//                         // screen saver and screen blanking
//                         return 0;
//                     }
//                     else
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
			LOG_WINMSG(WM_CHAR-WM_SYSCHAR);
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
			LOG_WINMSG(WM_KEYDOWN-WM_SYSKEYDOWN-WM_KEYUP-WM_SYSKEYUP);
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
                 scancode = MapVirtualKeyW((UINT) wParam, MAPVK_VK_TO_VSC);
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
             //else
                 //_glfwInputKey(window, key, scancode, action, mods);

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
			LOG_WINMSG(WM_LBUTTONDOWN-WM_RBUTTONDOWN-WM_MBUTTONDOWN-WM_XBUTTONDOWN-WM_LBUTTONUP-WM_RBUTTONUP-WM_MBUTTONUP-WM_XBUTTONUP);
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

			//             const int x = GET_X_LPARAM(lParam);
//             const int y = GET_Y_LPARAM(lParam);
// 
			if (!CursorsTracked)
			{
				TRACKMOUSEEVENT tme;
				ZeroMemory(&tme, sizeof(tme));
				tme.cbSize = sizeof(tme);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = MainWindowHandle;
				TrackMouseEvent(&tme);

				//window->win32.cursorTracked = GLFW_TRUE;
				CursorsTracked = true;
				//_glfwInputCursorEnter(window, GLFW_TRUE);
			}
// 
//             if (window->cursorMode == GLFW_CURSOR_DISABLED)
//             {
//                 const int dx = x - window->win32.lastCursorPosX;
//                 const int dy = y - window->win32.lastCursorPosY;
// 
//                 if (_glfw.win32.disabledCursorWindow != window)
//                     break;
//                 if (window->rawMouseMotion)
//                     break;
// 
//                 _glfwInputCursorPos(window,
//                                     window->virtualCursorPosX + dx,
//                                     window->virtualCursorPosY + dy);
//             }
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
			CursorsTracked = false;
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
			LOG_WINMSG(WM_ENTERSIZEMOVE-WM_ENTERMENULOOP);
			if (OngoingFrameAction)
			{
			   break;
		    }
// 
//             // HACK: Enable the cursor while the user is moving or
//             //       resizing the window or using the window menu
			if (CurrentCursorMode == ECursorMode::Disabled)
			{
				EnableCursor();
			}

            break;
        }

        case WM_EXITSIZEMOVE:
        case WM_EXITMENULOOP:
        {
			LOG_WINMSG(WM_EXITSIZEMOVE-WM_EXITMENULOOP);
			if (OngoingFrameAction)
                 break;
 
            // HACK: Disable the cursor once the user is done moving or
            //       resizing the window or using the menu
			if (CurrentCursorMode == ECursorMode::Disabled)
			{
				DisableCursor();
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
			LOG_WINMSG(WM_NCACTIVATE-WM_NCPAINT);
			// Prevent title bar from being drawn after restoring a minimized
            // undecorated window
            //if (!window->decorated)
				//return TRUE;

            break;
        }

        case WM_DWMCOMPOSITIONCHANGED:
        case WM_DWMCOLORIZATIONCOLORCHANGED:
        {
			LOG_WINMSG(WM_DWMCOMPOSITIONCHANGED-WM_DWMCOLORIZATIONCOLORCHANGED);
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

HWND CreateWindowsWindow(HINSTANCE instance, const int32_t desiredWidth, const int32_t desiredHeight)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = MainWndProc;
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

	return newWindow;
}

static glProc getProcAddressGLWindows(const char* procname)
{
	const glProc proc = (glProc)wglGetProcAddress(procname);
	if (proc)
		return proc;

	return (glProc)GetProcAddress(openglInstance, procname);
}

#endif

const uint32_t SHADOW_WIDTH = 4096;
const uint32_t SHADOW_HEIGHT = 4096;

OpenGLRenderer* RHI = nullptr;
static std::mutex RenderCommandsMutex;
static std::mutex LoadQueueMutex;
static std::mutex GetVAOMutex;
static std::condition_variable LoadQueueCondition;

const glm::vec3 lightPos(-10.0f, 10.0f, -1.0f);

constexpr glm::vec4 ClearColor(0.3f, 0.5f, 1.f, 0.4f);

void LoaderFunc(GLFWwindow* inLoadingThreadContext)
{
	while (Engine->IsRunning())
	{
		eastl::queue<RenderingLoadCommand>& loadQueue = RHI->GetLoadQueue();
		std::unique_lock<std::mutex> lock{ LoadQueueMutex };
		LoadQueueCondition.wait(lock, [&] {return !loadQueue.empty(); });

		RenderingLoadCommand newCommand = loadQueue.front();
		loadQueue.pop();

		lock.unlock();

		glfwMakeContextCurrent(inLoadingThreadContext);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLUtils::GLDebugCallback, nullptr);

		newCommand.LoadDel.Execute(newCommand.ModelPath, newCommand.Parent);

		glfwMakeContextCurrent(nullptr);
	}
}

OpenGLRenderer::OpenGLRenderer(const WindowProperties& inMainWindowProperties)
{
#if !WITH_GLFW
	HINSTANCE mhAppInst = GetModuleHandle(NULL);
	MainWindowHandle = CreateWindowsWindow(mhAppInst, inMainWindowProperties.Width, inMainWindowProperties.Height);
	SetPropA(MainWindowHandle, "MainWindow", (void*)1);

	openglInstance = LoadLibraryA("opengl32.dll");
	ASSERT(openglInstance);
 	gldc = GetDC(MainWindowHandle);
 	HGLRC glrc = init_opengl(gldc);
 
 	ShowWindow(MainWindowHandle, SW_SHOW);
 	UpdateWindow(MainWindowHandle);
 	const bool gladSuccess = gladLoadGLLoader((GLADloadproc)getProcAddressGLWindows) == 1;
	ASSERT(gladSuccess);
	MainWindow = eastl::make_unique<OpenGLWindow>(nullptr, inMainWindowProperties);
#else
  	const bool glfwSuccess = glfwInit() == GLFW_TRUE;
  	ASSERT(glfwSuccess);
  	//glfwSetErrorCallback(OpenGLUtils::GLFWErrorCallback);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
  	// Create new Window for data holding
  	GLFWwindow* newWindowHandle = glfwCreateWindow(inMainWindowProperties.Width, inMainWindowProperties.Height, inMainWindowProperties.Title.data(), nullptr, nullptr);
	glfwMakeContextCurrent(newWindowHandle);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  	const bool gladSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GLFW_TRUE;
	ASSERT(gladSuccess);
	glfwSetInputMode(newWindowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	MainWindow = eastl::make_unique<OpenGLWindow>(newWindowHandle, inMainWindowProperties);
#endif


#if !WITH_GLFW
	SetCursorMode(MainWindowHandle, ECursorMode::Disabled);
#endif
 
	SetViewportSizeToMain();

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glEnable(GL_CULL_FACE);

	glDebugMessageCallback(OpenGLUtils::GLDebugCallback, nullptr);
	glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);

	// Set the default uniforms
	SetupBaseUniforms();

	// Create the loading thread
	//std::thread(LoaderFunc, loadingThreadContext).detach();
}

OpenGLRenderer::~OpenGLRenderer() = default;

void OpenGLRenderer::Init(const WindowProperties & inMainWindowProperties)
{
	RHI = new OpenGLRenderer{ inMainWindowProperties };

	// Setup secondary framebuffer

	// Create the frame buffer
	glGenFramebuffers(1, &RHI->AuxiliarFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, RHI->AuxiliarFrameBuffer);

	const WindowProperties& windowProps = RHI->MainWindow->GetProperties();
	// Create a stencil and depth render buffer object for the frame buffer
	uint32_t rbo; // render buffer object
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowProps.Width, windowProps.Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attach the rbo to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	// Create the shadow map framebuffer
	glGenFramebuffers(1, &RHI->ShadowMapBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, RHI->ShadowMapBuffer);

	RHI->ShadowBufferTex = eastl::make_shared<OpenGLDepthMap>("ShadowMap");
	RHI->ShadowBufferTex->Init();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, RHI->ShadowBufferTex->TexHandle, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	// Bind the default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::Terminate()
{
	RHI->MainWindow.reset();

#if WITH_GLFW
	glfwTerminate();
#endif;

	glDeleteBuffers(1, &RHI->AuxiliarFrameBuffer);


	ASSERT(RHI);
	delete RHI;
}

void OpenGLRenderer::Draw()
{
	UpdateUniforms();

	//DrawShadowMap();

	SetupBaseUniforms();
	UpdateUniforms();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	DrawSkybox();
	RenderCommandsMutex.lock();
	DrawCommands(MainCommands);
	RenderCommandsMutex.unlock();

	CheckShouldCloseWindow(*MainWindow);


#if !WITH_GLFW
	MSG msg = {};

	// If there are Window messages then process them.
	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// Otherwise, do animation/game stuff.
	else
	{
		// 			mTimer.Tick();
		// 
		// 			if (!mAppPaused)
		// 			{
		// 				CalculateFrameStats();
		// 				UpdateScene(mTimer.Elapsed());
		// 				DrawScene();
		// 			}
		// 			else
			// 		{
			// 			::Sleep(0);
			// 		}
	}
	SwapBuffers(gldc);

#else
	glfwSwapBuffers(MainWindow->GetHandle());
#endif

}

void OpenGLRenderer::DrawSkybox()
{
	if (!MainSkyboxCommand.Parent.lock())
	{
		return;
	}

	if (!MainSkyboxCommand.VAO->bReadyForDraw)
	{
		MainSkyboxCommand.VAO->SetupState();
	}

	glDepthFunc(GL_LEQUAL);
	DrawCommand(MainSkyboxCommand);
	glDepthFunc(GL_LESS);
}

void OpenGLRenderer::DrawShadowMap()
{
	// Cull front face to solve Peter Panning
	//glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, ShadowMapBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	SetDrawMode(EDrawMode::DEPTH);
	SetViewportSize(SHADOW_WIDTH, SHADOW_HEIGHT);

	//const glm::vec3 lightLoc = LightSource->GetAbsoluteTransform().Translation;
	const glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const float near_plane = 1.f;
	const float far_plane = 20.f;
	const glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	//UniformsCache["projection"] = lightProjection;

	const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	UniformsCache["lightSpaceMatrix"] = lightSpaceMatrix;

	RenderCommandsMutex.lock();
	DrawCommands(MainCommands);
	RenderCommandsMutex.unlock();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	SetViewportSizeToMain();
	SetDrawMode(EDrawMode::NORMAL);

	// Reset to default
	//glCullFace(GL_BACK);
}

void OpenGLRenderer::SetupBaseUniforms()
{
	const glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(MainWindow->GetProperties().Width) / static_cast<float>(MainWindow->GetProperties().Height), 0.1f, 1000.0f);
	UniformsCache["projection"] = projection;
}

void OpenGLRenderer::UpdateUniforms()
{
	const glm::mat4 view = SceneManager::Get().GetCurrentScene().CurrentCamera->GetLookAt();
	UniformsCache["view"] = view;
}

void OpenGLRenderer::DrawCommands(const eastl::vector<RenderCommand>&inCommands)
{
	for (const RenderCommand& renderCommand : inCommands)
	{
		DrawCommand(renderCommand);
	}
}

void OpenGLRenderer::DrawCommand(const RenderCommand & inCommand)
{
	const bool parentValid = !inCommand.Parent.expired();
	if (!ENSURE(parentValid))
	{
		return;
	}

	const eastl::shared_ptr<const DrawableObject> parent = inCommand.Parent.lock();
	const eastl::shared_ptr<RenderMaterial> material = GetMaterial(inCommand);
	const eastl::shared_ptr<VertexArrayObject>& vao = inCommand.VAO;

	if (!parent->IsVisible())
	{
		return;
	}

	// TODO: Abstract the model and parent dependent uniforms (like the Model Matrix) to be present in the render command 
	// and updated only if dirty

	// Deffered VAO initialization on the Main Rendering Thread
	if (!vao->bReadyForDraw)
	{
		vao->SetupState();
	}

	material->Shader.Bind();
	material->ResetUniforms();

	UniformsCache["model"] = parent->GetModelMatrix();

	int32_t i = 0;
	for (i = 0; i < material->Textures.size(); ++i)
	{
		eastl::shared_ptr<OpenGLTexture>& tex = material->Textures[i];
		tex->Bind(i);
	}

	//
	ShadowBufferTex->Bind(i);
	//
	RHI->UniformsCache["ShadowMap"] = uint32_t(i);
	RHI->UniformsCache["LightPos"] = lightPos;

	const uint32_t indicesCount = vao->VBuffer.GetIndicesCount();
	vao->Bind();
	material->SetUniforms(UniformsCache);

	switch (inCommand.DrawType)
	{
	case EDrawCallType::DrawElements:
	{
		glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
		break;
	}
	case EDrawCallType::DrawArrays:
	{
		glDrawArrays(GL_TRIANGLES, 0, indicesCount);
		break;
	}
	}

	vao->Unbind();

	for (i = 0; i < material->Textures.size(); ++i)
	{
		eastl::shared_ptr<OpenGLTexture>& tex = material->Textures[i];
		tex->Unbind(i);
	}

	//
	ShadowBufferTex->Unbind(i);
	//

	material->Shader.UnBind();
}

eastl::shared_ptr<RenderMaterial> OpenGLRenderer::GetMaterial(const RenderCommand & inCommand) const
{
	switch (DrawMode)
	{
	case EDrawMode::NORMAL:
	{
		return inCommand.Material;
	}
	case EDrawMode::DEPTH:
	{
		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> depthMaterial = matManager.GetOrAddMaterial<DepthMaterial>("depth_material", materialExists);

		if (!materialExists)
		{
			depthMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicDepthVertexShader.glsl", "../Data/Shaders/BasicDepthFragmentShader.glsl");
		}

		return depthMaterial;
	}
	case EDrawMode::DEPTH_VISUALIZE:
	{
		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> depthMaterial = matManager.GetOrAddMaterial("visualize_depth_material", materialExists);

		if (!materialExists)
		{
			depthMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/VisualizeDepthFragmentShader.glsl");
		}

		return depthMaterial;
	}
	case EDrawMode::OUTLINE:
	{
		MaterialsManager& matManager = MaterialsManager::Get();
		bool materialExists = false;
		eastl::shared_ptr<RenderMaterial> outlineMaterial = matManager.GetOrAddMaterial("outline_material", materialExists);

		if (!materialExists)
		{
			outlineMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/OutlineFragmentShader.glsl");
		}

		return outlineMaterial;
	}
	}

	return { nullptr };
}

void OpenGLRenderer::DestroyWindow(GLFWwindow * inWindowHandle) const
{
#if WITH_GLFW
	glfwDestroyWindow(inWindowHandle);
#endif
}

void OpenGLRenderer::SetVSyncEnabled(const bool inEnabled)
{
	//glfwSwapInterval(inEnabled);
}

void OpenGLRenderer::AddMirrorCommand(const RenderCommand & inCommand)
{
	// 	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	// 	MirrorCommands.push_back(inCommand);
}

void OpenGLRenderer::AddCommand(const RenderCommand & inCommand)
{
	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	MainCommands.push_back(inCommand);
}

void OpenGLRenderer::AddCommands(eastl::vector<RenderCommand> inCommands)
{
	std::lock_guard<std::mutex> lock(RenderCommandsMutex);
	MainCommands.insert(MainCommands.end(), inCommands.begin(), inCommands.end());
}

void OpenGLRenderer::AddRenderLoadCommand(const RenderingLoadCommand & inCommand)
{
	std::unique_lock<std::mutex> lock{ LoadQueueMutex };

	LoadQueue.push(inCommand);
	LoadQueueCondition.notify_one();
}

bool OpenGLRenderer::GetOrCreateVAO(const eastl::string & inVAOId, OUT eastl::shared_ptr<VertexArrayObject>&outVAO)
{
	ASSERT(!inVAOId.empty());
	std::lock_guard<std::mutex> uniqueMutex(GetVAOMutex);
	//GetVAOMutex.lock(); // TODO: Why does this not work?

	using iterator = const eastl::unordered_map<eastl::string, eastl::shared_ptr<VertexArrayObject>>::iterator;
	const iterator& vaoIter = VAOs.find(inVAOId);
	const bool materialExists = vaoIter != VAOs.end();

	if (materialExists)
	{
		outVAO = (*vaoIter).second;

		return true;
	}

	eastl::shared_ptr<VertexArrayObject> newVAO = eastl::make_shared<VertexArrayObject>();
	VAOs[inVAOId] = newVAO;
	outVAO = newVAO;

	//GetVAOMutex.unlock();

	return false;
}

void OpenGLRenderer::SetViewportSize(const int32_t inWidth, const int32_t inHeight)
{
	glViewport(0, 0, inWidth, inHeight);
}

void OpenGLRenderer::SetViewportSizeToMain()
{
	const WindowProperties& props = MainWindow->GetProperties();
	SetViewportSize(props.Width, props.Height);
}

void OpenGLRenderer::CheckShouldCloseWindow(const OpenGLWindow & inWindow)
{
#if !WITH_GLFW
	if(inWindow.ShouldClose())
	{
		StopEngine();
	}
#else
	if (glfwWindowShouldClose(inWindow.GetHandle()))
	{
		StopEngine();
	}
#endif

}

