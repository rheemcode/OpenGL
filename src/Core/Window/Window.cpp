#include "Window.h"
#include <cassert>
#include <string>
#include "Input/Input.h"
#include <windowsx.h>
#include <Events/MouseEvent.h>

Window::Window()
{
}

int Window::Init()
{
	if (windowData.context.Init(windowData.hwnd))
	{
		MessageBoxW(nullptr, L"FAILED", L"FAILEd", 1);
	}
	glViewport(0, 0, windowData.width, windowData.height);
	glClearColor(.8, .8, .8, 1);
	return 0;
}

void Window::WindowFocused()
{
	SetHasFocus(true);
}

void Window::SwapBuffers()
{
	windowData.context.SwapBuffer();
}

Window::~Window()
{
}

bool Display::isCloseRequest = false;



LRESULT Display::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam)
{
	WindowID windowID;
	bool windowCreated = false;
	for (auto window : m_Windows)
	{
		if (!window)
			continue;

		if (window->GetNativeWindow() == hwnd)
		{
			windowID = window->GetID();
			windowCreated = true;
		}
	}

	if (!windowCreated) 
		return DefWindowProc(hwnd, uMsg, wParam, lparam);
	
	switch (uMsg)
	{

		case WM_MOUSEACTIVATE:
		{
			if (HIWORD(lparam) == WM_LBUTTONDOWN)
			{
				if (LOWORD(lparam) == HTCLIENT)
				{
					frameAction = true;
					ReleaseCapture();
				}

			}

			break;
		}

		case WM_CAPTURECHANGED:
		{
			if (lparam == 0 && frameAction)
			{
				if (m_mouseMode == MOUSE_MODE_CAPTURED)
					SetMouseMode(m_mouseMode);

				frameAction = false;
			}

			break;
		}

		case WM_SETFOCUS:
		{
			m_Windows[windowID]->SetHasFocus(true);
			focusedWindow = windowID;
			if (frameAction)
				break;
			
			SetMouseMode(m_mouseMode);
			return 0;
		}

		case WM_KILLFOCUS:
		{
			m_Windows[windowID]->SetHasFocus(false);
			focusedWindow = windowID;

			if (m_mouseMode == MOUSE_MODE_CAPTURED || m_mouseMode == MOUSE_MODE_HIDDEN)
			{
				ReleaseCapture();
				EnableCursor();

			}

			return 0;
		}

		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				return 0;
			case SC_KEYMENU:
				if ((lparam >> 16) <= 0)
					return 0;
			}

			break;
		}

		case WM_CLOSE:
		{
			isCloseRequest = true;
			return 0;
		}


		case WM_ACTIVATE:
		{

			if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
				m_Windows[windowID]->WindowFocused();
			else
			{
				m_Windows[windowID]->SetHasFocus(false);
			}


			return 0;
		}

		case WM_MOUSELEAVE:
		{
			oldMouseInvalid = true;
			mouseOutside = true;
			return 0;
		}

		case WM_INPUT:
		{
			if (m_mouseMode != MouseMode::MOUSE_MODE_CAPTURED || !useRawInput)
				break;

			UINT size = 0;

			GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[size];
			if (lpb == nullptr)
				return 0;

			if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &size, sizeof(RAWINPUTHEADER)) != size)
			{
				OutputDebugString(L"Failed to retrieve raw input data");
			}

			RAWINPUT* raw = (RAWINPUT*)lpb;

			int dx, dy;
			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				if (raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE)
				{
					dx = raw->data.mouse.lLastX - oldMouseX;
					dy = raw->data.mouse.lLastY - oldMouseY;
				}
				else
				{
					dx = raw->data.mouse.lLastX;
					dy = raw->data.mouse.lLastY;
				}
			}

			oldMouseX += dx;
			oldMouseY += dy;
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (m_mouseMode == MouseMode::MOUSE_MODE_CAPTURED && useRawInput)
				break;

			const int x = GET_X_LPARAM(lparam);
			const int y = GET_Y_LPARAM(lparam);
			if (mouseOutside)
			{
				CursorShape c = m_cursorShape;
				m_cursorShape = CursorShape::CURSOR_MAX;
				SetCursorShape(c);
				mouseOutside = false;

				//Once-Off notification, must call again....
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.dwFlags = TME_LEAVE;
				tme.hwndTrack = hwnd;
				tme.dwHoverTime = HOVER_DEFAULT;
				TrackMouseEvent(&tme);
			}


			if (!m_Windows[windowID]->IsFocused() && m_mouseMode == MouseMode::MOUSE_MODE_CAPTURED)
				break;


			if (m_mouseMode == MouseMode::MOUSE_MODE_CAPTURED)
			{
				Point2 c(m_Windows[windowID]->GetWidth() / 2, m_Windows[windowID]->GetHeight() / 2, 0);
				oldMouseX = c.x;
				oldMouseY = c.y;


				POINT pos = { (int)c.x, (int)c.y };
				ClientToScreen(m_Windows[windowID]->GetNativeWindow(), &pos);
				SetCursorPos(pos.x, pos.y);
			}

			Input::GetSingleton()->SetMousePos(Point2(x, y));

			if (oldMouseInvalid)
			{
				oldMouseX = x;
				oldMouseY = y;
				oldMouseInvalid = false;
			}

			const int dx = x - oldMouseX;
			const int dy = y - oldMouseY;

			oldMouseX = x;
			oldMouseY = y;

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
		case WM_MOUSEWHEEL:
		case WM_MOUSEHWHEEL:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_XBUTTONDBLCLK:
		{
			switch (uMsg)
			{
				case WM_LBUTTONDOWN:
				{
					MouseButtonPressedEvent mb(1);
				} break;
				case WM_LBUTTONUP: {
					MouseButtonReleasedEvent mb(1);
				} break;
				case WM_MBUTTONDOWN: {
					MouseButtonPressedEvent mb(3);
				} break;
				case WM_MBUTTONUP: {
					MouseButtonReleasedEvent mb(3);
				} break;
				case WM_RBUTTONDOWN: {
					MouseButtonPressedEvent mb(2);
				} break;
				case WM_RBUTTONUP: {
					MouseButtonReleasedEvent mb(2);
				} break;
				case WM_LBUTTONDBLCLK: {
					MouseButtonPressedEvent mb(1);
				} break;
				case WM_RBUTTONDBLCLK: {
					MouseButtonPressedEvent mb(2);
				} break;
				case WM_MBUTTONDBLCLK: {
					MouseButtonPressedEvent mb(3);
				} break;
				case WM_MOUSEWHEEL: {
					MouseScrolledEvent mb(0, (short)HIWORD(wParam) / (double)WHEEL_DELTA);

				} break;
				case WM_MOUSEHWHEEL: {
					MouseScrolledEvent mb(-(short)HIWORD(wParam) / (double)WHEEL_DELTA, 0.f);
				} break;
				case WM_XBUTTONDOWN: {
					if (HIWORD(wParam) == XBUTTON1)
						MouseButtonPressedEvent mb(Mouse::BUTTON_4);
					else
						MouseButtonPressedEvent mb(Mouse::BUTTON_5);
				} break;
				case WM_XBUTTONUP: {
					if (HIWORD(wParam) == XBUTTON1)
						MouseButtonReleasedEvent mb(Mouse::BUTTON_4);
					else
						MouseButtonReleasedEvent mb(Mouse::BUTTON_5);
				} break;
				case WM_XBUTTONDBLCLK: {
					if (HIWORD(wParam) == XBUTTON1)
						MouseButtonPressedEvent mb(Mouse::BUTTON_4);
					else
						MouseButtonPressedEvent mb(Mouse::BUTTON_5);
				} break;
				default:
				{
					return 0;
				}
			}

			Input::GetSingleton()->SetMousePos(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));

			if (m_mouseMode == MouseMode::MOUSE_MODE_CAPTURED && !useRawInput)
				Input::GetSingleton()->SetMousePos(oldMouseX, oldMouseY);

			break;
		}
		
		case WM_ENTERSIZEMOVE:
		case WM_ENTERMENULOOP:
		{
			if (frameAction)
				break;
			POINT pos;
			if ((m_mouseMode != MOUSE_MODE_CAPTURED))
			{
				if (GetCursorPos(&pos))
				{
					ScreenToClient(m_Windows[windowID]->GetNativeWindow(), &pos);
					oldMouseX = pos.x;
					oldMouseY = pos.y;

					SetCursorShape(m_cursorShape);
				}
			}
			if (m_mouseMode == MOUSE_MODE_CAPTURED)
			{
				RECT rect;
				SetCursorPosition(windowID, oldMouseX, oldMouseY);
				GetClientRect(m_Windows[windowID]->GetNativeWindow(), &rect);
				ClientToScreen(m_Windows[windowID]->GetNativeWindow(), (POINT*)&rect.left);
				ClientToScreen(m_Windows[windowID]->GetNativeWindow(), (POINT*)&rect.right);
				ClipCursor(&rect);
				CursorShape c = m_cursorShape;
				m_cursorShape = CURSOR_MAX;
				SetCursorShape(c);
			}

			break;
		}

		case WM_EXITSIZEMOVE:
		case WM_EXITMENULOOP:
		{
			if (frameAction)
				break;
			POINT pos;
			if ((m_mouseMode != MOUSE_MODE_CAPTURED))
			{
				if (GetCursorPos(&pos))
				{
					ScreenToClient(m_Windows[windowID]->GetNativeWindow(), &pos);
					oldMouseX = pos.x;
					oldMouseY = pos.y;
					CursorShape c = m_cursorShape;
					m_cursorShape = CURSOR_MAX;
					SetCursorShape(c);
				}
			}
			if (m_mouseMode == MOUSE_MODE_CAPTURED)
			{
				Size2 size = GetWindowSize(windowID);
				SetCursorPosition(windowID, size.x / 2, size.y / 2);
				RECT rect;
				GetClientRect(m_Windows[windowID]->GetNativeWindow(), &rect);
				ClientToScreen(m_Windows[windowID]->GetNativeWindow(), (POINT*)&rect.left);
				ClientToScreen(m_Windows[windowID]->GetNativeWindow(), (POINT*)&rect.right);
				ClipCursor(&rect);
				CursorShape c = m_cursorShape;
				m_cursorShape = CURSOR_MAX;
				SetCursorShape(c);
			}

			break;
		}

		case WM_MOVE:
		{
			if (m_mouseMode == MOUSE_MODE_CAPTURED)
			{
				RECT rect;
				GetClientRect(m_Windows[windowID]->GetNativeWindow(), &rect);
				ClientToScreen(m_Windows[windowID]->GetNativeWindow(), (POINT*)&rect.left);
				ClientToScreen(m_Windows[windowID]->GetNativeWindow(), (POINT*)&rect.right);
				ClipCursor(&rect);
			}
			if (!IsIconic(m_Windows[windowID]->GetNativeWindow()))
			{
				int x = GET_X_LPARAM(lparam);
				int y = GET_Y_LPARAM(lparam);
			}

			return 0;
		}

		case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED)
			{
				int windowW = LOWORD(lparam);
				int windowH = HIWORD(lparam);

				if (windowW > 0 && windowH > 0)
				{
					m_Windows[windowID]->SetWidth(windowW);
					m_Windows[windowID]->SetHeight(windowH);
				}
			}
		
			if (wParam == SIZE_MAXIMIZED)
			{
				m_Windows[windowID]->GetWindowData().isMaximized = true;
				m_Windows[windowID]->GetWindowData().isMinimized = false;
			}
			else if (wParam == SIZE_MINIMIZED)
			{
				m_Windows[windowID]->GetWindowData().isMaximized = false;
				m_Windows[windowID]->GetWindowData().isMinimized = true;
			}
			else if (wParam == SIZE_RESTORED)
			{
				m_Windows[windowID]->GetWindowData().isMaximized = false;
				m_Windows[windowID]->GetWindowData().isMinimized = false;
			}
			break;
		}

		case WM_GETMINMAXINFO:
		{
			auto data = m_Windows[windowID]->GetWindowData();
			if (data.resizeable && !data.isFullScreen)
			{
				Size2 dec = GetWindowSize(windowID) - GetActualWindowSize(windowID);
				MINMAXINFO* minMaxInfo = (MINMAXINFO*)lparam;
				if (data.minSize != Size2())
				{
					minMaxInfo->ptMinTrackSize.x = data.minSize.x + dec.x;
					minMaxInfo->ptMinTrackSize.y = data.minSize.y + dec.y;
				}

				if (data.maxSize != Size2())
				{
					minMaxInfo->ptMaxTrackSize.x = data.maxSize.x + dec.x;
					minMaxInfo->ptMaxTrackSize.y = data.maxSize.y + dec.y;
				}
				return 0;
			}
			else
			{
				break;
			}
			
		}


		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYUP:
		case WM_KEYDOWN: 
		{
			if (wParam == VK_SHIFT)
				isShift = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
			if (wParam == VK_CONTROL)
				isControl = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
			if (wParam == VK_MENU)
				isAlt = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);

		}
		case WM_CHAR:
		{
			if (uMsg == WM_SYSKEYDOWN || uMsg == WM_KEYDOWN)
			{
				KeyPressedEvent ke;
				ke.shift = (wParam != VK_SHIFT) ? isShift : false;
				ke.alt = (!(wParam == VK_MENU && (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN))) ? isAlt : false;
				ke.control = (wParam != VK_CONTROL) ? isControl : false;
				ke.action = WM_KEYDOWN;
				pEventBuffer[pEventPos++] = ke;
			}
			if (uMsg == WM_SYSKEYUP || uMsg == WM_KEYUP)
			{
				KeyReleasedEvent ke;
				ke.shift = (wParam != VK_SHIFT) ? isShift : false;
				ke.alt = (!(wParam == VK_MENU && (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN))) ? isAlt : false;
				ke.control = (wParam != VK_CONTROL) ? isControl : false;
				ke.action = WM_KEYUP;
				rEventBuffer[rEventPos++] = ke;
			}
			if (uMsg == WM_SYSCHAR || uMsg == WM_CHAR)
			{
				KeyReleasedEvent ke;
				ke.shift = (wParam != VK_SHIFT) ? isShift : false;
				ke.alt = (!(wParam == VK_MENU && (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN))) ? isAlt : false;
				ke.control = (wParam != VK_CONTROL) ? isControl : false;
				ke.action = WM_CHAR;
				rEventBuffer[rEventPos++] = ke;
			}
		}

		case WM_SETCURSOR:
		{
			if (LOWORD(lparam) == HTCLIENT) {
				if (m_Windows[windowID]->IsFocused() && (m_mouseMode == MOUSE_MODE_HIDDEN || m_mouseMode == MOUSE_MODE_CAPTURED)) {
					//Hide the cursor
					if (hCursor == nullptr) {
						hCursor = SetCursor(nullptr);
					}
					else {
						SetCursor(nullptr);
					}
				}
				else {
			//		if (hCursor != nullptr) {
						CursorShape c = m_cursorShape;
						m_cursorShape = CURSOR_MAX;
						SetCursorShape(c);
				//		hCursor = nullptr;
					//}
				}

				return TRUE;
			}

			break;
		}

		default:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lparam);
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lparam);
}


static BOOL CALLBACK _MonitorEnumProcCount(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	int* data = (int*)dwData;
	(*data)++;
	return TRUE;
}

int Display::GetScreenCount()
{
	int res = 0;
	EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProcCount, (LPARAM)&res);
	return res;
}

typedef struct {
	int count;
	int screen;
	Size2 size;
} EnumSizeData;

typedef struct {
	int count;
	int screen;
	RECT rect;
} EnumRectData;

static BOOL CALLBACK _MonitorEnumProcSize(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	EnumSizeData* data = (EnumSizeData*)dwData;
	if (data->count == data->screen) {
		data->size.x = lprcMonitor->right - lprcMonitor->left;
		data->size.y = lprcMonitor->bottom - lprcMonitor->top;
	}

	data->count++;
	return TRUE;
}

static BOOL CALLBACK _MonitorEnumProcUsableSize(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	EnumRectData* data = (EnumRectData*)dwData;
	if (data->count == data->screen) {
		MONITORINFO minfo;
		memset(&minfo, 0, sizeof(MONITORINFO));
		minfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfoA(hMonitor, &minfo);

		data->rect.left = minfo.rcWork.left;
		data->rect.top = minfo.rcWork.top;
		data->rect.right = minfo.rcWork.right - minfo.rcWork.left;
		data->rect.bottom = minfo.rcWork.bottom - minfo.rcWork.top;
	}

	data->count++;
	return TRUE;
}


typedef struct {
	int count;
	int screen;
	HMONITOR monitor;
} EnumScreenData;

static BOOL CALLBACK _MonitorEnumProcScreen(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	EnumScreenData* data = (EnumScreenData*)dwData;
	if (data->monitor == hMonitor) {
		data->screen = data->count;
	}

	data->count++;
	return TRUE;
}

typedef struct {
	int count;
	int screen;
	Point2 pos;
} EnumPosData;

static BOOL CALLBACK _MonitorEnumProcPos(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	EnumPosData* data = (EnumPosData*)dwData;
	if (data->count == data->screen) {
		data->pos.x = lprcMonitor->left;
		data->pos.y = lprcMonitor->top;
	}

	data->count++;
	return TRUE;
}


Size2 Display::GetScreenSize(int p_screen)
{
	EnumSizeData data = { 0, p_screen == -1 ? GetCurrentScreen(0) : p_screen, Size2() };
	EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProcSize, (LPARAM)&data);
	return data.size;
}


int Display::GetCurrentScreen(WindowID windowID)
{
	EnumScreenData data = { 0, 0, MonitorFromWindow(m_Windows[windowID]->GetNativeWindow(), MONITOR_DEFAULTTONEAREST) };
	EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProcScreen, (LPARAM)&data);
	return data.screen;return 0;
}

void Display::SetCurrentScreen(int p_screen, WindowID windowID)
{

	Point2 ofs = GetWindowPosition(windowID) - GetScreenPosition(GetCurrentScreen(windowID));
	POINT point;
	point.x = ofs.x + GetScreenPosition(p_screen).x;
	point.y = ofs.y + GetScreenPosition(p_screen).y;
	SetWindowPosition(point, windowID);
}


Point2 Display::GetScreenPosition(int p_screen)
{
	EnumPosData data = { 0, p_screen == -1 ? GetCurrentScreen(0) : p_screen, Point2() };
	EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProcPos, (LPARAM)&data);
	return data.pos;
}


void Display::GetWindowStyles(bool p_main_window, bool p_fullscreen, bool p_borderless, bool p_resizable, bool p_maximized, bool p_no_activate_focus, DWORD& r_style, DWORD& r_style_ex)
{
	r_style = 0;
	r_style_ex = WS_EX_WINDOWEDGE;
	if (p_main_window) {
		r_style_ex |= WS_EX_APPWINDOW;
	}

	if (p_fullscreen || p_borderless) {
		r_style |= WS_POPUP;
		//if (p_borderless) {
		//	r_style_ex |= WS_EX_TOOLWINDOW;
		//}
	}
	else {
		if (p_resizable) {
			if (p_maximized) {
				r_style = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
			}
			else {
				r_style = WS_OVERLAPPEDWINDOW;
			}
		}
		else {
			r_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
		}
	}
	if (!p_borderless) {
		r_style |= WS_VISIBLE;
	}

	if (p_no_activate_focus) {
		r_style_ex |= WS_EX_TOPMOST | WS_EX_NOACTIVATE;
	}
	r_style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
}

void Display::DestroyWindowDisplay(WindowID windowID)
{
	DestroyWindow(m_Windows[windowID]->GetNativeWindow());
}

Point2 Display::GetCursorPosition(WindowID windowID)
{
	POINT pos;
	if (GetCursorPos(&pos))
	{

		ScreenToClient(m_Windows[windowID]->GetNativeWindow(), &pos);
		return Point2(pos.x, pos.y, 0);

	}

	return Point2();
}

Point2 Display::GetCursorPosition()
{
	POINT pos;
	if (GetCursorPos(&pos))
	{
		if (focusedWindow != -1)
		{
			ScreenToClient(m_Windows[focusedWindow]->GetNativeWindow(), &pos);
			return Point2(pos.x, pos.y, 0);
		}
	}

	return Point2();
}

void Display::FocusWindow(WindowID windowID)
{
	BringWindowToTop(m_Windows[windowID]->GetNativeWindow());
	SetForegroundWindow(m_Windows[windowID]->GetNativeWindow());
	SetFocus(m_Windows[windowID]->GetNativeWindow());
}

void Display::SetCursorShape(CursorShape p_cursorShape)
{
	if (m_cursorShape == p_cursorShape)
		return;

	if (m_mouseMode != MouseMode::MOUSE_MODE_VISIBLE && m_mouseMode != MOUSE_MODE_CONFINED)
	{
		m_cursorShape = p_cursorShape;
		return;
	}

	static const LPCTSTR winCursors[CursorShape::CURSOR_MAX]
	{
		IDC_ARROW,
		IDC_IBEAM,
		IDC_HAND, //finger
		IDC_CROSS,
		IDC_WAIT,
		IDC_APPSTARTING,
		IDC_ARROW,
		IDC_ARROW,
		IDC_NO,
		IDC_SIZENS,
		IDC_SIZEWE,
		IDC_SIZENESW,
		IDC_SIZENWSE,
		IDC_SIZEALL,
		IDC_SIZENS,
		IDC_SIZEWE,
		IDC_HELP
	};

	if (cursors[p_cursorShape] != nullptr)
	{
		SetCursor(cursors[p_cursorShape]);
	}

	else
	{
		SetCursor(LoadCursor(hInstance, winCursors[p_cursorShape]));
	}
	
	m_cursorShape = p_cursorShape;
}

void Display::EnableCursor()
{
	SetCursorPosition(focusedWindow, oldMouseX, oldMouseY);
}

void Display::UpdateWindowStyle(WindowID windowID, bool p_repaint)
{
	WindowData& wd = m_Windows[windowID]->GetWindowData();

	DWORD style = 0;
	DWORD style_ex = 0;

	GetWindowStyles(windowID == 0, wd.isFullScreen, wd.isBorderless, wd.resizeable, wd.isMaximized, false, style, style_ex);

	SetWindowLongPtr(wd.hwnd, GWL_STYLE, style);
	SetWindowLongPtr(wd.hwnd, GWL_EXSTYLE, style_ex);

	SetWindowPos(wd.hwnd, false ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | (false ? SWP_NOACTIVATE : 0));

	if (p_repaint) {
		RECT rect;
		GetWindowRect(wd.hwnd, &rect);
		MoveWindow(wd.hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
	}
}

void Display::SetMouseMode(MouseMode p_mouseMode)
{
	if (p_mouseMode == MouseMode::MOUSE_MODE_CAPTURED || p_mouseMode == MouseMode::MOUSE_MODE_CONFINED)
	{
		Window* window = m_Windows[0];

		RECT clipRect;
		GetClientRect(window->GetNativeWindow(), &clipRect);
		ClientToScreen(window->GetNativeWindow(), (POINT*)&clipRect.left);
		ClientToScreen(window->GetNativeWindow(), (POINT*)&clipRect.right);
		ClipCursor(&clipRect);

		if (p_mouseMode == MouseMode::MOUSE_MODE_CAPTURED)
		{
			
			POINT center = { (clipRect.right - clipRect.left) / 2, (clipRect.bottom - clipRect.top) / 2 };
			POINT pos = { center.x, center.y };
			ClientToScreen(window->GetNativeWindow(), &pos);
			SetCursorPos(pos.x, pos.y);
			SetCapture(window->GetNativeWindow());
		}
	}
	else
	{
		ReleaseCapture();
		ClipCursor(nullptr);
	}


	if (p_mouseMode == MOUSE_MODE_CAPTURED || p_mouseMode == MOUSE_MODE_HIDDEN)
	{
		if (hCursor == nullptr) {
			hCursor = SetCursor(nullptr);
		}
		else {
			SetCursor(nullptr);
		}
	}
	else {
		hCursor = LoadCursorW(NULL, IDC_ARROW);
		hCursor = SetCursor(hCursor);
		CursorShape c = m_cursorShape;
		m_cursorShape = CURSOR_MAX;
		SetCursorShape(c);
	}
}

void Display::ProcessEvents()
{
	while (PeekMessageW(&m_msg, nullptr, NULL, NULL, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessageW(&m_msg);
	}
}

int Display::GetKeyBoardLayouts()
{
	HKL curLayout = GetKeyboardLayout(0);

	int layoutCount = GetKeyboardLayoutList(0, nullptr);
	HKL* layouts = (HKL*)malloc(layoutCount * sizeof(HKL));
	GetKeyboardLayoutList(layoutCount, layouts);

	for (int i = 0; i < layoutCount; i++)
	{
		if (curLayout == layouts[i])
		{
			free(layouts);
			return i;
		}
	}
	
	if (layouts != nullptr) 
		free(layouts);
	return -1;
}

void Display::SetCursorPosition(WindowID windowID, const double& xPos, const double& yPos)
{
	POINT pos = { (int)xPos, (int)yPos };
	oldMouseX = pos.x;
	oldMouseY = pos.y;

	ClientToScreen(m_Windows[windowID]->GetNativeWindow(), &pos);
	SetCursorPos(pos.x, pos.y);
}

void Display::SetWindowPosition(const POINT& p_point, WindowID windowID)
{
	Window* window = m_Windows[windowID];
	const WindowData& windowData = window->GetWindowData();

	if (windowData.isFullScreen)
		return;

	RECT rc;
	rc.left = p_point.x;
	rc.right = p_point.x + windowData.width;
	rc.top = p_point.y;
	rc.bottom = p_point.y + windowData.height;

	const DWORD style = GetWindowLongPtr(windowData.hwnd, GWL_STYLE);
	const DWORD exstyle = GetWindowLongPtr(windowData.hwnd, GWL_EXSTYLE);

	AdjustWindowRectEx(&rc, style, false, exstyle);
	MoveWindow(windowData.hwnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);

	if (m_mouseMode == MouseMode::MOUSE_MODE_CONFINED)
	{
		RECT rect;
		GetClientRect(windowData.hwnd, &rect);
		ClientToScreen(windowData.hwnd, (POINT*)&rect.left);
		ClientToScreen(windowData.hwnd, (POINT*)&rect.right);
		ClipCursor(&rect);
	}

	POINT mousePos;
	if (GetCursorPos(&mousePos) && ScreenToClient(windowData.hwnd, &mousePos)) {
		if (mousePos.x > 0 && mousePos.y > 0 && mousePos.x <= windowData.width && mousePos.y <= windowData.height) {
			oldMouseX = mousePos.x;
			oldMouseY = mousePos.y;
			Input::GetSingleton()->SetMousePos(oldMouseX, oldMouseX);
		}
	}

}

RECT Display::GetUsableScreenRect(int p_screen)
{
	RECT rect;
	rect.left = 0;
	rect.right = 0;
	rect.top = 0;
	rect.bottom = 0;
	EnumRectData data = { 0, p_screen == -1 ? GetCurrentScreen(0) : p_screen, rect };
	EnumDisplayMonitors(nullptr, nullptr, _MonitorEnumProcUsableSize, (LPARAM)&data);
	return data.rect;
}

void Display::SetWindowSize(const Size2& p_s, WindowID windowID)
{
	Window* window = m_Windows[windowID];
	const WindowData& windowData = window->GetWindowData();

	if (windowData.isFullScreen)
		return;
	
	int w = (int) p_s.width;
	int h = (int)p_s.height;

	RECT rect;
	GetWindowRect(windowData.hwnd, &rect);

	RECT _rect;
	GetClientRect(windowData.hwnd, &_rect);

	w += (rect.right - rect.left) - (_rect.right - _rect.left);
	h += (rect.bottom - rect.top) - (_rect.bottom - _rect.top);

	MoveWindow(windowData.hwnd, rect.left, rect.top, w, h, TRUE);

	if (m_mouseMode == MouseMode::MOUSE_MODE_CONFINED)
	{
		RECT _rect;
		GetClientRect(windowData.hwnd, &_rect);
		ClientToScreen(windowData.hwnd, (POINT*)&_rect.left);
		ClientToScreen(windowData.hwnd, (POINT*)&_rect.right);
		ClipCursor(&_rect);
	}
}

Size2 Display::GetWindowSize(WindowID windowID)
{
	const WindowData& windowData = m_Windows[windowID]->GetWindowData();

	if (windowData.isMinimized)
		return Size2(windowData.width, windowData.height);

	RECT rect;

	if (GetClientRect(windowData.hwnd, &rect))
	{
		return Size2(rect.right - rect.left, rect.bottom - rect.top);
	}

	return Size2();

}

Point2 Display::GetWindowPosition(WindowID windowID)
{
	const WindowData& windowData = m_Windows[windowID]->GetWindowData();

	if (windowData.isMinimized)
		return Point2();

	RECT rect;

	if (GetClientRect(windowData.hwnd, &rect))
	{
		return Point2(rect.left, rect.top, 0);
	}

	return Point2();

}

Size2 Display::GetActualWindowSize(WindowID windowID)
{
	RECT rect;
	if (GetWindowRect(m_Windows[windowID]->GetNativeWindow(), &rect))
	{
		return Size2(rect.right - rect.left, rect.bottom - rect.top);
	}
	return Size2();
}

void Display::SetKeyBoardLayout(int p_index)
{
	int layout_count = GetKeyboardLayoutList(0, nullptr);

	HKL* layouts = (HKL*)malloc(layout_count * sizeof(HKL));
	GetKeyboardLayoutList(layout_count, layouts);
	ActivateKeyboardLayout(layouts[p_index], KLF_SETFORPROCESS);
	free(layouts);
}

void Display::ShowWindow(WindowID windowID)
{
	::ShowWindow(m_Windows[windowID]->GetNativeWindow(), SW_SHOWNA);
}


Display* Display::s_Instance;
Display* Display::GetSingleton() { return s_Instance; }

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam)
{
	Display* display = Display::GetSingleton();
	if (display)
	{
		return display->WndProc(hwnd, uMsg, wParam, lparam);
	}
	else return DefWindowProcW(hwnd, uMsg, wParam, lparam);
}


Window* Display::CreateWindowDisplay(WindowMode p_mode, const LPCWSTR& windowName, const RECT& p_rect)
{
	DWORD dwExStyle;
	DWORD dwStyle;

	GetWindowStyles(0, p_mode == WindowMode::WINDOW_MODE_FULLSCREEN, false, true, false, false, dwStyle, dwExStyle);
	RECT WindowRect;

	WindowRect.left = p_rect.left;
	WindowRect.right = p_rect.right;
	WindowRect.top = p_rect.top;
	WindowRect.bottom = p_rect.bottom;

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	HWND hwnd = CreateWindowExW(
		dwExStyle,
		L"ENGINE",
		windowName,
		dwStyle,
		WindowRect.left,
		WindowRect.top,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!hwnd)
	{
		MessageBoxW(nullptr, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return nullptr;
	}
	auto window = new Window();
	WindowData& windowData = window->GetWindowData();
	windowData.bottom = WindowRect.bottom;
	windowData.top = WindowRect.top;
	windowData.left = WindowRect.left;
	windowData.right = WindowRect.right;
	windowData.width = WindowRect.right - WindowRect.left;
	windowData.height = WindowRect.bottom - WindowRect.top;
	windowData.hwnd = hwnd;
	windowData.resizeable = true;
	windowData.isFullScreen = false;
	windowData.isMinimized = false;
	windowData.isMaximized = false;
	windowData.windowID = m_windowCount;
	windowData.minSize = Size2(400, 300);

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = windowData.hwnd;
	tme.dwHoverTime = HOVER_DEFAULT;
	TrackMouseEvent(&tme);

	DragAcceptFiles(windowData.hwnd, true);

	window->Init();
	m_Windows[m_windowCount++] = window;

	return window;
}

void Display::SetWindowFlags(WindowFlags p_flag, bool p_enabled, WindowID windowID)
{
	WindowData& wd = m_Windows[windowID]->GetWindowData();
	switch (p_flag) {
	case WINDOW_FLAG_RESIZE_DISABLED: {
		wd.resizeable = !p_enabled;
		UpdateWindowStyle(windowID, true);
	} break;
	case WINDOW_FLAG_BORDERLESS: {
		wd.isBorderless = p_enabled;
		UpdateWindowStyle(windowID, true);
	} break;
	case WINDOW_FLAG_ALWAYS_ON_TOP: {
		UpdateWindowStyle(windowID, true);;
	} break;
	case WINDOW_FLAG_TRANSPARENT: {
		// FIXME: Implement.
	} break;
	case WINDOW_FLAG_NO_FOCUS: {
		wd.canFocus = p_enabled;
		UpdateWindowStyle(windowID, true);
	} break;
	case WINDOW_FLAG_MAX:
		break;
	}
}


void Display::Create(HINSTANCE p_hInstance, WindowFlags p_flags, WindowMode p_mainWindowMode, Size2 p_windowSize)
{
	if (s_Instance)
		return;
	s_Instance = new Display(p_hInstance, p_flags, p_mainWindowMode, p_windowSize);
}

Display::Display(HINSTANCE p_hInstance, WindowFlags p_flags, WindowMode p_mainWindowMode, Size2 p_windowSize)
{

	memset(&wc, 0, sizeof(WNDCLASSEXW));
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wc.lpfnWndProc = ::WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance ? hInstance : GetModuleHandle(nullptr);
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = L"Engine";

	mouseOutside = true;

	if (!RegisterClassExW(&wc)) {
		MessageBoxA(nullptr, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	m_mouseMode = MOUSE_MODE_VISIBLE;
	mouseOutside = true;
	useRawInput = true;

	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = 0;
	Rid[0].hwndTarget = 0;

	if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE) {
		useRawInput = false;
	}

	Point2 mainWinPos((GetScreenSize(0).width - p_windowSize.width) / 2, (GetScreenSize(0).height - p_windowSize.height) /2);
	RECT winrect;
	winrect.left = mainWinPos.x;
	winrect.top = mainWinPos.y;
	winrect.right = p_windowSize.x;
	winrect.bottom = p_windowSize.y;
	Window* window = CreateWindowDisplay(p_mainWindowMode, L"ENGINE", winrect);
	window->GetWindowData().isBorderless = p_flags & WINDOW_FLAG_BORDERLESS_BIT;
	for (int i = 0; i < WINDOW_FLAG_MAX; i++) {
		if (p_flags & (1 << i)) {
			SetWindowFlags(WindowFlags(i), true, 0);
		}
	}

	ShowWindow(0);
}
