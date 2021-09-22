#pragma once
#include <vector>
#include <memory>
#include "OpenGL/Shader.h"
#include "Renderer/Buffers/IndexBuffer.h"
#include "OpenGL/VertexArray.h"
#include "Math/Matrix4x4.h"
#include "Math/Transform.h"
#include "Renderer/Material.h"
#include "Math/AABB.h"
#include "Math/frustum.h"

template<typename T>
using Ref = std::shared_ptr<T>;

struct VertexAttrib
{
	Vector3 vertices;
	Vector3 normals;
	Vector2 uv;
	Vector3 tangent;
	Vector3 bitangent;

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
	Material m_material;
	AABB m_aabb;
	InstanceBounds m_instanceBounds;
	Transform m_transform;

	friend class Model;
	friend class MeshRendererComponent;
	friend class StaticMeshRendererComponent;
	Ref<class Model> m_modelInstance;
	Transform& GetTransformRef() { return m_transform; }
public:
	const Matrix4x4 GetModelMatrix() const { return m_transform.GetWorldMatrix(); };
	void SetParent(const Transform& p_transform) { m_transform.SetParent(p_transform); }
	const VertexArray& GetVertexAttribs() const { return *m_Va; };
	const Transform& GetTransform() const { return m_transform; }
	const Shader& GetShader() const { return *m_Shader; };
	const Material& GetMaterial() const { return m_material; }
	const AABB& GetAABB() const { return m_aabb;  }
	const InstanceBounds& GetInstanceBound() const { return m_instanceBounds; }
	Ref<Model> GetModelInstance() const;
	 
	Mesh& operator=(Mesh&& p_mesh) noexcept
	{
		m_Va = std::move(p_mesh.m_Va);
		m_Vb = std::move(p_mesh.m_Vb);
		m_Shader = std::move(p_mesh.m_Shader);
		m_material = std::move(p_mesh.m_material);
		m_aabb = std::move(p_mesh.m_aabb);
		m_modelInstance = p_mesh.m_modelInstance;
		m_instanceBounds = std::move(p_mesh.m_instanceBounds);
		return *this;
	}
	
	Mesh& operator=(const Mesh& p_mesh)
	{
		m_Va = p_mesh.m_Va;
		m_Vb = p_mesh.m_Vb;
		m_Shader = p_mesh.m_Shader;
		m_material = p_mesh.m_material;
		m_aabb = p_mesh.m_aabb;
		m_modelInstance = p_mesh.m_modelInstance;
		m_instanceBounds = p_mesh.m_instanceBounds;
		return *this;
	}

	Mesh(const Mesh& p_mesh)
	{
		m_Va = p_mesh.m_Va;
		m_Vb = p_mesh.m_Vb;
		m_Shader = p_mesh.m_Shader;
		m_material = p_mesh.m_material;
		m_aabb = p_mesh.m_aabb;
		m_modelInstance = p_mesh.m_modelInstance;
		m_instanceBounds = p_mesh.m_instanceBounds;
	}
	 Mesh(Mesh&& p_mesh) noexcept;
	 Mesh(const VertexAttrib* p_attribs, uint32_t count) {};
	 Mesh(const std::vector<VertexAttrib> p_attribs) {};
	 Mesh(const std::vector<VertexAttrib>& p_vAttribs, const std::vector<uint32_t>& p_indices, Material p_material, AABB p_aabb);
	 Mesh(VertexAttrib* p_vAttribs, uint32_t* p_indices, uint32_t count, Material p_material, AABB p_aabb);
	 Mesh(const std::vector<VertexAttrib>& p_vAttribs, uint32_t* p_indices, uint32_t count, Material p_material, AABB p_aabb);
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