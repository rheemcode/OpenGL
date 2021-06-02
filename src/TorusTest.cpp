#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Renderer.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"


#define _USE_MATH_DEFINES
#include <math.h>

#define PI_ 3.14159265358979323846f

int arraySize;

int width, height;
float ratio;
GLFWwindow* window;

int mainSegments  = 10, tubeSegments = 4, numIndicies, restartIndex, numVertices;
float mainRadius = 4.f, tubeRadius = 1.5f;

struct vec3 { float x, y, z; };

std::vector<vec3> torusVertexPositions;
std::vector<unsigned int> indicies;

void DrawTorus()
{


    numVertices = (mainSegments + 1) * (tubeSegments + 1);
    restartIndex = numVertices;
    numIndicies = (mainSegments * 2 * (tubeSegments + 1)) + mainSegments - 1;

    float mainSegmentAngleStep = glm::radians(360.f / float(mainSegments));
    float tubeSegmentAngleStep = glm::radians(360.f / float(tubeSegments));

    float currentMainSegmentAngle = .0f;
    for (int i = 0; i <= mainSegments; i++)
    {
        float sinMainSegment = sin(currentMainSegmentAngle);
        float cosMainSegment = cos(currentMainSegmentAngle);
        float currentTubeSegmentAngle = .0f;

        for (int j = 0; j <= tubeSegments; j++)
        {
            float sinTubeSegment = sin(currentTubeSegmentAngle);
            float cosTubeSegment = cos(currentTubeSegmentAngle);


            torusVertexPositions.push_back(
                {
                (mainRadius + tubeRadius * cosTubeSegment) * cosMainSegment,
                (mainRadius + tubeRadius * cosTubeSegment) * sinMainSegment,
                tubeRadius * sinTubeSegment
                });

            currentTubeSegmentAngle += tubeSegmentAngleStep;
        }

        currentMainSegmentAngle += mainSegmentAngleStep;

    }
}

void GenerateIndicies(IndexBuffer* ib)
{
    unsigned int currentVertexOffset = 0;

    for (int i = 0; i < mainSegments; i++)
    {
        for (int j = 0; j <= tubeSegments; j++)
        {
            unsigned int vertexIndexA = currentVertexOffset;
            ib->AddData(vertexIndexA);
            
            unsigned int vertexIndexB = currentVertexOffset + tubeSegments + 1;
            ib->AddData(vertexIndexB);
            currentVertexOffset++;
        }

        if (i != mainSegments - 1)
        {
            ib->AddData(restartIndex);
        }
     }


}

void initWindow()
{
  
    /* Initialize the library */
    if (!glfwInit())
        return;

    auto* monitor = glfwGetPrimaryMonitor();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      // Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(700, 600, "Hello World", 0, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    glfwGetFramebufferSize(window, &width, &height);

    ratio = width / height;

    glViewport(0, 0, width, height);
    glfwSwapInterval(1);
}

int main(void)
{


    DrawTorus();

    initWindow();

    VertexArray va;
    VertexBuffer vb(torusVertexPositions.data(), (sizeof(float) * 3) * torusVertexPositions.size());

    IndexBuffer indexBuffer(numIndicies);
    GenerateIndicies(&indexBuffer);
    indexBuffer.BufferData();
    

    unsigned int shader = 0;
    {
        Shader shaderSource("src/shader.shader");
        shader = shaderSource.GetProgram();
    }

    int mvpLocation = glGetUniformLocation(shader, "MVP");
    int colorUniform = glGetUniformLocation(shader, "u_Color");
    int vPosLocation = glGetAttribLocation(shader, "vPos");

    GLCall(glEnableVertexAttribArray(vPosLocation));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));

    while (!glfwWindowShouldClose(window))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4x4 m, p, v, mvp;

        m = glm::rotate(m, (float)glfwGetTime(), glm::vec3(1.f, 1.f, 1.f));
        m = glm::scale(m, glm::vec3(.5f, .5f, .5f));

        p = glm::perspective(65.f, (float) width / height, 0.f, 200.f);
     //   p = glm::ortho((float)-20, (float)20, (float)20, (float)-20, 20.f, -20.f);
        v = glm::mat4x4();
        v = glm::translate(v, glm::vec3(0.f, 0.f, -5.f));

        mvp = p * v * m;

        glUseProgram(shader);

        glUniform4f(colorUniform, 1.f, 0.f, 0.f, 1.f);
        GLCall(glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]));

        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(restartIndex);
        glDrawElements(GL_TRIANGLE_STRIP, numIndicies, GL_UNSIGNED_INT, NULL);

        glDisable(GL_PRIMITIVE_RESTART);
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;

}
