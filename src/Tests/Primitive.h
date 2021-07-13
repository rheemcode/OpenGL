#pragma once
#include <vector>
#include <memory>
#include "Object.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffers/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "Math/Matrix4x4.h"
#include "Renderer/Material.h"

struct vec3 { float x, y, z; };
struct vec4 { float x, y, z, w; };

struct VertexAttrib
{
    Vector3 vertices;
    Vector3 normals;
    Vector2 uv;
};

class Primitive : public Object
{

protected:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_Va;
    std::unique_ptr<VertexBuffer> m_Vb;
    std::unique_ptr<Material> m_material;

public:

    virtual const Matrix4x4& GetTransform() const { return transform; };
    virtual const VertexArray& GetVertexAttribs() const { return *m_Va; };
    virtual const Shader& GetShader() const { return *m_Shader; };
    virtual const Material& GetMaterial() const { return *m_material; }
    virtual void OnUpdate() = 0;
    
    Primitive() {};
    Primitive(Primitive&& primitive)
    {
        m_Va = std::move(primitive.m_Va);
        m_Vb = std::move(primitive.m_Vb);
        m_Shader = std::move(primitive.m_Shader);
        transform = std::move(primitive.transform);
    }

    Matrix4x4 transform;
};
