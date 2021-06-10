#pragma once
#include <vector>
#include <memory>
#include "Object.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

struct vec3 { float x, y, z; };

class Primitive : public Object
{
private:
    virtual void EnableAttribs() = 0;
    virtual void CreateVertexArray() = 0;
    virtual void CreateVertexBuffer() = 0;
    virtual void CreateIndexBuffer() = 0;

protected:
    std::vector<vec3> m_VertexPositions;
    std::vector<unsigned int> m_Indicies;

    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<IndexBuffer> m_Ib;
    std::unique_ptr<VertexArray> m_Va;
    std::unique_ptr<VertexBuffer> m_Vb;
    
    glm::mat4x4 transform;
    glm::mat4x4 viewTransform;
    glm::mat4x4 projTransform;

    int viewLocation, modelLocation, projLocation;

};