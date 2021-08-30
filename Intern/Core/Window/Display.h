#pragma once
#include "Window.h"


enum KeyState
{
	RELEASE,
	PRESSED,
	REPEAT
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

class GLMain;
class Input;

class GLIB_API Display
{
	friend Input;
	friend GLMain;
	HINSTANCE hInstance;
	MSG m_msg = { 0 };
	HCURSOR cursors[CursorShape::CURSOR_MAX] = { nullptr };
	HCURSOR hCursor;
	WNDCLASSEXW wc;

	static Display* s_Instance;

	bool isShift, isControl, isAlt;
	bool oldMouseInvalid;
	bool mouseOutside;
	bool useRawInput;
	bool frameAction;
	double oldMouseX, oldMouseY;
	int keyEventPos, rEventPos;
	Point2 mouseCenter;

	MouseMode m_mouseMode = MouseMode::MOUSE_MODE_VISIBLE;
	CursorShape m_cursorShape = CursorShape::CURSOR_ARROW;
	KeyEvent keyEventBuffer[512];

	WindowID focusedWindow = -1;
	uint8_t m_windowCount = 0;
	std::array<Window*, 3> m_Windows;

private:
	static void Create(HINSTANCE p_hInstance, WindowFlags p_flags, LPCWSTR windowName, WindowMode p_mainWindowMode, Size2 p_windowSize);
	Display(HINSTANCE p_hInstance, WindowFlags p_flags, LPCWSTR windowName, WindowMode p_mainWindowMode, Size2 p_windowSize);

	void GetWindowStyles(bool p_main_window, bool p_fullscreen, bool p_borderless, bool p_resizable, bool p_maximized, bool p_no_activate_focus, DWORD& r_style, DWORD& r_style_ex);
	int CreateWindowDisplay(WindowMode mode, LPCWSTR windowName, uint32_t p_flags, const RECT& p_rect);
	void SetWindowFlags(WindowFlags p_flags, bool p_enabled, WindowID windowID);
	void DestroyWindowDisplay(WindowID windowID);
	void UpdateWindowStyle(WindowID windowID, bool p_repaint);

	void SetCursorPosition(WindowID windowID, const double& xPos, const double& yPos);
	void SetCurrentScreen(int p_sreen, WindowID windowID);
	Point2 GetScreenPosition(int p_screen);
	int GetScreenCount();
	RECT GetUsableScreenRect(int p_screen);
	void SetCursorShape(CursorShape p_cursorShape);
	int GetCurrentScreen(WindowID windowID);

	void SetKeyBoardLayout(int p_index);
public:
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam);
	static bool isCloseRequest;

	void EnableCursor();
	void FocusWindow(WindowID windowID);
	void ProcessKeyEvents();

	Window* GetMainWindow() const { return m_Windows[0]; }
	Point2 GetCursorPosition(WindowID windowID);
	Point2 GetCursorPosition();
	Point2 GetWindowPosition(WindowID windowID);
	Size2 GetScreenSize(int p_screen);
	Size2 GetActualWindowSize(WindowID windowID);
	Size2 GetWindowSize(WindowID windowID);
	int GetKeyBoardLayouts();

	void SetWindowSize(const Size2& p_size, WindowID);
	void SetMouseMode(MouseMode mouseMode);
	void SetWindowPosition(const POINT& p_point, WindowID);

	void UseVysnc(bool use, WindowID ID)
	{
		m_Windows[ID]->SetUseVysnc(use);
	}
	void SwapBuffer()
	{
		if (focusedWindow != -1)
		{
			m_Windows[focusedWindow]->SwapBuffers();
			return;
		}
	}

	void SwapBuffer(WindowID ID)
	{
		m_Windows[ID]->SwapBuffers();
	}

	void ShowWindow(WindowID windowID);

	static Display* GetSingleton();
	void ProcessEvents();

	~Display();
};