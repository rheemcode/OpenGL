#pragma once
#include "Renderer/OpenGLContext.h"
#include "Input/KeyCode.h"
#include "Math/SimpleVec.h"
#include "Math/Vector2.h"
#include "Events/KeyEvent.h"


typedef int WindowID;
constexpr WindowID MAIN_WINDOW_ID = 1;


/* Bad Bad API but we'll work with it*/
class EventQueue;

enum WindowMode {
	WINDOW_MODE_WINDOWED,
	WINDOW_MODE_MINIMIZED,
	WINDOW_MODE_MAXIMIZED,
	WINDOW_MODE_FULLSCREEN
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

	void SetUseVysnc(bool use);
	void MakeCurrent() { windowData.context.MakeCurrent(); }
	void ReleaseCurrent() { windowData.context.ReleaseCurrent(); }
	void SwapBuffers();

	/* Getters and Setters */
	WindowID GetID() { return windowData.windowID; }

	const WindowData& GetWindowData() const { return windowData; }
	WindowData& GetWindowData() { return windowData; }
	const HWND& GetNativeWindow() const { return windowData.hwnd; }
	
	void SetHasFocus(bool hasFocus) { windowData.hasFocus = hasFocus; }
	void SetMouseMode(MouseMode p_mouseMode) { windowData.mouseMode = p_mouseMode;  }


	MouseMode GetMouseMode(MouseMode p_mouseMode) { return windowData.mouseMode;  }
	
	int GetWidth() const { return windowData.width;  }
	int GetHeight() const { return windowData.height; }
	void SetWidth(int width) { windowData.width = width; }
	void SetHeight(int height) { windowData.height = height; }
	float GetAspectRatio() const { return float(windowData.width / windowData.height); }
	bool IsFocused() { return windowData.hasFocus; }
	void WindowFocused();
	

	// Callbacks
	void WindowResized(int width, int height);
	void BindEventCallback(EventCallback p_callback);
	void EventDispatcher(const Event& event);

	Window();
	~Window();

};


