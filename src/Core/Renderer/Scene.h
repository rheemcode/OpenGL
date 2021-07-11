#pragma once
#include <vector>
#include "Tests/Object.h"
#include <Tests/Primitive.h>
#include <Renderer/Renderer.h>
#include "Math/Transform.h"
#include <Events/Event.h>

struct Light
{
	friend class Scene;
	
	enum LightSource
	{
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT
	};

	LightSource LightSource = LightSource::DIRECTIONAL_LIGHT;
	
	Vector4 LightColor;
	float SpecularStrength;
	float Energy;
	Vector4 Position;
	Vector3 Direction;

	bool Use;

	Matrix4x4 transform;
};


struct PointLight : Light
{
	float Radius;
	float LinearAttenuation;
	float QuadraticAttenuation;
};

struct DirectionalLight : Light
{

};

struct SpotLight : PointLight
{
	float outerCutoff;
	float innerCutoff; // innercutoff;
};

struct LightUniformBuffer
{
	Vector3 Direction;
	Vector4 LightColor;
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
		Vector4 Ambient;
		float Energy;
	};


	static Light m_Light;
	static EnviromentLight m_EnviromentLight;


	uint32_t indices[4];
	int offset[4];
	int size[4];
	int type[4];


	LightUniformBuffer* lightsBufferData;
	LightUniformBuffer* lightsBufferOffsetData;
	int32_t lightsBufferSize;
	int32_t lightsBufferBinding;

	std::vector<std::unique_ptr<Primitive>> m_Primitives;
	std::unique_ptr<UniformBuffer> m_LightsBuffer;
	std::unique_ptr<SceneCamera> sceneCamera;
	static std::unique_ptr<Shader> sceneShader;
	

	friend class Renderer;
	static const EnviromentLight& GetEnviromentLight();
	static const Light& GetLight();

public:

	void OnUpdate();
	void AddObject(std::unique_ptr<Primitive>& primitive);
	void OnEvent(const Event& event);

	void InitLightUniforms();
	
	Scene();
	~Scene() {}
};

