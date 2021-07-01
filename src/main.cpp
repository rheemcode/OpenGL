//#include "Window/Window.h"
//#include "Tests/Torus.h"
//#include "Tests/Cube.h"
//#include "Tests/Sphere.h"

#include <Windows.h>
#include <Camera.h>
#include <Window/Window.h>
#include <Tests/Torus.h>
#include <Tests/Cube.h>
#include <Tests/Sphere.h>
#include <iostream>
#include <GL/glew.h>


UINT32 CompileShader(const char* src, UINT32 type)
{
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		std::ofstream file("log_output", std::ios_base::app);
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)_malloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		file << message;
		file.flush();
		return 0;
	}

	return id;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PWSTR pCmdLine, int nCmdShow)
{
	uint32_t flags = 0;

	Size2 size;
	size.x = 1300;
	size.y = 730;

	Display::Create(hInstance, (WindowFlags)flags, WINDOW_MODE_WINDOWED, size);
	Display* display = Display::GetSingleton();

	float vertexPos[] =
	{
		0.f, -0.5f, 0.f,
		-.5f, .5f, 0.f,
		.5f, .5f, 0.f,
	};

	unsigned int indicies[] = { 0, 1, 2 };
	unsigned int va;
	glGenVertexArrays(1, &va);
	glBindVertexArray(va);


	unsigned int vb;
	glGenBuffers(1, &vb);
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, vb));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPos), (void*)vertexPos, GL_STATIC_DRAW));

	unsigned int ib;
	glGenBuffers(1, &ib);
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), (void*)indicies, GL_STATIC_DRAW));

	const char* vsS = "#version 410 core \n"
		"layout(location = 0) in vec4 vPos; \n"
		"void main()\n"
		"{\n"
		"   gl_Position = vPos;\n"
		"};";
	const char* fsS = "#version 410 core \n"
		"out vec4 vCol;\n"
		"void main()\n"
		"{\n"
		"   vCol = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
		"}";


	int program;

	program = glCreateProgram();
	unsigned int vs = CompileShader(vsS, GL_VERTEX_SHADER);
	unsigned int fs = CompileShader(fsS, GL_FRAGMENT_SHADER);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	GLCall(glUseProgram(program));

	//
	int z= 0;
	//display->ShowWindow(1);
	while (!Display::isCloseRequest)
	{

		
		
		GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LEQUAL));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
		display->ProcessEvents();

		if (z == 0)
		{

			z = 1;
		}

		//display->m_Windows[0]->MakeCurrent();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		display->SwapBuffer(0);
	}
}
