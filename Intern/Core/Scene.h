#pragma once
#include "Utils/Thread.h"
#include "Math/Transform.h"
#include "Events/Event.h"
#include "Utils/Console.h"
#include "ShadowBox.h"
#include "Utils/Timestep.h"
#include "Utils/CommandBuffer.h"

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

//using namespace FrameBufferName;


struct ShaderTest;
struct ShadowData;
struct CameraData;
class Mesh;
class UniformBuffer;
class GBuffer;
class Renderer;
class FrameBuffer;
class GLApplication;
class SkyBox;
class Shader;
class Actor;

GLIBSTORAGE template GLIB_API class std::shared_ptr<Camera>;
GLIBSTORAGE template GLIB_API class std::shared_ptr<Shader>;
GLIBSTORAGE template GLIB_API class std::unique_ptr<Light>;
GLIBSTORAGE template GLIB_API class std::shared_ptr<FrameBuffer>;
GLIBSTORAGE template GLIB_API class std::shared_ptr<UniformBuffer>;
GLIBSTORAGE template GLIB_API class std::shared_ptr<GBuffer>;
GLIBSTORAGE template GLIB_API class std::shared_ptr<CameraData>;
GLIBSTORAGE template GLIB_API class std::shared_ptr<Actor>;
GLIBSTORAGE template GLIB_API class std::shared_ptr<ShadowData>;
GLIBSTORAGE template GLIB_API class std::shared_ptr<Mesh>;

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
	std::vector<std::shared_ptr<Actor>> m_actors;
	std::vector< Mesh> culledMeshes;
	std::vector< Mesh> meshes;
	bool meshDirty = false;

	using ShadowBuffer = FrameBuffer;
	std::shared_ptr<ShadowBuffer> m_shadowBuffer;
	std::shared_ptr<UniformBuffer> m_LightsBuffer;
	std::shared_ptr<UniformBuffer> m_MatrixBuffer;
	std::shared_ptr<GBuffer> m_Gbuffer;
	/* Scene Shaders */
	std::shared_ptr<Shader> sceneShader;
	std::shared_ptr<Shader> shadowShader;
	std::shared_ptr<Shader> testShader;

	std::shared_ptr<CameraData> cameraData;
	std::shared_ptr<ShadowData> shadowData;

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
	
	void BindFBO(int name);
	void BindFBOTex(int name);

	void PrepareMeshes();

	void Render();
	void OnUpdate(float p_delta);
	void OnEvent(const Event& event);

	void CreateActor();
	void CreateSkyLight();

	void InitSceneShaders();
	void InitRenderer();
	void InitSceneCamera();
	void InitLightBuffer();
	void CreateBuffers();
	void CreateTests();
	void InitScene();

public:

	static Scene* GetActiveScene();

	void SetMainCamera(std::shared_ptr<Camera> p_camera) { sceneCamera = p_camera; }
	const std::string& GetSceneName() const { return sceneName; }
	void CreateDefaultActor();
	const std::vector<Mesh>& GetCulledMeshes();
	const Camera& GetSceneCamera() const { return *sceneCamera; }
	
	Vector3 GetSkyLightDirection() const { return m_lights[0]->Direction; }
	const EnviromentLight& GetEnviromentLight();
	const std::array<std::unique_ptr<Light>, 10>& GetLight();

	void AddActor(std::shared_ptr<class Actor>& p_actor);

	Scene(const std::string& p_name);
	~Scene();
};

//
//struct ShaderTest
//{
//	struct ShaderParams
//	{
//		float iTime;
//
//	};
//
//	static const int MAX_TEST_SHADERS = 5;
//	std::unique_ptr<VertexArray> vertexArray;
//	std::unique_ptr<VertexBuffer> vertexBuffer;
//	std::unique_ptr<FrameBuffer> frameBuffer;
//	
//	std::unique_ptr<Shader> fboShader;
//	std::array<std::unique_ptr<Shader>, MAX_TEST_SHADERS> shaders;
//
//	void CreateShader(const std::string& filePath, int index)
//	{
//		shaders[index] = std::make_unique<Shader>(filePath);
//	}
//
//	void InitTest()
//	{
//		/*float vAttrib[] = 
//		{
//		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
//			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
//		   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
//		};
//
//		uint32_t quadIndices[] = { 0, 1, 2, 2, 3, 0 };
//
//
//		vertexArray = std::make_unique<VertexArray>();
//		vertexBuffer = std::make_unique<VertexBuffer>(vAttrib, sizeof(vAttrib));
//		vertexBuffer->SetLayout({ { GL_FLOAT, 0, 3, 0 }, { GL_FLOAT, 1, 2, 0 } });
//		vertexArray->SetIndices(quadIndices, 6);
//		vertexArray->AddBuffer(*vertexBuffer.get());
//
//		frameBuffer = std::make_unique<FrameBuffer>();
//		frameBuffer->CreateTexture();
//		frameBuffer->AttachColorTexture();
//		frameBuffer->AttachRenderBuffer();*/
//
//	//	fboShader = std::make_unique<Shader>("./Assets/Shaders/fboTest.glsl");
//	}
//
//	void RenderTests(float deltaTime)
//	{
//		for (const auto& shader : shaders)
//		{
//			if (shader == nullptr)
//				return;
//
//			shader->Bind();
//			shader->UploadUniformFloat("iTime", deltaTime);
//			vertexArray->Bind();
//			RenderCommand::DrawIndexed(vertexArray->GetIndicies());
//		}
//	}
//
//	void RenderTest(float deltaTime, int id)
//	{
//		const auto& shader = shaders[id];
//
//		//frameBuffer->Bind(FrameBufferName::COLORBUFFER);
//		glViewport(0, 0, Display::GetSingleton()->GetMainWindow()->GetWidth(), Display::GetSingleton()->GetMainWindow()->GetHeight());
//		//fboShader->Bind();
//
//		shader->Bind();
//		shader->UploadUniformFloat("iTime", Time::GetSingleton()->GetElapsedTime());
//		frameBuffer->BindTexture(FrameBufferTexture::COLOR);
//		vertexArray->Bind();
//		RenderCommand::DrawIndexed(vertexArray->GetIndicies());
//	}
//};
//
//class SDFTest : public ShaderTest
//{
//
//};
