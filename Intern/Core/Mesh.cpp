#include "Mesh.h"
#include "Components/MeshRendererComponent.h"

Mesh::Mesh(const std::vector<VertexAttrib>& p_vAttribs, const std::vector<uint32_t>& p_indices, Ref<Material> p_material, Ref<AABB> p_aabb)
{
	m_material = p_material;
	m_Va = std::make_shared<VertexArray>();
	m_Vb = std::make_shared<VertexBuffer>(p_vAttribs.data(), p_vAttribs.size() * sizeof(VertexAttrib));
	m_Vb->SetLayout
	({
		{GL_FLOAT, 0, 3, 0},
		{GL_FLOAT, 1, 3, 0},
		{GL_FLOAT, 2, 2, 0}

	});

	m_Va->SetIndices(p_indices.data(), uint32_t(p_indices.size()));
	m_Va->AddBuffer(*m_Vb);


	m_aabb = p_aabb;
	m_instanceBounds = std::make_shared<InstanceBounds>(*m_aabb);
}

Mesh::Mesh(VertexAttrib* p_vAttribs, uint32_t* p_indices, uint32_t count, Ref<Material> p_material, Ref<AABB> p_aabb)
{
	m_material = p_material;
	m_Va = std::make_shared<VertexArray>();
	m_Vb = std::make_shared<VertexBuffer>(p_vAttribs, count * sizeof(VertexAttrib));
	m_Vb->SetLayout
	({
		{GL_FLOAT, 0, 3, 0},
		{GL_FLOAT, 1, 3, 0},
		{GL_FLOAT, 2, 2, 0}

	});

	m_Va->SetIndices(p_indices, count);
	m_Va->AddBuffer(*m_Vb);

	m_aabb = p_aabb;
	m_instanceBounds = std::make_shared<InstanceBounds>(*m_aabb);
}

Ref<Model> Mesh::GetModelInstance() const
{
	return m_modelInstance;
}

Mesh::Mesh(Mesh&& p_mesh) noexcept
{
	m_Va = std::move(p_mesh.m_Va);
	m_Vb = std::move(p_mesh.m_Vb);
	m_Shader = std::move(p_mesh.m_Shader);
	m_material = std::move(p_mesh.m_material);
	m_aabb = std::move(p_mesh.m_aabb);
	
	m_instanceBounds = std::make_shared<InstanceBounds>(*m_aabb);
	m_modelInstance = p_mesh.m_modelInstance;
}

//
//Mesh::Mesh(const std::vector<VertexAttrib>& p_vAttribs, const std::vector<uint32_t>& p_indices, const Material& p_material)
//{
//	m_material = std::make_shared<Material>(p_material);
//	m_Va = std::make_shared<VertexArray>();
//	m_Vb = std::make_shared<VertexBuffer>(p_vAttribs.data(), p_vAttribs.size() * sizeof(VertexAttrib));
//	m_Vb->SetLayout
//	({
//		{GL_FLOAT, 0, 3, 0},
//		{GL_FLOAT, 1, 3, 0},
//		{GL_FLOAT, 2, 2, 0}
//
//	});
//
//	m_Va->SetIndices(p_indices.data(), uint32_t(p_indices.size()));
//	m_Va->AddBuffer(*m_Vb);
//}