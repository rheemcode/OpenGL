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

	m_Va->SetIndices(p_indices.data(), p_indices.size());
	m_Va->AddBuffer(*m_Vb);
}