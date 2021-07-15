#pragma once
#include "Renderer/VertexArray.h"
#include "Renderer/Buffers/IndexBuffer.h"
#include "Renderer/Texture.h"
#include <Renderer/Shader.h>
#include "Camera.h"

class Quad
{
	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<IndexBuffer> m_ib;
	std::unique_ptr<VertexBuffer> m_vb;
	std::unique_ptr<BufferLayout> layout;

	Shader shader;

	std::unique_ptr<Texture> m_texture;
	int m_vertexCount;
	int m_Indices;

public:
	Quad();
	void AddTexture(Texture&& tex);
	void AddTextureImage(const std::string filepath);
	void Draw(const Camera& cam);
	void Bind() { m_vao->Bind(); }
	int GetIndicies() { return m_Indices;  }
	~Quad() {}

};

