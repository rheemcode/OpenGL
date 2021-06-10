#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "../Tests/Object.h"

class Window
{
public:
	Window() {}
	Window(int width, int height)
		: m_Width(width), m_Height(height)
	{

	}

	int Init()
	{
		if (!glfwInit())
			return 1;


		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(m_Width, m_Height, "Hello World", 0, NULL);
		if (!m_Window)
		{
			glfwTerminate();
			return 1;
		}

		glfwMakeContextCurrent(m_Window);



		if (glewInit() != GLEW_OK)
		{
			std::cout << "Error!" << std::endl;
			return 1;
		}
			


		glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);

		winRatio = m_Width / m_Height;
		glViewport(0, 0, m_Width, m_Height);
		glfwSwapInterval(1);

		CameraSettings setting(CameraMode::PERSPECTIVE, winRatio, 65.f);

		m_Camera = Camera(setting);
		m_Camera.Translate(glm::vec3(0.f, 0.f, -10.f));
		return 0;
	}

	void Update()
	{
		while (!glfwWindowShouldClose(m_Window))
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (auto object : m_Objects)
			{
				object->Draw(m_Camera);
			}

			glfwSwapBuffers(m_Window);
			glfwPollEvents();
		}
	}

	void AddObject(Object& object) { m_Objects.push_back(&object); }

	int GetWidth() const { return m_Width;  }
	int GetHeight() const { return m_Height; }
	float GetAspectRatio() const { return m_Width / m_Height; }

	static Window* Create(int width, int height) 
	{
		return new Window(width, height); 
	}

private:
	GLFWwindow* m_Window;
	int m_Width, m_Height;
	float winRatio;
	Camera m_Camera;

	std::vector<Object*> m_Objects;
};