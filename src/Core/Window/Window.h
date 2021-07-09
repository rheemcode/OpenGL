#pragma once
#include <Windows.h>
#include <fstream>
#include <array>
#include "../Tests/Object.h"
#include "../Renderer/OpenGLContext.h"
#include "Input/KeyCode.h"
#include "Math/SimpleVec.h"
#include <Events/KeyEvent.h>
#include <functional>

typedef int WindowID;
constexpr int MAIN_WINDOW_ID = 1;


/* Bad Bad API but we'll work with it*/
enum MouseMode
{
	MOUSE_MODE_VISIBLE,
	MOUSE_MODE_HIDDEN,
	MOUSE_MODE_CAPTURED,
	MOUSE_MODE_CONFINED
};

enum KeyState
{
	RELEASE,
	PRESSED,
	REPEAAT
};

enum CursorShape {
	CURSOR_ARROW,
	CURSOR_IBEAM,
	CURSOR_POINTING_HAND,
	CURSOR_CROSS,
	CURSOR_WAIT,
	CURSOR_BUSY,
	CURSOR_DRAG,
	CURSOR_CAN_DROP,
	CURSOR_FORBIDDEN,
	CURSOR_VSIZE,
	CURSOR_HSIZE,
	CURSOR_BDIAGSIZE,
	CURSOR_FDIAGSIZE,
	CURSOR_MOVE,
	CURSOR_VSPLIT,
	CURSOR_HSPLIT,
	CURSOR_HELP,
	CURSOR_MAX
};

enum WindowMode {
	WINDOW_MODE_WINDOWED,
	WINDOW_MODE_MINIMIZED,
	WINDOW_MODE_MAXIMIZED,
	WINDOW_MODE_FULLSCREEN
};



enum WindowFlags {
	WINDOW_FLAG_RESIZE_DISABLED,
	WINDOW_FLAG_BORDERLESS,
	WINDOW_FLAG_ALWAYS_ON_TOP,
	WINDOW_FLAG_TRANSPARENT,
	WINDOW_FLAG_NO_FOCUS,
	WINDOW_FLAG_MAX,
};

// Separate enum otherwise we get warnings in switches not handling all values.
enum WindowFlagsBit {
	WINDOW_FLAG_RESIZE_DISABLED_BIT = (1 << WINDOW_FLAG_RESIZE_DISABLED),
	WINDOW_FLAG_BORDERLESS_BIT = (1 << WINDOW_FLAG_BORDERLESS),
	WINDOW_FLAG_ALWAYS_ON_TOP_BIT = (1 << WINDOW_FLAG_ALWAYS_ON_TOP),
	WINDOW_FLAG_TRANSPARENT_BIT = (1 << WINDOW_FLAG_TRANSPARENT),
	WINDOW_FLAG_NO_FOCUS_BIT = (1 << WINDOW_FLAG_NO_FOCUS)
}; 

struct WindowData
{
	HWND hwnd;
	WindowID windowID;
	HMONITOR hMonitor;
	bool hasFocus;
	Point2 lastPos;
	int width, height;
	float winRatio;
	OpenGLContext context;
	MouseMode mouseMode;
	WindowMode windowMode;
	Size2 minSize;
	Size2 maxSize;
	Size2 fullSize;
	Size2 clientSize;
	bool canFocus = true;
	bool isFullScreen;
	bool isBorderless = false;
	bool isMinimized;
	bool isMaximized;
	bool resizeable;
};

using EventCallback = std::function<void(const Event&)>;

class Window
{
	WindowData windowData;	
	EventCallback callback;

public:
	int Init();

	void UseVysnc(bool use);
	
	/* Getters and Setters */
	const WindowData& GetWindowData() const { return windowData; }
	WindowData& GetWindowData() { return windowData; }
	const HWND& GetNativeWindow() const { return windowData.hwnd; }
	
	WindowID GetID() { return windowData.windowID; }
	void SetHasFocus(bool hasFocus) { windowData.hasFocus = hasFocus; }
	void SetMouseMode(MouseMode p_mouseMode) { windowData.mouseMode = p_mouseMode;  }
	void MakeCurrent() { windowData.context.MakeCurrent(); }
	void ReleaseCurrent() { windowData.context.ReleaseCurrent(); }

	MouseMode GetMouseMode(MouseMode p_mouseMode) { return windowData.mouseMode;  }
	
	int GetWidth() const { return windowData.width;  }
	void SetWidth(int width) { windowData.width = width; }
	void SetHeight(int height) { windowData.height = height; }
	int GetHeight() const { return windowData.height; }
	
	float GetAspectRatio() const { return windowData.width / windowData.height; }
	void WindowFocused();
	
	bool IsFocused() { return windowData.hasFocus; }
	void SwapBuffers();

	void BindEventCallback(EventCallback p_callback);
	void EventDispatcher(const Event& event);

	// Callbacks
	void WindowResized(int width, int height);
	Window();
	~Window();

};

class Display
{
	static Display* s_Instance;
	HINSTANCE hInstance;
	MSG m_msg = { 0 };

	double oldMouseX, oldMouseY;
	Point2 mouseCenter;

	bool isShift, isControl, isAlt;
	bool frameAction;
	MouseMode m_mouseMode = MouseMode::MOUSE_MODE_VISIBLE;
	CursorShape m_cursorShape = CursorShape::CURSOR_ARROW;
	KeyEvent keyEventBuffer[512];
	
	int keyEventPos, rEventPos;
	HCURSOR cursors[CursorShape::CURSOR_MAX] = { nullptr };
	HCURSOR hCursor;

	WindowID focusedWindow = -1;


	bool oldMouseInvalid;
	bool mouseOutside;
	bool useRawInput;

	WNDCLASSEXW wc;
	uint8_t m_windowCount = -1;
	Display(HINSTANCE p_hInstance, WindowFlags p_flags, WindowMode p_mainWindowMode, Size2 p_windowSize);
public:
	static bool isCloseRequest;
	static void Create(HINSTANCE p_hInstance, WindowFlags p_flags, WindowMode p_mainWindowMode, Size2 p_windowSize);
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam);
	void GetWindowStyles(bool p_main_window, bool p_fullscreen, bool p_borderless, bool p_resizable, bool p_maximized, bool p_no_activate_focus, DWORD& r_style, DWORD& r_style_ex);
	std::array<Window*, 3> m_Windows;
	
	int CreateWindowDisplay(WindowMode mode, const LPCWSTR& windowName, uint32_t p_flags, const RECT& p_rect);
	void SetWindowFlags(WindowFlags p_flags, bool p_enabled, WindowID windowID);
	void DestroyWindowDisplay(WindowID windowID);
	void EnableCursor();
	void UpdateWindowStyle(WindowID windowID, bool p_repaint);
	void SetCursorPosition(WindowID windowID, const double& xPos, const double& yPos);
	Point2 GetCursorPosition(WindowID windowID);
	Point2 GetCursorPosition();
	void FocusWindow(WindowID windowID);
	int GetScreenCount();
	Size2 GetScreenSize(int p_screen);
	int GetCurrentScreen(WindowID windowID);
	void SetCurrentScreen(int p_sreen, WindowID windowID);
	Point2 GetScreenPosition(int p_screen);
	void SetMouseMode(MouseMode mouseMode);	
	void SetCursorShape(CursorShape p_cursorShape);
	void SetWindowPosition(const POINT& p_point, WindowID);
	RECT GetUsableScreenRect(int p_screen);
	Point2 GetWindowPosition(WindowID windowID);
	Size2 GetActualWindowSize(WindowID windowID);
	void SetWindowSize(const Size2& p_size, WindowID);
	Size2 GetWindowSize(WindowID windowID);
	int GetKeyBoardLayouts();
	void SetKeyBoardLayout(int p_index);
	void ProcessKeyEvents();
	void UseVysnc(bool use) 
	{ 
		for (auto window : m_Windows)
		{
			if (window)
				window->UseVysnc(use);
		}
	}
	void SwapBuffer()
	{
		if (focusedWindow != -1)
		{
			m_Windows[focusedWindow]->SwapBuffers();
			return;
		}
	}
	void ShowWindow(WindowID windowID);
	static Display* GetSingleton();
	void ProcessEvents();
	~Display(){}
private:
};

