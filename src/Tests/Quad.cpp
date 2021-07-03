#include "Quad.h"

Quad::Quad()
{
	float vertexPos[] =
	{
		-0.5f, 0.5f, 0.f, 0.0f, 0.0f,
		 0.5f, 0.5f, 0.f, 1.f,  0.f,
		 0.5f, -.5f, 0.f, 1.f,  1.0f,
		-.5f,  -.5f, 0.f, 0.0f,  1.0f
	};

	float texCoord[] =
	{
		0.0f, 0.0f,
		1.f,  0.f,
		1.f,  1.0f,
		0.0f,  1.0f
	};
	unsigned int indicies[] = { 0, 1, 3, 3, 2, 1 };

	m_vao = std::make_unique<VertexArray>();
	m_ib = std::make_unique<IndexBuffer>(indicies, 6);
	m_vb = std::make_unique<VertexBuffer>(vertexPos, sizeof(vertexPos));
	layout = std::make_unique<VertexBufferLayout>();
	m_texture = std::make_unique<Texture>();
	shader = Shader("src/Shaders/default.shader");

	layout->Push<float>(VertexBufferLayout::VEC3, 0);
	layout->Push<float>(VertexBufferLayout::VEC2, 1);

	m_vao->AddBuffer(*m_vb, *layout);

}

void Quad::AddTexture(Texture&& tex)
{
	m_texture.get();
	Texture* t = m_texture.get();
	t = &(Texture&)std::move(tex);
}

void Quad::AddTextureImage(const std::string filepath)
{
	m_texture->AddImage(filepath);
}

void Quad::Draw()
{
	m_vao->Bind();
	GLCall(glUseProgram(shader.GetProgram()));
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

