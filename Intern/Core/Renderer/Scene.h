#pragma once
#include <vector>
#include "Tests/Object.h"
#include <Tests/Primitive.h>
#include "Renderer/Renderer.h"
#include "Math/Transform.h"
#include "Events/Event.h"


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

	static uint32_t lightCount;
	static std::array<std::unique_ptr<Light>, 10> m_lights;
	static EnviromentLight m_EnviromentLight;

	std::vector<std::unique_ptr<Primitive>> m_Primitives;
	std::vector<std::shared_ptr<class Actor>> m_actors;
	std::unique_ptr<UniformBuffer> m_LightsBuffer;
	std::unique_ptr<SceneCamera> sceneCamera;
	static std::unique_ptr<Shader> sceneShader;
	

	friend class Renderer;
	static const EnviromentLight& GetEnviromentLight();
	static const std::array<std::unique_ptr<Light>, 10>& GetLight();
	static int GetLightCount();

public:

	void OnUpdate();
	void AddObject(std::unique_ptr<Primitive>& primitive);
	void AddActor(std::shared_ptr<class Actor>& p_actor);
	void OnEvent(const Event& event);

//	void InitLightUniforms();
	
	Scene();
	~Scene() {}
};

