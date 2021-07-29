#pragma once
#include "Texture.h"
#include "VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Math/Matrix4x4.h"
#include "Shader.h"
#include <memory>

class SkyBox
{
	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<VertexBuffer> m_vb;
	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<Texture> m_texture;

	static SkyBox* s_Instance;
	void Init();
public:
	void BeginRender(const Matrix4x4& p_projview)
	{
		m_shader->Bind();
		m_vao->Bind();
		m_texture->BindCubeMap();

		m_shader->UploadUniformMat4("projView", p_projview);
	}
	
	uint32_t GetIndices() const { return m_vao->GetIndicies(); }
	static void Create();
	static SkyBox* GetSingleton() { return s_Instance; }
};

