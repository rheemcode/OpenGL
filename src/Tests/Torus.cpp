#pragma once
#include "Torus.h"
#include <GLFW/include/GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"


Torus::Torus(int mainSegments, int tubeSegments, float mainRadius, float tubeRadius)
    : m_MainSegments(mainSegments), m_TubeSegments(tubeSegments),
    m_TubeRadius(tubeRadius), m_MainRadius(mainRadius)
{

    GenTorusVertices();

    CreateVertexArray();
    CreateVertexBuffer();
    CreateIndexBuffer();

    GenIndicies();

    m_Ib->BufferData();
    m_Shader = std::make_unique<Shader>("src/shader.shader");
    EnableAttribs();

    Scale();

}

void Torus::CreateIndexBuffer()
{
    m_Ib = std::make_unique<IndexBuffer>(m_NumIndicies);
}

void Torus::CreateVertexArray()
{

    m_Va = std::make_unique<VertexArray>();
}

void Torus::CreateVertexBuffer()
{
    m_Vb = std::make_unique<VertexBuffer>(m_VertexPositions.data(), (sizeof(float) * 3) * m_VertexPositions.size());
}

static glm::mat4x4 p;

void Torus::Draw(const Camera& camera)
{
    m_Va->Bind();

    Rotate();
    glUseProgram(m_Shader->GetProgram());

    glUniform4f(m_UniformLocation, 1.f, 0.f, 0.f, 1.f);

    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transform));
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(m_RestartIndex);

     glDrawElements(GL_TRIANGLE_STRIP, m_NumIndicies, GL_UNSIGNED_INT, NULL);
}

void Torus::Rotate()
{
    transform = glm::rotate(transform, (float)0.01f, glm::vec3(1.f, 1.f, 1.f));
}

void Torus::Scale()
{
    transform = glm::scale(transform, glm::vec3(.5f, .5f, .5f));
}


void Torus::GenTorusVertices()
{
    m_NumVertices = (m_MainSegments + 1) * (m_TubeSegments + 1);
    m_RestartIndex = m_NumVertices;
    m_NumIndicies = (m_MainSegments * 2 * (m_TubeSegments + 1)) + m_MainSegments - 1;

    float mainSegmentAngleStep = glm::radians(360.f / float(m_MainSegments));
    float tubeSegmentAngleStep = glm::radians(360.f / float(m_TubeSegments));

    float currentMainSegmentAngle = .0f;
    for (int i = 0; i <= m_MainSegments; i++)
    {
        float sinMainSegment = sin(currentMainSegmentAngle);
        float cosMainSegment = cos(currentMainSegmentAngle);
        float currentTubeSegmentAngle = .0f;

        for (int j = 0; j <= m_TubeSegments; j++)
        {
            float sinTubeSegment = sin(currentTubeSegmentAngle);
            float cosTubeSegment = cos(currentTubeSegmentAngle);


            m_VertexPositions.push_back(
                {
                (m_MainRadius + m_TubeRadius * cosTubeSegment) * cosMainSegment,
                (m_MainRadius + m_TubeRadius * cosTubeSegment) * sinMainSegment,
                m_TubeRadius * sinTubeSegment
                });

            currentTubeSegmentAngle += tubeSegmentAngleStep;
        }

        currentMainSegmentAngle += mainSegmentAngleStep;

    }

}

void Torus::GenIndicies()
{
    unsigned int currentVertexOffset = 0;

    for (int i = 0; i < m_MainSegments; i++)
    {
        for (int j = 0; j <= m_TubeSegments; j++)
        {
            unsigned int vertexIndexA = currentVertexOffset;
            m_Ib->AddData(vertexIndexA);

            unsigned int vertexIndexB = currentVertexOffset + m_TubeSegments + 1;
            m_Ib->AddData(vertexIndexB);
            currentVertexOffset++;
        }

        if (i != m_MainSegments - 1)
        {
            m_Ib->AddData(m_RestartIndex);
        }
    }
}

void Torus::EnableAttribs()
{
    unsigned int shader = m_Shader->GetProgram();

    m_UniformLocation = glGetUniformLocation(shader, "u_Color");
    int vPosLocation = glGetAttribLocation(shader, "vPos");


    viewLocation = glGetUniformLocation(shader, "viewMatrix");
    modelLocation = glGetUniformLocation(shader, "modelMatrix");
    projLocation = glGetUniformLocation(shader, "projMatrix");

    GLCall(glEnableVertexAttribArray(vPosLocation));
    GLCall(glVertexAttribPointer(vPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));
}