#pragma once
#include <vector>
#include <memory>
#include "Renderer/Shader.h"
#include "Renderer/Buffers/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Math/Matrix4x4.h"
#include "Math/Transform.h"
#include "Renderer/Material.h"

template<typename T>
using Ref = std::unique_ptr<T>;

struct VertexAttrib
{
	Vector3 vertices;
	Vector3 normals;
	Vector2 uv;
};

class Mesh
{
	Ref<Shader> m_Shader;
	Ref<VertexArray> m_Va;
	Ref<VertexBuffer> m_Vb;
	Ref<Material> m_material;

	std::shared_ptr<Matrix4x4> m_modelMatrix;
public:
	 const std::shared_ptr<Matrix4x4>& GetModelMatrix() const { return m_modelMatrix; };
	 const void SetModelMatrix(Matrix4x4* p_modelMat) { m_modelMatrix = std::make_shared<Matrix4x4>(p_modelMat); }
	 const VertexArray& GetVertexAttribs() const { return *m_Va; };
	 const Shader& GetShader() const { return *m_Shader; };
	 const Material& GetMaterial() const { return *m_material; }

	Mesh(const std::vector<VertexAttrib>& p_vAttribs, const std::vector<uint32_t>& p_indices, Ref<Material> p_material);
	Mesh(const std::vector<VertexAttrib>& p_vAttribs, const std::vector<uint32_t>& p_indices, const Material& p_material);
	~Mesh() = default;
};

