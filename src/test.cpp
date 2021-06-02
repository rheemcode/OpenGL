////#include <GL/glew.h>
////#include <GLFW/glfw3.h>
////#include <iostream>
////#include "Shader.h"
////#include "glm/glm.hpp"
////#include "glm/gtc/matrix_transform.hpp"
////#include "Renderer.h"
////
////#include "IndexBuffer.h"
////#include "VertexBuffer.h"
////#include "VertexArray.h"
////
////
////#define _USE_MATH_DEFINES
////#include <math.h>
////
////#define PI_ 3.14159265358979323846f
////
////int arraySize;
////
////int width, height;
////float ratio;
////GLFWwindow* window;
////
////
////
////
////static float* drawTorus(int numc, int numt)
////{
////
////    std::vector<float>* torusPos = new std::vector<float>();
////    float s, t, x, y, z, twopi;
////
////    twopi = 2.f * PI_;
////    for (int i = 0; i < numc; i++) {
////        for (int j = 0; j <= numt; j++) {
////            for (int k = 1; k >= 0; k--) {
////                s = (i + k) % numc + 0.5;
////                t = j % numt;
////
////                x = (1.f + .1f * cos(s * twopi / numc)) * cos(t * twopi / numt);
////                y = (1.f + .1f * cos(s * twopi / numc)) * sin(t * twopi / numt);
////                z = .1f * sin(s * twopi / numc);
////
////                torusPos->push_back(x);
////                torusPos->push_back(y);
////                torusPos->push_back(z);
////
////            }
////        }
////    }
////
////    arraySize = torusPos->size();
////
////    return torusPos->data();
////}
////
////
////void initWindow()
////{
////
////
////    /* Initialize the library */
////    if (!glfwInit())
////        return;
////
////    auto* monitor = glfwGetPrimaryMonitor();
////
////    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
////    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
////  //  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
////
////    // Create a windowed mode window and its OpenGL context */
////    window = glfwCreateWindow(700, 600, "Hello World", 0, NULL);
////    if (!window)
////    {
////        glfwTerminate();
////        return;
////    }
////
////    /* Make the window's context current */
////    glfwMakeContextCurrent(window);
////
////
////    if (glewInit() != GLEW_OK)
////        std::cout << "Error!" << std::endl;
////
////    glfwGetFramebufferSize(window, &width, &height);
////
////    ratio = width / height;
////
////    glViewport(0, 0, width, height);
////    glfwSwapInterval(1);
////}
////
////int main(void)
////{
////
////
////    static const float cubePositions[] =
////{
////    -1.0f, -1.0f, -1.0f, 
////    -1.0f, -1.0f, 1.0f, 
////    -1.0f, 1.0f, -1.0f, 
////    -1.0f, 1.0f, 1.0f, 
////    1.0f, -1.0f, -1.0f, 
////    1.0f, -1.0f, 1.0f, 
////    1.0f, 1.0f, -1.0f,
////    1.0f, 1.0f, 1.0f, 
////};
////    static const unsigned int cube_indices[] =
////{
////0, 1, 2, 3, 6, 7, 4, 5, // First strip
////0xFFFF, // <<- - This is the restart index
////2, 6, 0, 4, 1, 5, 3, 7 // Second strip
////};
////
////    float* torusPos = drawTorus(8, 5);
////    std::cout << arraySize << "\n";
////    std::cout << torusPos[200] << "\n";
////    initWindow();
////    
////    VertexArray va;
////    //VertexBuffer vb(sizeof(float) * arraySize);
////    IndexBuffer ibo(cube_indices, 17);
////    VertexBuffer vb(sizeof(cubePositions));
////    //vb.BufferSubData(torusPos, 0, sizeof(float) * arraySize);
////    vb.BufferSubData(cubePositions, 0, sizeof(cubePositions));
////
////   // va.Bind();
////    //vb.Bind();
////
////    unsigned int shader = 0;
////    {
////        Shader shaderSource("src/shader.shader");
////        shader = shaderSource.GetProgram();
////    }
////
////    int mvpLocation = glGetUniformLocation(shader, "MVP");
////    int colorUniform = glGetUniformLocation(shader, "u_Color");
////    int vPosLocation = glGetAttribLocation(shader, "vPos");
////
////    GLCall(glEnableVertexAttribArray(vPosLocation));
////    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0));
////
////    while (!glfwWindowShouldClose(window))
////    {
////        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
////        glEnable(GL_DEPTH_TEST);
////        glDepthFunc(GL_LEQUAL);
////
////        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////
////        glm::mat4x4 m, p, v,  mvp;
////
////        m = glm::rotate(m, 45.f, glm::vec3(1, 1, 0));
////        m = glm::translate(m, glm::vec3(0, 0, -1.8f));
////        p = glm::perspective(60.f, (float)width / height, .0f, 200.0f);
////      //  p = glm::ortho((float)-20, (float)20, (float)20, (float)-20, 20.f, -20.f);
////        v = glm::mat4x4();
////        //v = glm::rotate(glm::mat4x4(), 10.f, glm::vec3(1, 0, 0));
////        mvp = p * v * m;
////
////        glUseProgram(shader);
////
////        glUniform4f(colorUniform, 1.f, 0.f, 0.f, 1.f);
////        GLCall(glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]));
////
////        GLCall(glEnable(GL_PRIMITIVE_RESTART));
////        GLCall(glPrimitiveRestartIndex(0xFFFF));
////
////        GLCall(glDrawElements(GL_TRIANGLE_STRIP, 17, GL_UNSIGNED_INT, NULL));
////
////        glfwSwapBuffers(window);
////        glfwPollEvents();
////
////    }
////
////    glfwTerminate();
////    return 0;
////
////}
//
//#include <iostream>
//#include <vector>
//
//std::vector<unsigned int*> vec;
//
//
//template<typename T>
//void addData(const T& d)
//{
//	vec.push_back(d);
//}
//
//void add(const void* d)
//{
//	vec.push_back((unsigned int)d);
//}
//
//void func()
//{
//	{
//		unsigned int a =  100;
//		addData(&a);
//	}
//}
//
//
//int main()
//{
//	func();
//
//	std::cout << vec.front();
//}