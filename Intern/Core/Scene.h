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


struct ShaderTest;
struct ShadowData;
struct CameraData;
class Mesh;
class UniformBuffer;
class GBuffer;
class Renderer;
class Framebuffer;
class GLApplication;
class SkyBox;
class Shader;
class Actor;
class SSAO;

GLIBSTORAGE template GLIB_API class std::shared_ptr<Camera>;
GLIBSTORAGE template GLIB_API class std::shared_ptr<Shader>;
GLIBSTORAGE template GLIB_API class std::unique_ptr<Light>;
GLIBSTORAGE template GLIB_API class std::shared_ptr<Framebuffer>;
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

	using ShadowBuffer = Framebuffer;
	std::shared_ptr<ShadowBuffer> m_shadowBuffer;
	std::shared_ptr<UniformBuffer> m_LightsBuffer;
	std::shared_ptr<UniformBuffer> m_MatrixBuffer;
	std::shared_ptr<UniformBuffer> m_ssaoSamplesBuffer;
	std::shared_ptr<UniformBuffer> m_materialsBuffer;

	std::shared_ptr<GBuffer> m_Gbuffer;
	
	/* PostProcess */
	std::shared_ptr<SSAO> m_ssaoEffect;

	/* Scene Shaders */
	std::shared_ptr<Shader> uniformsBufferShader; // For some reason all uniform bindings must be initilized with 1 shader
	std::shared_ptr<Shader> sceneShader;
	std::shared_ptr<Shader> shadowShader;
	std::shared_ptr<Shader> aabbShader;
	std::shared_ptr<Shader> testShader;

	std::shared_ptr<class VertexArray> aabbVertexArray;
	std::shared_ptr<class VertexBuffer> aabbVertexBuffer;

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