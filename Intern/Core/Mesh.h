#pragma once
#include <vector>
#include <memory>
#include "Renderer/Shader.h"
#include "Renderer/Buffers/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Math/Matrix4x4.h"
#include "Math/Transform.h"
#include "Renderer/Material.h"
#include "Math/AABB.h"
#include "Math/frustum.h"

template<typename T>
using Ref = std::unique_ptr<T>;

struct VertexAttrib
{
	Vector3 vertices;
	Vector3 normals;
	Vector2 uv;

	bool operator==(const VertexAttrib& p_other) const
	{
		return ((vertices == p_other.vertices) && (normals == p_other.normals) && (uv == p_other.uv));
	}	

	VertexAttrib()
		: vertices(Vector3()), normals(Vector3()), uv(Vector2()) {}
};

class Mesh
{
	Ref<Shader> m_Shader;
	Ref<VertexArray> m_Va;
	Ref<VertexBuffer> m_Vb;
	Ref<Material> m_material;
	Ref<AABB> m_aabb;
	Ref<InstanceBounds> m_instanceBounds;
	Transform m_transform;

	friend class Model;
	friend class MeshRendererComponent;
	Transform& GetTransformRef() { return m_transform; }
public:
	const Matrix4x4 GetModelMatrix() const { return m_transform.GetWorldMatrix(); };
	void SetParent(const Transform& p_transform) { m_transform.SetParent(p_transform); }
	const VertexArray& GetVertexAttribs() const { return *m_Va; };
	const Transform& GetTransform() const { return m_transform; }
	const Shader& GetShader() const { return *m_Shader; };
	const Material& GetMaterial() const { return *m_material; }
	const AABB& GetAABB() const { return *m_aabb;  }
	const InstanceBounds& GetInstanceBound() const { return *m_instanceBounds; }
	 
	Mesh& operator=(Mesh&& p_mesh) noexcept
	{
		m_Va = std::move(p_mesh.m_Va);
		m_Vb = std::move(p_mesh.m_Vb);
		m_Shader = std::move(p_mesh.m_Shader);
		m_material = std::move(p_mesh.m_material);
		m_aabb = std::move(p_mesh.m_aabb);

		return *this;
	}
	
	 Mesh(Mesh&& p_mesh) noexcept;
	 Mesh(const std::vector<VertexAttrib>& p_vAttribs, const std::vector<uint32_t>& p_indices, Ref<Material>& p_material, Ref<AABB>& p_aabb);
	 Mesh(VertexAttrib* p_vAttribs, uint32_t* p_indices, uint32_t count, Ref<Material>& p_material, Ref<AABB>& p_aabb);
	 ~Mesh() = default;
};

namespace std
{
	template<>
	struct hash<VertexAttrib>
	{
		size_t operator()(const VertexAttrib& p_val) const
		{
			return ((hash<Vector3>()(p_val.vertices) ^ (hash<Vector3>()(p_val.normals) << 1) ^ (hash<Vector2>()(p_val.uv) << 1)));
		}
	};
}