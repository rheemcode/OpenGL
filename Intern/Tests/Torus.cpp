//#pragma once
//#include "Torus.h"
//#include <GLFW/include/GLFW/glfw3.h>
//#include <glm/gtc/type_ptr.hpp>
//#include "Camera.h"
//#include "Math/Matrix4x4.h"
//#include "Math/SimpleVec.h"
//#include <iostream>
//#include <vector>
//
//Torus::Torus(const Torus& t)
//{
//    
//}
//
//Torus::Torus(int mainSegments, int tubeSegments, float mainRadius, float tubeRadius)
//    : m_MainSegments(mainSegments), m_TubeSegments(tubeSegments),
//    m_TubeRadius(tubeRadius), m_MainRadius(mainRadius)
//{
//    //glm::perspective()
//    std::cout << "Created";
//    GenTorusVertices();
//    CreateVertexArray();
//    CreateVertexBuffer();
//    CreateIndexBuffer();
//
//    GenIndicies();
//
//    m_Shader = std::make_unique<Shader>("src/shader.shader");
//    EnableAttribs();
//
//    glm::mat4 m;
//    Scale();
//    m = glm::scale(m, { .5f, .5f, .5f });
//    transform = Matrix4x4::Translate(transform, Vector3(0.f, 2.f, -5.f));
//    m = glm::translate(m, glm::vec3(0.f, 2.f, 0.f));
//
//}
//
//void Torus::CreateIndexBuffer()
//{
//    m_Ib = std::make_unique<IndexBuffer>(m_NumIndicies);
//}
//
//void Torus::CreateVertexArray()
//{
//
//    m_Va = std::make_unique<VertexArray>();
//}
//
//void Torus::CreateVertexBuffer()
//{
//    m_Vb = std::make_unique<VertexBuffer>(m_VertexPositions.data(), (sizeof(float) * 4) * m_VertexPositions.size());
//}
//
//static Matrix4x4 p;
//
//void Torus::Draw(const Camera& camera)
//{
//    m_Va->Bind();
//    glUseProgram(m_Shader->GetProgram());
//
//   // glUniform4f(m_UniformLocation, 1.f, 0.f, 0.f, 1.f);
//    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &camera.GetViewMatrix()[0].x);
//    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &transform[0].x);
//    glUniformMatrix4fv(projLocation, 1, GL_FALSE, &camera.GetProjectionMatrix()[0].x);
//
//    glEnable(GL_PRIMITIVE_RESTART);
//    glPrimitiveRestartIndex(m_RestartIndex);
//
//    glDrawElements(GL_TRIANGLE_STRIP, m_NumIndicies, GL_UNSIGNED_INT, NULL);
//}
//
//void Torus::Rotate()
//{
//    transform = Matrix4x4::Rotate(transform, Vector3(1.f, 1.f, 1.f),(float)0.01f);
//}
//
//void Torus::Scale()
//{
//    transform = Matrix4x4::Scale(transform, Vector3(.5f, .5f, .5f));
//}
//
//
//void Torus::GenTorusVertices()
//{
//    m_NumVertices = (m_MainSegments + 1) * (m_TubeSegments + 1);
//    m_RestartIndex = m_NumVertices;
//    m_NumIndicies = (m_MainSegments * 2 * (m_TubeSegments + 1)) + m_MainSegments - 1;
//
//    float mainSegmentAngleStep = glm::radians(360.f / float(m_MainSegments));
//    float tubeSegmentAngleStep = glm::radians(360.f / float(m_TubeSegments));
//
//    float currentMainSegmentAngle = .0f;
//    for (int i = 0; i <= m_MainSegments; i++)
//    {
//        float sinMainSegment = sin(currentMainSegmentAngle);
//        float cosMainSegment = cos(currentMainSegmentAngle);
//        float currentTubeSegmentAngle = .0f;
//
//        for (int j = 0; j <= m_TubeSegments; j++)
//        {
//            float sinTubeSegment = sin(currentTubeSegmentAngle);
//            float cosTubeSegment = cos(currentTubeSegmentAngle);
//
//
//            m_VertexPositions.push_back(
//                {
//                (m_MainRadius + m_TubeRadius * cosTubeSegment) * cosMainSegment,
//                (m_MainRadius + m_TubeRadius * cosTubeSegment) * sinMainSegment,
//                m_TubeRadius * sinTubeSegment,
//                1.f
//                });
//
//            currentTubeSegmentAngle += tubeSegmentAngleStep;
//        }
//
//        currentMainSegmentAngle += mainSegmentAngleStep;
//
//    }
//
//}
//
//void Torus::GenIndicies()
//{  
//    std::vector<unsigned int> indicies;
//    unsigned int currentVertexOffset = 0;
//
//    for (int i = 0; i < m_MainSegments; i++)
//    {
//        for (int j = 0; j <= m_TubeSegments; j++)
//        {
//            unsigned int vertexIndexA = currentVertexOffset;
//            indicies.push_back(vertexIndexA);
//            
//
//            unsigned int vertexIndexB = currentVertexOffset + m_TubeSegments + 1;
//            indicies.push_back(vertexIndexB);
//            currentVertexOffset++;
//        }
//
//        if (i != m_MainSegments - 1)
//        {
//            indicies.push_back(m_RestartIndex);
//        }
//    }
//
//    m_Ib->BufferData(indicies.data(), sizeof(unsigned int) * indicies.size(), indicies.size());
//}
//
//void Torus::EnableAttribs()
//{
//    unsigned int shader = m_Shader->GetProgram();
//
//    m_UniformLocation = glGetUniformLocation(shader, "u_Color");
//    int vPosLocation = glGetAttribLocation(shader, "vPos");
//
//
//    viewLocation = glGetUniformLocation(shader, "viewMatrix");
//    modelLocation = glGetUniformLocation(shader, "modelMatrix");
//    projLocation = glGetUniformLocation(shader, "projMatrix");
//
//    GLCall(glEnableVertexAttribArray(vPosLocation));
//    GLCall(glVertexAttribPointer(vPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0));
//}
//
//Torus::~Torus()
//{
//    std::cout << "deleted";
//}