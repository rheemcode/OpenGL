#include "Window.h"
bool Window::isWkey, Window::isAkey, Window::isSkey, Window::isDkey, Window::isQkey, Window::isEkey;

Window::Window(){}

Window::Window(int width, int height)
	: m_Width(width), m_Height(height)
{

}

int Window::Init()
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


	glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			switch (action)
			{
			case (GLFW_PRESS):
			{
				if (key == GLFW_KEY_W)
					isWkey = true;
				else if (key == GLFW_KEY_D)
					isDkey = true;
				else if (key == GLFW_KEY_S)
					isSkey = true;
				else if (key == GLFW_KEY_A)
					isAkey = true;
				else if (key == GLFW_KEY_Q)
					isQkey = true;
				else if (key == GLFW_KEY_E)
					isEkey = true;
				break;
			}


			case (GLFW_RELEASE):
			{
				if (key == GLFW_KEY_W)
					isWkey = false;

				else if (key == GLFW_KEY_D)
					isDkey = false;

				else if (key == GLFW_KEY_S)
					isSkey = false;

				else if (key == GLFW_KEY_A)
					isAkey = false;
				
				else if (key == GLFW_KEY_Q)
					isQkey = false;
				else if (key == GLFW_KEY_E)
					isEkey = false;
				break;
			}

			case (GLFW_REPEAT):
			{
				break;
			}

			default:
				break;

			}

		});


	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
		return 1;
	}



	glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);

	winRatio = m_Width / m_Height;
	glViewport(0, 0, m_Width, m_Height);
	glfwSwapInterval(1);

	CameraSettings setting(CameraMode::PERSPECTIVE, winRatio, 85.f);

	m_Camera = Camera(setting);
	//m_Camera.Translate(glm::vec3(0.f, 0.f, -3.f));
	return 0;
}

void Window::Update()
{
	while (!glfwWindowShouldClose(m_Window))
	{

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		m_Camera.Update();

		for (auto object : m_Objects)
		{
			object->Draw(m_Camera);
		}

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

Window* Window::Create(int width, int height)
{
	{
		return new Window(width, height);
	}
}

Window::~Window()
{
}