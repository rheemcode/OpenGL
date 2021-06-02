//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include "Shader.h"
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "Renderer.h"
//
//#include "IndexBuffer.h"
//#include "VertexBuffer.h"
//#include "VertexArray.h"
//#include <array>
//
//#define _USE_MATH_DEFINES
//#include <math.h>
//
//#define TORUS_MAJOR     1.5
//#define TORUS_MINOR     0.5
//#define TORUS_MAJOR_RES 32
//#define TORUS_MINOR_RES 32
//#define TORUS_MINOR_RES 32
//
//
//
//static const float cubePositions[] =
//{
//    -100.0f, -100.0f, -100.0f, 
//    -100.0f, -100.0f, 100.0f, 
//    -100.0f, 100.0f, -100.0f, 
//    -100.0f, 100.0f, 100.0f, 
//    100.0f, -100.0f, -100.0f, 
//    100.0f, -100.0f, 100.0f, 
//    100.0f, 100.0f, -100.0f,
//    100.0f, 100.0f, 100.0f, 
//};
//// Color for each vertex
//static const float cube_colors[] =
//{
//    1.0f, 1.0f, 1.0f, 1.0f,
//    1.0f, 1.0f, 0.0f, 1.0f,
//    1.0f, 0.0f, 1.0f, 1.0f,
//    1.0f, 0.0f, 0.0f, 1.0f,
//    0.0f, 1.0f, 1.0f, 1.0f,
//    0.0f, 1.0f, 0.0f, 1.0f,
//    0.0f, 0.0f, 1.0f, 1.0f,
//    0.5f, 0.5f, 0.5f, 1.0f
//};
//// Indices for the triangle strips
//static const unsigned int cube_indices[] =
//{
//0, 1, 2, 3, 6, 7, 4, 5, // First strip
//0xFFFF, // <<- - This is the restart index
//2, 6, 0, 4, 1, 5, 3, 7 // Second strip
//};
//
//
//static unsigned int indicies[] = { 0, 1, 2, 2, 3, 0 };
//
//
//struct vertexPos { float x, y, z;  };
//
//vertexPos torusList[2112];
//std::vector<vertexPos> torusVec;
//
//
//void drawTorus()
//{
//
//    float twopi = 2.0f * M_PI;
//    float s, t, x, y, z, ny, nx, nz, scale;
//
//    for (int i = 0; i < TORUS_MINOR_RES; i++)
//    {
//        for (int j = 0; j <= TORUS_MAJOR_RES; j++)
//        {
//            for (int k = 1; k >= 0; k--)
//            {
//                s = (i + k) % TORUS_MINOR_RES + 0.5;
//                t = j % TORUS_MAJOR_RES;
//
//                // Calculate point on surface
//                x = (TORUS_MAJOR + TORUS_MINOR * cos(s * twopi / TORUS_MINOR_RES)) * cos(t * twopi / TORUS_MAJOR_RES);
//                y = TORUS_MINOR * sin(s * twopi / TORUS_MINOR_RES);
//                z = (TORUS_MAJOR + TORUS_MINOR * cos(s * twopi / TORUS_MINOR_RES)) * sin(t * twopi / TORUS_MAJOR_RES);
//
//                // Calculate surface normal
//                nx = x - TORUS_MAJOR * cos(t * twopi / TORUS_MAJOR_RES);
//                ny = y;
//                nz = z - TORUS_MAJOR * sin(t * twopi / TORUS_MAJOR_RES);
//               
//                scale = 1.0 / sqrt(nx * nx + ny * ny + nz * nz);
//                nx *= scale;
//                ny *= scale;
//                nz *= scale;
//
//               // glNormal3f((float)nx, (float)ny, (float)nz);
//              //  glVertex3f((float)x, (float)y, (float)z);
//               // std::cout << i + j + k * (32 * 32 * 2) << "\n";
//              //  torusList[i + j + k * 32 * 32 * 2] = { x, y, z };
//                torusVec.push_back({ x * 50.f, y * 50.f, z * 50.f });
//
//            }
//        }
//        
//      //  std::cout << torusVec.size() << "\n";
//    }
//}
//
//int main(void)
//{
//    std::cout << sizeof(GLuint) << "\n";
//    drawTorus();
//
//    GLFWwindow* window;
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//    auto* monitor = glfwGetPrimaryMonitor();
//   
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // Create a windowed mode window and its OpenGL context */
//    window = glfwCreateWindow(700, 600, "Hello World", 0, NULL);
//    if (!window)
//    {
//        glfwTerminate();
//        return -1;
//    }
//    
//
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//
//
//
//    if (glewInit() != GLEW_OK)
//        std::cout << "Error!" << std::endl;
//    
//    int width, height;
//    glfwGetFramebufferSize(window, &width, &height);
//
//    glViewport(0, 0, width, height);
//    glfwSwapInterval(1);
//
//    VertexArray va;
//
//    //IndexBuffer ibo(cube_indices, 17);
//   
//    
//    //VertexBuffer vb(sizeof(cubePositions) + sizeof(cube_colors));
//    //vb.BufferSubData(torusList, 0, sizeof(torusList));
//    //vb.BufferSubData(cube_colors, sizeof(cubePositions), sizeof(cube_colors));
//    VertexBuffer vb(sizeof(float) * torusVec.size() * 3 );
//    vb.BufferSubData(torusVec.data(), 0, sizeof(float) * torusVec.size() * 3);
//    //vb.BufferSubData(cube_colors, sizeof(cubePositions), sizeof(cube_colors));
//
//    va.Bind();
//    //unsigned int vbo;
//
//    unsigned int shader = 0; 
//    {
//        Shader shaderSource("src/shader.shader");
//        shader = shaderSource.GetProgram();
//    }
//
//    
//
//    int mvpLocation = glGetUniformLocation(shader, "MVP");
//    int colorUniform = glGetUniformLocation(shader, "u_Color");
//    int vPosLocation = glGetAttribLocation(shader, "vPos");
//    int vColLocation = glGetAttribLocation(shader, "vCol");
//
//    GLCall(glEnableVertexAttribArray(0));
//    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL));
//    
//  //  GLCall(glEnableVertexAttribArray(1));
//  //  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*) sizeof(cubePositions));
//
//
//    using namespace glm;
//
//    /* Loop until the user closes the window */
//    while (!glfwWindowShouldClose(window))
//    {
//       /* const GLfloat light_position[4] = { 0.0f, 8.0f, 8.0f, 1.0f };
//        const GLfloat light_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
//        const GLfloat light_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
//        const GLfloat light_ambient[4] = { 0.2f, 0.2f, 0.3f, 1.0f };
//
//        glCullFace(GL_BACK);
//        glFrontFace(GL_CW);
//        glLightfv(GL_LIGHT1, GL_POSITION, light_position);
//        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
//        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
//        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
//        glEnable(GL_LIGHT1);*/
//        //glEnable(GL_LIGHTING);
//        // Set up for a glDrawElements call
//
//        mat4x4 m, p, mvp, m2, p2, mvp2;
//
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//        glEnable(GL_DEPTH_TEST);
//        glDepthFunc(GL_LEQUAL);
//
//        /* Render here */
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        float ratio = width / (float)height;
//
//        m = mat4x4();
//       // m = rotate(m, (float)glfwGetTime(), vec3(1, 1, 1));
//        p = ortho((float)-width, (float)width, (float)height, (float) -height, -400.f, 400.f);
//        //p = perspective(65.f * 3.1415926535f / 180.0f, (float) width / height, 1.0f, 50.0f);
//        //p = translate(p, vec3(0.f, 0.f, -100.f));
//        mvp = p * m;
//        
//        glUseProgram(shader);
//        
//       glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);
//
//      //  glUniform4f(colorUniform, 1.f, 0.f, 0.f, 1.f);
//        //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
//
//        // When primitive restart is on, we can call one draw command
//       // GLCall(glEnable(GL_PRIMITIVE_RESTART));
//       // GLCall(glPrimitiveRestartIndex(0xFFFF));
//
//        GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 2112));
//       // GLCall(glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, NULL));
//
//
//        /* Swap front and back buffers */
//        glfwSwapBuffers(window);
//
//        /* Poll for and process events */
//        glfwPollEvents();
//    }
//
//
//    glfwTerminate();
//    return 0;
//}