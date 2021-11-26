#pragma once
#include "Utils/Thread.h"
#include "Math/Transform.h"
#include "Events/Event.h"
#include "Utils/Console.h"
#include "ShadowBox.h"
#include "Utils/Timestep.h"
#include "Utils/CommandBuffer.h"
#include "Camera.h"

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

#define MAX_SPLIT 4
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

	

	struct GLIB_API ShadowData
	{
		uint32_t splitCount = MAX_SPLIT;
		AABB bounds[MAX_SPLIT];

		Camera* camera;

		Vector4 farBound;
		Vector2 ShadowSize;
		Vector3 LightDir;

		Matrix4x4 TextureMatrix[MAX_SPLIT];
		Vector4 depthSplits;
		Vector4 splitDistances;

		void Update()
		{
			float nearClip = 0.1f;
			float farClip = 500.f;
			float clipRange = farClip - nearClip;

			float minZ = nearClip;
			float maxZ = nearClip + clipRange;
			float range = maxZ - minZ;
			float ratio = maxZ / minZ;

			depthSplits[0] = minZ;
			splitDistances[0] = minZ;
			for (uint32_t i = 1; i < 4; i++)
			{
				float p = static_cast<float>(i + 1) / static_cast<float>(4);
				float log = minZ * std::pow(ratio, p);
				float uniform = minZ + range * p;
				float d = 0.5f * (log - uniform) + uniform;
				depthSplits[i] = (d - nearClip) / clipRange;
				splitDistances[i] = log * 0.5f + uniform * (1 - 0.5f);
			}

			splitDistances[3] = maxZ;

			Matrix4x4 modifiedProj = sceneCamera->GetProjectionMatrix();
			Matrix4x4 invCam = Matrix4x4::Inverse(modifiedProj * sceneCamera->GetViewMatrix());
			Matrix4x4 View;
			Matrix4x4 Proj;

			float lastSplitDist = 0.0f;
			for (int c = 0; c < (int)4; ++c)
			{
				float splitDist = depthSplits[c];

				Vector3 frustumCorners[8] = {
					{ -1.0f,  1.0f, -1.0f },
					{  1.0f,  1.0f, -1.0f },
					{  1.0f, -1.0f, -1.0f },
					{ -1.0f, -1.0f, -1.0f },
					{ -1.0f,  1.0f,  1.0f },
					{  1.0f,  1.0f,  1.0f },
					{  1.0f, -1.0f,  1.0f },
					{ -1.0f, -1.0f,  1.0f },
				};

				// Transform frustum corners from clip space to world space
				for (Vector3& frustumCorner : frustumCorners)
				{
					Vector4 invCorner = invCam * Vector4(frustumCorner, 1.0f);
					frustumCorner = invCorner / invCorner.w;
				}

				for (int i = 0; i < 4; ++i)
				{
					Vector3 dist = frustumCorners[i + 4] - frustumCorners[i];
					frustumCorners[i + 4] = frustumCorners[i] + (dist * splitDist);
					frustumCorners[i] = frustumCorners[i] + (dist * lastSplitDist);
				}

				Vector3 frustumCenter;
				for (const Vector3& frustumCorner : frustumCorners)
				{
					bounds[c].Expand(frustumCorner);
					frustumCenter += frustumCorner;


					frustumCenter /= 8.0f;

					float radius = 0.0f;
					for (const Vector3& frustumCorner : frustumCorners)
					{
						float distance = Vector3::Length(frustumCorner - frustumCenter);
						radius = MAX(radius, distance);
					}
					radius = std::ceil(radius * 16.0f) / 16.0f;

					Vector3 maxExtents = Vector3(radius, radius, radius);
					Vector3 minExtents = -maxExtents;
					float zFar = sceneCamera->GetCameraSettings().zfar;
				
					float pitch = Math::ACos(Vector2::Length(Vector2(LightDir.x, LightDir.z)));
					float yaw = Math::Rad2deg((Math::ATan(LightDir.x / LightDir.z)));
					float yawRad = Math::Deg2Rad(yaw);

					Matrix4x4 View = Matrix4x4::Identity;
					View = Matrix4x4::Rotate(View, Vector3(1, 0, 0), pitch);

					yaw = LightDir.z > 0 ? yaw - 180 : yaw;
					View = Matrix4x4::Rotate(View, Vector3(0, 1, 0), -yawRad);
					View = Matrix4x4::Translate(View, -frustumCenter);
					Proj = Matrix4x4::CreateOrtho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, -(maxExtents.z - minExtents.z), maxExtents.z - minExtents.z);

					TextureMatrix[c] = Proj * View;

					lastSplitDist = depthSplits[c];
				}
			}
		}
	};


	/* Lights */
	uint32_t lightCount;
	std::array<std::unique_ptr<Light>, 10> m_lights;
	EnviromentLight m_EnviromentLight;

	/* Render-ables */
	std::vector<std::shared_ptr<Actor>> m_actors;
	std::vector<Mesh*> culledMeshes;
	std::vector<Mesh*> meshes;
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

	const Vector4& GetDepthSplits() { return shadowData->splitDistances; }

	void AddActor(std::shared_ptr<class Actor>& p_actor);

	Scene(const std::string& p_name);
	~Scene();
};