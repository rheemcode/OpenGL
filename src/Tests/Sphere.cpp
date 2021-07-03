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
    //glm::perspective()
    std::cout << "Created";
    GenSphereVertices();
    CreateVertexArray();
    CreateVertexBuffer();
    CreateIndexBuffer();

    GenIndicies();

    m_Shader = std::make_unique<Shader>("src/shader.shader");
    EnableAttribs();
    m_NumIndicies = m_Ib->GetCount();
    transform = Matrix4x4::Translate(transform, Vector3(3.7f, 1.f, 0.f));

}

void Sphere::CreateIndexBuffer()
{
    m_Ib = std::make_unique<IndexBuffer>(m_NumIndicies);
}

void Sphere::CreateVertexArray()
{

    m_Va = std::make_unique<VertexArray>();
}

void Sphere::CreateVertexBuffer()
{
    m_Vb = std::make_unique<VertexBuffer>(m_VertexPositions.data(), (sizeof(float) * 4) * m_VertexPositions.size());
}

static Matrix4x4 p;

void Sphere::Draw(const Camera& camera)
{
    m_Va->Bind();
    glUseProgram(m_Shader->GetProgram());

    glUniform4f(m_UniformLocation, 1.f, 0.f, 0.f, 1.f);
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &camera.GetViewMatrix()[0].x);
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &transform[0].x);
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &camera.GetProjectionMatrix()[0].x);

    glDrawElements(GL_TRIANGLES, m_NumIndicies, GL_UNSIGNED_INT, NULL);
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
    float sectorStep = 2 * PI / m_SectorSegments;
    float stackStep = PI / m_StackSegments;
    
    float sectorAngle, stackAngle;
    float z, xy;
    
    for (int i = 0; i <= m_StackSegments; ++i)
    {
        stackAngle = PI / 2 - i * stackStep;
        xy = m_radius * Math::Cos(stackAngle);
        z = m_radius * Math::Sin(stackAngle);
   
        for (int j = 0; j <= m_SectorSegments; ++j)
        {
            sectorAngle = j * sectorStep;
            float x = xy * Math::Cos(sectorAngle);
            float y = xy * Math::Sin(sectorAngle);
            m_VertexPositions.push_back({x,y,z, 1.f});
        }

    }

}

void Sphere::GenIndicies()
{
    int k1, k2;

    for (int i = 0; i < m_StackSegments; i++)
    {
        k1 = i * (m_SectorSegments + 1);
        k2 = k1 + m_SectorSegments + 1;
        for (int j = 0; j <= m_SectorSegments; j++)
        {
            if (i != 0)
            {
               // m_Ib->AddData(k1);
                //m_Ib->AddData(k2);
                //m_Ib->AddData(k1 + 1);
            }

            if (i != (m_StackSegments - 1))
            {
               // m_Ib->AddData(k1 + 1);
               // m_Ib->AddData(k2);
                //m_Ib->AddData(k2 + 1);
            }

            k1++;
            k2++;
        }
    }
}

void Sphere::EnableAttribs()
{
    unsigned int shader = m_Shader->GetProgram();

    m_UniformLocation = glGetUniformLocation(shader, "u_Color");
    int vPosLocation = glGetAttribLocation(shader, "vPos");


    viewLocation = glGetUniformLocation(shader, "viewMatrix");
    modelLocation = glGetUniformLocation(shader, "modelMatrix");
    projLocation = glGetUniformLocation(shader, "projMatrix");

    GLCall(glEnableVertexAttribArray(vPosLocation));
    GLCall(glVertexAttribPointer(vPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0));
}

Sphere::~Sphere()
{
    std::cout << "deleted";
}