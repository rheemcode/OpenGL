#include <glpch.h>
#include "Window.h"
#include "Input/Input.h"
#include <Events/MouseEvent.h>
#include "Math/Vector2.h"


int Window::Init()
{
	
	if (windowData.context.Init(windowData.hwnd))
	{
		MessageBoxW(nullptr, L"Context Creation Failed", L"Error", 1);
	}
	glViewport(0, 0, windowData.width, windowData.height);
	glClearColor(.1f, .1f, .1f, 1);
	return 0;
}

void Window::SetUseVysnc(bool use) { windowData.context.SetUseVysnc(use); }

void Window::WindowFocused()
{
	SetHasFocus(true);
}

void Window::SwapBuffers()
{
	windowData.context.SwapBuffer();
}

void Window::BindEventCallback(EventCallback p_eventCallback)
{
	callback = p_eventCallback;
}

void Window::EventDispatcher(const Event& event)
{
	callback(event);
}

void Window::WindowResized(int width, int height)
{
	//TODO: replace with proper implementation
	glViewport(0, 0, width, height);
}

Window::Window()
{
}


Window::~Window()
{
}
