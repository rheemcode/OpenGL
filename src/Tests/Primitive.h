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

struct vec3 { float x, y, z; };
struct vec4 { float x, y, z, w; };

class Primitive : public Object
{
private:
    virtual void EnableAttribs() = 0;
    virtual void CreateVertexArray() = 0;
    virtual void CreateVertexBuffer() = 0;
    virtual void CreateIndexBuffer() = 0;

protected:
    std::vector<unsigned int> m_Indicies;

    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<IndexBuffer> m_Ib;
    std::unique_ptr<VertexArray> m_Va;
    std::unique_ptr<VertexBuffer> m_Vb;
    
    Matrix4x4 transform;
    Matrix4x4 viewTransform;
    Matrix4x4 projTransform;

    int viewLocation, modelLocation, projLocation;

};