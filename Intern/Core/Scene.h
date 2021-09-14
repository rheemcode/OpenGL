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


struct ShaderTest;
class Renderer;
class FrameBuffer;
class GLApplication;


struct GLIB_API ShadowData
{
	uint32_t splitCount;
	Vector2 ShadowSize;
	Vector3 LightDir;
	Vector4 farBound;
	Matrix4x4 View;
	Matrix4x4 Proj;
	Matrix4x4 Bias;
	Matrix4x4 ProjView;

	ShadowBox shadowBounds;


	void UpdateView(Vector3 direction)
	{
		direction = Vector3::Normalize(direction);
		const auto& center = -shadowBounds.GetCenter();
		View = Matrix4x4();

		float pitch = Math::ACos(Vector2::Length(Vector2(direction.x, direction.z)));

		View = Matrix4x4::Rotate(View, Vector3(1, 0, 0), pitch);
		float yaw = Math::Rad2deg((Math::ATan(direction.x / direction.z)));
		yaw = direction.z > 0 ? yaw - 180 : yaw;
		View = Matrix4x4::Rotate(View, Vector3(0, 1, 0), -Math::Deg2Rad(yaw));

		View = Matrix4x4::Translate(View, center);  
	}

	void UpdateProjection()
	{
		const CameraSettings& camSettings = *shadowBounds.cameraSettings;
		//Proj = Matrix4x4::CreatePerspective(camSettings.fovY, camSettings.ratio, 0.1f, 20.f);
		Proj = Matrix4x4::CreateOrtho(shadowBounds.minX, shadowBounds.maxX, shadowBounds.minY, shadowBounds.maxY, shadowBounds.minZ, shadowBounds.maxZ);
	}
};


class GLIB_API Scene
{
	friend Renderer;
	friend GLApplication;
	static Scene* s_activeScene;

	static std::shared_ptr<Camera> sceneCamera;
	std::shared_ptr<CameraData> cameraData;
	std::shared_ptr<ShadowData> shadowData;

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
	bool meshDirty = false;

	using ShadowBuffer = FrameBuffer;
	std::unique_ptr<ShadowBuffer> m_shadowBuffer;
	std::unique_ptr<class UniformBuffer> m_LightsBuffer;

	/* Scene Shaders */
	std::shared_ptr<class Shader> sceneShader;
	std::shared_ptr<class Shader> shadowShader;
	std::shared_ptr<class Shader> testShader;

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

	void PrepareMeshes();

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
		/*float vAttrib[] = 
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
		frameBuffer->AttachRenderBuffer();*/

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
