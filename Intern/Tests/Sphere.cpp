#pragma once
#include "Sphere.h"
#include <GLFW/include/GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Math/Matrix4x4.h"
#include "Math/SimpleVec.h"
#include <iostream>

Sphere::Sphere(const Sphere& t)
{

}

Sphere::Sphere(int stackSegments, int sectorSegments, float radius)
    : m_StackSegments(stackSegments), m_SectorSegments(sectorSegments), m_radius(radius)
{
    m_Va = std::make_unique<VertexArray>();
    GenIndicies();
    GenSphereVertices();
    transform = Matrix4x4::Translate(transform, Vector3(-2.f, 0, -10.f));
    
    m_material = std::make_unique<Material>();
    m_material->Color = { .0f, .7f, .7f, 1.f };
    m_material->Shininess = 12.f;
    m_material->SpecularHighlights = 1.f;
}

void Sphere::Rotate()
{
    transform = Matrix4x4::Rotate(transform, Vector3(1.f, 1.f, 1.f), (float)0.01f);
}

void Sphere::Scale()
{
    transform = Matrix4x4::Scale(transform, Vector3(.5f, .5f, .5f));
}


void Sphere::GenSphereVertices()
{
    std::vector<VertexAttrib> attrib;

    float sectorStep = 2.f * PI / m_SectorSegments;
    float stackStep = float(PI / m_StackSegments);
    
    float lengthInv = 1.0f / m_radius;
    float sectorAngle, stackAngle;
    float z, xy;
    
    for (int i = 0; i <= m_StackSegments; ++i)
    {
        stackAngle = PI / 2.f - i * stackStep;
        xy = m_radius * Math::Cos(stackAngle);
        z = m_radius * Math::Sin(stackAngle);
   
        for (int j = 0; j <= m_SectorSegments; ++j)
        {
            sectorAngle = j * sectorStep;
            float x = xy * Math::Cos(sectorAngle);
            float y = xy * Math::Sin(sectorAngle);

            VertexAttrib a;
            a.vertices = { x, y, z };
            a.normals = { x * lengthInv, y * lengthInv, z * lengthInv, };
            a.uv = { (float) j / m_SectorSegments, (float) i / m_StackSegments };


            attrib.push_back(a);
        }

    }

    m_Vb = std::make_unique<VertexBuffer>(attrib.data(), attrib.size() * sizeof(VertexAttrib));
    m_Vb->SetLayout
    ({ {GL_FLOAT, 0,  3, GL_FALSE},
        {GL_FLOAT, 1, 3, GL_FALSE},
        {GL_FLOAT, 2, 2, GL_FALSE},
        });
    m_Va->AddBuffer(*m_Vb);

}

const Matrix4x4& Sphere::GetTransform() const
{
    return transform;
}

const VertexArray& Sphere::GetVertexAttribs() const
{
    return *m_Va;
}

const Shader& Sphere::GetShader() const
{
    return *m_Shader;
}

void Sphere::GenIndicies()
{
    int k1, k2;
    std::vector<uint32_t> indices;
    for (int i = 0; i < m_StackSegments; i++)
    {
        k1 = i * (m_SectorSegments + 1);
        k2 = k1 + m_SectorSegments + 1;
        for (int j = 0; j <= m_SectorSegments; j++)
        {
            if (i != 0)
            {
               indices.push_back(k1);
               indices.push_back(k2);
               indices.push_back(k1 + 1);
            }

            if (i != (m_StackSegments - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

            k1++;
            k2++;
        }
    }

    m_Va->SetIndices(indices.data(), uint32_t(indices.size()));
}


Sphere::~Sphere()
{
    std::cout << "deleted";
}