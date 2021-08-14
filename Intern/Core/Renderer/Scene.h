#pragma once
#include <vector>
#include "Thread.h"
#include "CommandBuffer.h"
#include "Tests/Object.h"
#include <Tests/Primitive.h>
#include "Renderer/Renderer.h"
#include "Buffers/FrameBuffer.h"
#include "Renderer/SkyBox.h"
#include "Math/Transform.h"
#include "Events/Event.h"
#include "Console.h"
#include <chrono>


struct Light
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
		Console::Log("function took:");
		Console::Log(std::to_string(duration.count() * 1000.f));
		Console::Log("ms\n");
		Console::Log(std::to_string(duration.count()));
		Console::Log("s\n");
	}
};

class Scene
{
	enum DrawMode
	{
		NORMAL,
		WIREFRAME,
	};

	struct EnviromentLight
	{
		Vector3 Ambient;
		float Energy;
	};
	
	static Scene* s_instance;

	static uint32_t lightCount;
	static std::array<std::unique_ptr<Light>, 10> m_lights;
	static EnviromentLight m_EnviromentLight;

	static uint64_t lastTicks;
	static uint32_t frames;
	static uint32_t frame;

	std::vector<std::unique_ptr<Primitive>> m_Primitives;
	std::vector<std::shared_ptr<class Actor>> m_actors;
	std::vector<class Mesh> culledMeshes;
	std::vector<class Mesh> meshes;

	std::unique_ptr<FrameBuffer> m_frameBuffer;
	std::unique_ptr<UniformBuffer> m_LightsBuffer;
	std::unique_ptr<SceneCamera> sceneCamera;
	static std::unique_ptr<Shader> sceneShader;
	static std::unique_ptr<Shader> shadowShader;
	
	friend class Renderer;
	static const EnviromentLight& GetEnviromentLight();
	static const std::array<std::unique_ptr<Light>, 10>& GetLight();
	static int GetLightCount();

	Thread::ID threadID;
	Thread thread;
	std::recursive_mutex mutex;
	mutable CommandBuffer commandQueue;

	SkyBox* skybox;

	uint32_t drawPending = 0;
	bool running = true;


	void _Render();
	void ThreadRunLoop(float p_delta);
	void _Update(float p_delta);
	void ThreadLoop();
	void ThreadExit();
	void ThreadFlush();
	void ThreadRender();
	void ThreadCreateDefaultActor();
	static void ThreadCallback(void* p_instance);
	void ThreadUpdate(float p_delta);

public:
	void Sync();

	void RunLoop(float p_delta);
	std::vector<Mesh>& GetCulledMeshes();
	const SceneCamera& GetSceneCamera() const { return *sceneCamera; }
	void Render();
	void OnUpdate(float p_delta);
	void AddObject(std::unique_ptr<Primitive>& primitive);
	void AddActor(std::shared_ptr<class Actor>& p_actor);
	void OnEvent(const Event& event);
	void Shutdown();
	void BindFBO(FrameBufferName name) { m_frameBuffer->Bind(name); }
	void BindFBOTex(FrameBufferTexture name) { m_frameBuffer->BindTexture(name); }
	static Scene* GetSingleton();
	void InitLightUniforms();

	static void Init();
	
	void CreateActor();
	void CreateDefaultActor();
	void ThreadBeginScene();
	void BeginScene();
	Scene();
	~Scene();
};

