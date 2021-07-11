#include "Cube.h"
#include <GLFW/include/GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Math/Quaternion.h"

Cube::Cube()
{
	m_Va = std::make_unique<VertexArray>();
	GenVertices();
//	m_Shader = std::make_unique<Shader>("src/Shaders/lighting.shader");
	transform = Matrix4x4::Translate(transform, Vector3(0, 0, -15.f));
}


Plane::Plane()
{
	m_Va = std::make_unique<VertexArray>();

	float vertexAttribs[] =
	{
		-0.5f, 0.f,  0.5f,  0.f, 1.f, 0.f, 0.f, 0.f,
		 0.5f, 0.f,  0.5f,  0.f, 1.f, 0.f, 1.f, 0.f,
		 0.5f, 0.f,  -0.5f, 0.f, 1.f, 0.f, 1.f, 1.f,
		-0.5f, 0.f, -0.5f,  0.f, 1.f, 0.f, 0.f, 1.f
	};

	uint32_t indices[] = { 0, 1, 3, 3, 2, 1 };

	m_Va->SetIndices(indices, 6);
	m_Vb = std::make_unique<VertexBuffer>(vertexAttribs, sizeof(vertexAttribs));
	m_Vb->SetLayout(
		{
			{GL_FLOAT, 0, 3, GL_FALSE},
			{GL_FLOAT, 1, 3, GL_FALSE},
			{GL_FLOAT, 2, 2, GL_FALSE}
		});
	m_Va->AddBuffer(*m_Vb);

	transform = Matrix4x4::Scale(transform, 10.f);


}

const Matrix4x4& Cube::GetTransform() const 
{
	return transform;
}

const VertexArray& Cube::GetVertexAttribs() const
{
	return *m_Va;
}

const Shader& Cube::GetShader() const
{
	return *m_Shader;
}

const Matrix4x4& Plane::GetTransform() const 
{
	return transform;
}

const VertexArray& Plane::GetVertexAttribs() const
{
	return *m_Va;
}

const Shader& Plane::GetShader() const
{
	return *m_Shader;
}

static float t = 0;
static Quaternion rot;
void Cube::OnUpdate()
{
	t += 0.1;
	if (t >= 100)
	{
		t = 0;
	}
	rot = Quaternion::RotateY(Math::Deg2Rad(t + 0.012f));
	//rot = rot * Quaternion::RotateZ(Math::Deg2Rad(t));
	// = rot * Quaternion::RotateX(-Math::Deg2Rad(t));
	//transform = Matrix4x4::Rotate(transform, rot);

	//transform = Matrix4x4::Rotate(transform, Quaternion::Roll(Math::Deg2Rad(t)));
}


void Plane::OnUpdate()
{

}

void Cube::GenVertices()
{

	std::vector<VertexAttrib> attrib;

	for (int i = 0;i < 6;i++) 
	{
		Vector3 face_points[4];
		Vector3 normal_points[4];
		float uv_points[8] = { 0,0,0,1,1,1,1,0 };

		for (int j = 0; j < 4; j++) {


			float v[3];
			v[0] = 1.0;
			v[1] = 1 - 2 * ((j >> 1) & 1);
			v[2] = v[1] * (1 - 2 * (j & 1));

			for (int k = 0;k < 3;k++) {

				if (i < 3)
				{
					face_points[j][(i + k) % 3] = v[k] * (i >= 3 ? -1 : 1);
				}
				else
				{
					face_points[3 - j][(i + k) % 3] = v[k] * (i >= 3 ? -1 : 1);
				}
			}

		
			normal_points[j] = Vector3();
			normal_points[j][i % 3] = (i >= 3 ? -1 : 1);
		}

		VertexAttrib a;
		a.vertices = face_points[0];
		a.normals = normal_points[0];
		a.uv = Vector2(uv_points[0 * 2 + 0], uv_points[0 * 2 + 1]);
		attrib.push_back(a);
		a.vertices = face_points[1];
		a.normals = normal_points[1];
		a.uv = Vector2(uv_points[1 * 2 + 0], uv_points[1 * 2 + 1]);
		attrib.push_back(a);
		a.vertices = face_points[2];
		a.normals = normal_points[2];
		a.uv = Vector2(uv_points[2 * 2 + 0], uv_points[2 * 2 + 1]);
		attrib.push_back(a);
		a.vertices = face_points[2];
		a.normals = normal_points[2];
		a.uv = Vector2(uv_points[2 * 2 + 0], uv_points[2 * 2 + 1]);
		attrib.push_back(a);
		a.vertices = face_points[3];
		a.normals = normal_points[3];
		a.uv = Vector2(uv_points[3 * 2 + 0], uv_points[3 * 2 + 1]);
		attrib.push_back(a);
		a.vertices = face_points[0];
		a.normals = normal_points[0];
		a.uv = Vector2(uv_points[0 * 2 + 0], uv_points[0 * 2 + 1]);
		attrib.push_back(a);
	}

	std::vector<uint32_t> indices;
	for (int i = 0; i < 36; i++)
		indices.push_back(i);
	

	m_Vb = std::make_unique<VertexBuffer>(attrib.data(), attrib.size() * sizeof(VertexAttrib));
	m_Vb->SetLayout
	({ {GL_FLOAT, 0,  3, 0},
		{GL_FLOAT, 1, 3, 0},
		{GL_FLOAT, 2, 2, 0},
		});
	
	m_Va->SetIndices(indices.data(), indices.size());
	m_Va->AddBuffer(*m_Vb);
	
}