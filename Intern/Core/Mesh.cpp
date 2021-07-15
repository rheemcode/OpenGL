#include "Mesh.h"

Mesh::Mesh(const std::vector<VertexAttrib>& p_vAttribs, const std::vector<uint32_t>& p_indices, Ref<Material> p_material)
{
	m_Va = std::make_unique<VertexArray>();
	m_Vb = std::make_unique<VertexBuffer>(p_vAttribs.data(), p_vAttribs.size() * sizeof(VertexAttrib));
	m_Vb->SetLayout
	({
		{GL_FLOAT, 0, 3, 0},
		{GL_FLOAT, 1, 3, 0},
		{GL_FLOAT, 2, 2, 0}

	});

	m_Va->SetIndices(p_indices.data(), uint32_t(p_indices.size()));
	m_Va->AddBuffer(*m_Vb);
}

Mesh::Mesh(Mesh&& p_mesh)
{
	m_Va = std::move(p_mesh.m_Va);
	m_Vb = std::move(p_mesh.m_Vb);
	m_Shader = std::move(p_mesh.m_Shader);
	m_material = std::move(p_mesh.m_material);

}

Mesh::Mesh(const std::vector<VertexAttrib>& p_vAttribs, const std::vector<uint32_t>& p_indices, const Material& p_material)
{
	m_Va = std::make_unique<VertexArray>();
	m_Vb = std::make_unique<VertexBuffer>(p_vAttribs.data(), p_vAttribs.size() * sizeof(VertexAttrib));
	m_Vb->SetLayout
	({
		{GL_FLOAT, 0, 3, 0},
		{GL_FLOAT, 1, 3, 0},
		{GL_FLOAT, 2, 2, 0}

	});

	m_Va->SetIndices(p_indices.data(), uint32_t(p_indices.size()));
	m_Va->AddBuffer(*m_Vb);
}