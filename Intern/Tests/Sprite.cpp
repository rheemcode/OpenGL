#include "Sprite.h"
#include "Math/Vector3.h"
#include "Math/Vector2.h"
Sprite::Sprite()
{
	struct VertexAttrib
	{
		Vector3 Position;
		Vector2 texCoord;
	};

	VertexAttrib attrib[4];
	attrib[0].Position = { -0.5f, 0.5f, 0.f };
	attrib[1].Position = { 0.5f, 0.5f, 0.f };
	attrib[2].Position = {0.5f, -0.5f, 0.f };
	attrib[3].Position = {-0.5f,- 0.5f, 0.f};
						 
	attrib[0].texCoord = {0.0f, 0.0f };
	attrib[1].texCoord = {1.0f, 0.0f };
	attrib[2].texCoord = {1.0f, 1.0f };
	attrib[3].texCoord = {0.0f, 1.0f };

	unsigned int indicies[] = { 0, 1, 3, 3, 2, 1 };
	
	
}

void Sprite::AddTexture(Texture&& tex)
{
	m_texture.get();
	Texture* t = m_texture.get();
	t = &(Texture&)std::move(tex);
}

const Texture& Sprite::GetTexture() const
{
	return *m_texture.get();
}

void Sprite::AddTextureImage(const std::string filepath)
{
	m_texture = std::make_unique<Texture>(filepath);
}

const Matrix4x4& Sprite::GetTransform() const
{
	return transform;
}


void Sprite::Scale(float scale)
{
	transform = Matrix4x4::Scale(transform, scale);
}
void Sprite::Draw()
{
	//GLCall(glUseProgram(shader.GetProgram()));
	//GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

