#pragma once
#include "Thread.h"
#include "Renderer/Renderer.h"
#include "CommandBuffer.h"
#include "Buffers/FrameBuffer.h"
#include "Buffers/UniformBuffer.h"
#include "Renderer/SkyBox.h"
#include "Math/Transform.h"
#include "Events/Event.h"
#include "Console.h"
#include "ShadowBox.h"
#include "Timestep.h"

struct GLIB_API Light
{
	friend class Scene;

	enum LightSource
	{
		DIRECTIONAL_LIGHT = 1,
		POINT_LIGHT,
		SPOT_LIGHT
	};

	LightSource LightSource;

	Vector3 LightColor;
	Vector3 Position;
	Vector3 Direction;
	Vector2 LightAttenuation;

	float Energy;
	bool Use;

	//Transform transform;
};

struct DirectionalLight : Light
{

};

struct PointLight : Light
{
	float Radius;
};

struct SpotLight : Light
{
	float Radius;
	float outerCutoff;
	float innerCutoff; // innercutoff;
};

struct LightUniformBuffer
{
	Vector4 LightColor;
	Vector3 Direction;
	float Energy;
	bool Use;
};

using namespace FrameBufferName;

struct Timer
{
	std::chrono::steady_clock::time_point start;
	std::chrono::duration<float> duration;
	Timer()
	{
		start = std::chrono::high_resolution_clock().now();
	}

	~Timer()
	{
		auto end = std::chrono::high_resolution_clock().now();
		duration = end - start;
		Console::Log(LogMode::DEBUG, "function took:");
		Console::Log(LogMode::DEBUG, std::to_string(duration.count() * 1000.f));
		Console::Log(LogMode::DEBUG, "ms\n");
		Console::Log(LogMode::DEBUG, std::to_string(duration.count()));
		Console::Log(LogMode::DEBUG, "s\n");
	}
};

struct ShaderTest;
class Renderer;
class FrameBuffer;
class GLApplication;

class GLIB_API Scene
{
	friend Renderer;
	friend GLApplication;
	static Scene* s_activeScene;
	static std::shared_ptr<Camera> sceneCamera;

	std::string sceneName;

	enum DrawMode
	{
		NORMAL,
		WIREFRAME,
	};

	SkyBox* skybox;

	struct EnviromentLight
	{
		Vector3 Ambient;
		float Energy;
	};

	/* Lights */
	uint32_t lightCount;
	std::array<std::unique_ptr<Light>, 10> m_lights;
	EnviromentLight m_EnviromentLight;

	/* Render-ables */
	std::vector<std::shared_ptr<class Actor>> m_actors;
	std::vector<class Mesh> culledMeshes;
	std::vector<class Mesh> meshes;

	using ShadowBuffer = FrameBuffer;
	std::unique_ptr<ShadowBuffer> m_shadowBuffer;
	std::unique_ptr<class UniformBuffer> m_LightsBuffer;

	/* Scene Shaders */
	std::unique_ptr<class Shader> sceneShader;
	std::unique_ptr<class Shader> shadowShader;
	std::unique_ptr<class Shader> testShader;

	
	ShadowBox m_shadowBox;


	/* Threading */
	Thread::ID threadID;
	Thread thread;
	std::recursive_mutex mutex;
	mutable CommandBuffer commandQueue;
	uint32_t drawPending = 0;
	bool running = true;

	/* Thread Methods */
	static void ThreadCallback(void* p_instance);

private:
	
	void BindFBO(FrameBufferName::Type name);
	void BindFBOTex(FrameBufferTexture::Type name);

	void Render();
	void OnUpdate(float p_delta);
	void OnEvent(const Event& event);

	void CreateActor();
	void CreateSkyLight();

	void InitSceneShaders();
	void InitRenderer();
	void InitSceneCamera();
	void InitLightUniforms();
	void CreateBuffers();
	void CreateTests();
	void InitScene();

public:

	static Scene* GetActiveScene();
	const std::string& GetSceneName() const { return sceneName; }
	void CreateDefaultActor();

	const std::vector<Mesh>& GetCulledMeshes();
	const Camera& GetSceneCamera() const { return *sceneCamera; }

	const ShadowBox& GetShadowBox() const { return m_shadowBox; }
	Vector3 GetSkyLightDirection() const { return m_lights[0]->Direction; }
	const EnviromentLight& GetEnviromentLight();
	const std::array<std::unique_ptr<Light>, 10>& GetLight();

	void AddActor(std::shared_ptr<class Actor>& p_actor);

	Scene(const std::string& p_name);
	~Scene();
};


struct ShaderTest
{
	struct ShaderParams
	{
		float iTime;

	};

	static const int MAX_TEST_SHADERS = 5;
	std::unique_ptr<VertexArray> vertexArray;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<FrameBuffer> frameBuffer;
	
	std::unique_ptr<Shader> fboShader;
	std::array<std::unique_ptr<Shader>, MAX_TEST_SHADERS> shaders;

	void CreateShader(const std::string& filePath, int index)
	{
		shaders[index] = std::make_unique<Shader>(filePath);
	}

	void InitTest()
	{
		float vAttrib[] = 
		{
		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		uint32_t quadIndices[] = { 0, 1, 2, 2, 3, 0 };


		vertexArray = std::make_unique<VertexArray>();
		vertexBuffer = std::make_unique<VertexBuffer>(vAttrib, sizeof(vAttrib));
		vertexBuffer->SetLayout({ { GL_FLOAT, 0, 3, 0 }, { GL_FLOAT, 1, 2, 0 } });
		vertexArray->SetIndices(quadIndices, 6);
		vertexArray->AddBuffer(*vertexBuffer.get());

		frameBuffer = std::make_unique<FrameBuffer>();
		frameBuffer->CreateTexture();
		frameBuffer->AttachColorTexture();
		frameBuffer->AttachRenderBuffer();

	//	fboShader = std::make_unique<Shader>("./Assets/Shaders/fboTest.glsl");
	}

	void RenderTests(float deltaTime)
	{
		for (const auto& shader : shaders)
		{
			if (shader == nullptr)
				return;

			shader->Bind();
			shader->UploadUniformFloat("iTime", deltaTime);
			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray->GetIndicies());
		}
	}

	void RenderTest(float deltaTime, int id)
	{
		const auto& shader = shaders[id];

		//frameBuffer->Bind(FrameBufferName::COLORBUFFER);
		glViewport(0, 0, Display::GetSingleton()->GetMainWindow()->GetWidth(), Display::GetSingleton()->GetMainWindow()->GetHeight());
		//fboShader->Bind();

		shader->Bind();
		shader->UploadUniformFloat("iTime", Time::GetSingleton()->GetElapsedTime());
		frameBuffer->BindTexture(FrameBufferTexture::COLOR);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray->GetIndicies());
	}
};

class SDFTest : public ShaderTest
{

};
