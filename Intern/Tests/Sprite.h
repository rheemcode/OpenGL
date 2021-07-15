#pragma once
#include "Renderer/VertexArray.h"
#include "Renderer/Buffers/IndexBuffer.h"
#include "Renderer/Texture.h"
#include <Renderer/Shader.h>

class Sprite
{
	std::unique_ptr<Texture> m_texture;
	Matrix4x4 transform;
	int m_vertexCount;
	int m_Indices;

public:
	Sprite();
	void AddTexture(Texture&& tex);
	const Texture& GetTexture() const;
	void AddTextureImage(const std::string filepath);
	const Matrix4x4& GetTransform() const;
	void Scale(float scale);
	void Draw();
	int GetIndicies() { return m_Indices;  }
	~Sprite() {}

};

