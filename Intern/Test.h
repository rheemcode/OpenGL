#pragma once 
#include "Window/Window.h"
#include "GL/glew.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/glm.hpp"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"
#include "Renderer/Buffers/FrameBuffer.h"
#include <iostream>
#include "Model.h"

#ifdef TEST
class Test
{

public:
	
	std::unique_ptr<Shader> testShader;
	std::unique_ptr<Shader> shadowShader;
	std::unique_ptr<FrameBuffer> frameBuffer;
	std::unique_ptr<VertexArray> vertexArray;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	StaticModel* testModel;

	unsigned int planeVBO;

	glm::vec3 lightPos = { -2.f, 12.f, 30.f };
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	unsigned int depthMap;
	uint32_t planeVAO;
	Shader simpleDepthShader;
	Shader debugDepthQuad;

	void Init()
	{
		Display* display = Display::GetSingleton();
		display->GetMainWindow()->Init();
		display->GetMainWindow()->MakeCurrent();

		Renderer::Init();
		// build and compile shaders
		// -------------------------
		simpleDepthShader=Shader("./Assets/Shaders/depth.glsl");
		debugDepthQuad =Shader("./Assets/Shaders/depthtest.glsl");



		debugDepthQuad.Bind();
		debugDepthQuad.SetInt("depthMap", 0);

		testModel = new StaticModel("./Assets/house.obj");


		float testRender[] = 
		{
		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f 
		};

		uint32_t quadIndices[] = { 0, 1, 2, 2, 3, 0 };

		vertexArray = std::make_unique<VertexArray>();
		vertexBuffer = std::make_unique<VertexBuffer>(testRender, sizeof(testRender));
		vertexBuffer->SetLayout({ { GL_FLOAT, 0, 3, 0 }, { GL_FLOAT, 1, 2, 0 } });
		vertexArray->SetIndices(quadIndices, 6);
		vertexArray->AddBuffer(*vertexBuffer.get());

		debugDepthQuad.Bind();
		debugDepthQuad.SetInt("depthMap", 0);
		glBindVertexArray(0);

		frameBuffer = std::make_unique<FrameBuffer>();
		//frameBuffer = std::make_unique<FrameBuffer>();
	}

	Shader* GetTestShader() const { return testShader.get(); }
	Shader* GetShadowShader() const { return shadowShader.get();  }

	//void BindFramebuffer() const { frameBuffer->Bind(SHADOW);  }
	//void BindFramebufferTexture() const { frameBuffer->BindTexture(SHADOWMAP); }

	void renderQuad()
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(*vertexArray.get());
	}


	void Run()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		simpleDepthShader.Bind();
		auto loc = glGetUniformLocation(simpleDepthShader.GetProgram(),"lightSpaceMatrix");
		glUniformMatrix4fv(loc, 1, GL_FALSE, &lightSpaceMatrix[0].x);
		
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		frameBuffer->Bind(SHADOW);
		glClear(GL_DEPTH_BUFFER_BIT);
		testModel->BindTextures();
		renderScene(simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		// reset viewport
		glViewport(0, 0, 1200, 700);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		debugDepthQuad.Bind();
		debugDepthQuad.SetFloat("near_plane", near_plane);
		debugDepthQuad.SetFloat("far_plane", far_plane);
		debugDepthQuad.SetFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
		frameBuffer->BindTexture(SHADOWMAP);
		renderQuad();

		Display::GetSingleton()->SwapBuffer();
		Display::GetSingleton()->ProcessEvents();

	}
	
	void renderScene(const Shader& shader)
	{
		shader.Bind();
				// floor
		glm::mat4 model = glm::mat4(1.0f);
		//shader.setMat4("model", model);
		auto loc = glGetUniformLocation(shader.GetProgram(), "model");
		glUniformMatrix4fv(loc, 1, GL_FALSE, &model[0].x);
		
		const auto& attribs = testModel->GetMeshes()[0].GetVertexAttribs();
		attribs.Bind();
		RenderCommand::DrawIndexed(attribs);

	}
};
#endif