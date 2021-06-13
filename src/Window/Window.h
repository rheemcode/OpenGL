#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "../Tests/Object.h"
#include <functional>
#include "Camera.h"

using namespace std::placeholders;
#define BIND_EVENT_FN(x) std::bind(&Window::x, this, _1, _2, _3, _4, _5)


class Window
{
public:
	Window();
	Window(int width, int height);
	~Window();

	int Init();
	void Update();

	void AddObject(Object& object) { m_Objects.push_back(&object); }

	int GetWidth() const { return m_Width;  }
	int GetHeight() const { return m_Height; }
	float GetAspectRatio() const { return m_Width / m_Height; }

	static Window* Create(int width, int height);

private:
	GLFWwindow* m_Window;
	int m_Width, m_Height;
	float winRatio;
	Camera m_Camera;

	std::vector<Object*> m_Objects;

public:
	// this is probably a bad implentation and should abstract into an Input class or so!
	static bool isWkey, isAkey, isSkey, isDkey, isQkey, isEkey;
};