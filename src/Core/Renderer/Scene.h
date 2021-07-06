#pragma once
#include <vector>
#include "Tests/Object.h"
#include <Tests/Primitive.h>
#include <Renderer/Renderer.h>
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

	Matrix4x4 transform;
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
		Vector4 ambientColor;
		Vector4 lightColor;
		Vector3 lightPos;
		float ambientStrength;
	};


	Light m_Light;
	static EnviromentLight m_EnviromentLight;

	std::vector<std::unique_ptr<Primitive>> m_Primitives;
	std::unique_ptr<Camera> sceneCamera;
	

	friend class Renderer;
	static const EnviromentLight& GetEnviromentLight();
public:

	void OnUpdate();
	//void AddObject(Primitive&& primitive);
	void AddObject(std::unique_ptr<Primitive>& primitive);
	void OnEvent(const Event& event);
	
	Scene();
	~Scene() {}
};

