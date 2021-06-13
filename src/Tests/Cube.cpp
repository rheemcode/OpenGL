#include "Cube.h"
#include <GLFW/include/GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

Cube::Cube()
{
	GenVertices();

	CreateVertexArray();
	CreateVertexBuffer();
	CreateIndexBuffer();

	m_Shader = std::make_unique<Shader>("src/shader.shader");
	EnableAttribs();
	
	transform = glm::mat4x4(1);
}

void Cube::GenVertices()
{
	vec4 currentVertex;

	float x = -1.f, y = -1.f, z = 1.f;
	int n = -1;

	for (int i = 0; i < 8; i++)
	{
		for (int _x = 0; _x < 1; _x++)
		{
			for (int _y = 0; _y < 1; _y++)
			{
				for (int _z = 0; _z < 1; _z++)
				{
					if (i != 0 && !(i % 4) && x != 1.f)
						x = 1.f;
					
					n += 1;
					if (n == 2 || n == 3)
					{
						y = 1.f;
						if (n == 3)
							n = -1;
					}
					else y = -1.f;

					if (z == 1.f)
						z = -1.f;
					else
						z = 1.f;

					currentVertex = { x, y, z, 1.f };
					 

				}
			}
		}

		m_VertexPositions.push_back(currentVertex);
	}
}

void Cube::EnableAttribs()
{
	unsigned int shader = m_Shader->GetProgram();

	viewLocation = glGetUniformLocation(shader, "viewMatrix");
	modelLocation = glGetUniformLocation(shader, "modelMatrix");
	projLocation = glGetUniformLocation(shader, "projMatrix");


	m_UniformLocation = glGetUniformLocation(shader, "u_Color");
	int vPosLocation = glGetAttribLocation(shader, "vPos");

	GLCall(glEnableVertexAttribArray(vPosLocation));
	GLCall(glVertexAttribPointer(vPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));
}

void Cube::CreateVertexArray()
{
	m_Va = std::make_unique<VertexArray>();
}

void Cube::CreateVertexBuffer()
{
	m_Vb = std::make_unique<VertexBuffer>(m_VertexPositions.data(), (sizeof(float) * 3) * m_VertexPositions.size());
}

void Cube::CreateIndexBuffer()
{
	uint32_t ci[] =
	{
		0, 1, 2, 3, 6, 7, 4, 5,
		0xFFFF,
		2, 6, 0, 4, 1, 5, 3, 7 
	};

	m_Ib = std::make_unique<IndexBuffer>(ci, 17);
}

void Cube::Draw(const Camera& camera)
{
	m_Va->Bind();

	transform = glm::rotate(transform, 0.01f, glm::vec3(1, 1, 1));
	glUseProgram(m_Shader->GetProgram());


	glUniform4f(m_UniformLocation, 1.f, 0.f, 0.f, 1.f);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transform));
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));



	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);

	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, NULL);
}