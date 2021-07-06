#include "Scene.h"

Scene::EnviromentLight Scene::m_EnviromentLight;

const Scene::EnviromentLight& Scene::GetEnviromentLight()
{
	return m_EnviromentLight;
}

void Scene::OnUpdate()
{
	Renderer::BeginScene(*sceneCamera);
	for (auto &object : m_Primitives)
	{
		object->OnUpdate();
		Renderer::Render(object);
	}

	Renderer::EndScene();
}

//void Scene::AddObject(Primitive&& primitive)
//{
//
//	m_Primitives.push_back(std::make_unique<Primitive>(primitive));
//}

void Scene::AddObject(std::unique_ptr<Primitive>& primitive)
{
	m_Primitives.push_back(std::move(primitive));
}

void Scene::OnEvent(const Event& event)
{
}

Scene::Scene()
{

	CameraSettings cameraSettings;

	cameraSettings.mode = CameraMode::PERSPECTIVE;
	cameraSettings.fovY = 85.f;
	cameraSettings.winWidth = 1200;
	cameraSettings.winHeight = 700;
	cameraSettings.ratio = 1.f;


	sceneCamera = std::make_unique<Camera>(cameraSettings);
	sceneCamera->Translate(Vector3(0, 0, 5.f));
	sceneCamera->Rotate(Math::Deg2Rad(32.f), Vector3(0, 0, 1.f));

	std::unique_ptr<Primitive> testCube = std::make_unique<Cube>();
	std::unique_ptr<Primitive> testSphere = std::make_unique<Sphere>(18, 36, 1.2f);
	m_EnviromentLight.ambientColor = Vector4(.2f, .1f, .3f, 1.0f);
	m_EnviromentLight.lightColor = Vector4(.7563f, .7f, 0.1f, 1.0f);
	m_EnviromentLight.lightPos = sceneCamera->pos;
	//m_EnviromentLight.lightPos.z *= -1;
	////m_EnviromentLight.lightPos.y -= 3.f;
	m_EnviromentLight.ambientStrength = .9f;

	AddObject(testCube);
	AddObject(testSphere);

}
