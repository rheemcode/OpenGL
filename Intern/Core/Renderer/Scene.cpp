#include "Scene.h"
#include "Actor.h"
#include <functional>
#include <Window/Window.h>
#include "Events/MouseEvent.h"
#include "Components/TransformComponent.h"
#include "Components/MeshRendererComponent.h"

#include <thread>


Scene::EnviromentLight Scene::m_EnviromentLight;
std::unique_ptr<Shader> Scene::sceneShader;
std::array<std::unique_ptr<Light>, 10> Scene::m_lights;
uint32_t Scene::lightCount;


const Scene::EnviromentLight& Scene::GetEnviromentLight()
{
	return m_EnviromentLight;
}

const std::array<std::unique_ptr<Light>, 10>& Scene::GetLight()
{
	return m_lights;
}

int Scene::GetLightCount()
{
	return 1;
}

using namespace std::literals::chrono_literals;
void Scene::Process()
{
	while (s_active)
	{
		sceneCamera->OnUpdate();
		std::this_thread::sleep_for(2ms);
	}
}

void Scene::OnUpdate()
{	
	//Timer timer;


	Renderer::BeginScene(*sceneCamera);

	for (auto& actor : m_actors)
	{
		if (const std::shared_ptr<Component> cmp = actor->GetComponent("Renderer Component").lock())
		{
			auto c = std::dynamic_pointer_cast<MeshRendererComponent, Component>(cmp);
			auto meshRenderer = std::dynamic_pointer_cast<MeshRendererComponent, Component>(cmp);

			Renderer::Render(meshRenderer, sceneCamera->GetFrustum());
			int* p = new int(1);
			
		}
	}

	//for (auto &object : m_Primitives)
	//{
		//object->OnUpdate();
	//	Renderer::Render(object);
	//}

	Renderer::EndScene();
}


void Scene::AddObject(std::unique_ptr<Primitive>& primitive)
{
	m_Primitives.push_back(std::move(primitive));
}

void Scene::AddActor(std::shared_ptr<Actor>& p_actor)
{
	m_actors.push_back(p_actor);
}



void Scene::OnEvent(const Event& event)
{
	sceneCamera->OnEvent(event);

	if (event.GetEventType() == EventType::KeyEvent)
	{
		std::stringstream ss;
		const auto& ke = (KeyEvent&)event;
	}


}

void Scene::Shutdown()
{
	s_active = false;
}

Scene::Scene()
{
	s_active = true;
	Display* display = Display::GetSingleton();
	display->m_Windows[0]->BindEventCallback(std::bind(&Scene::OnEvent, this, std::placeholders::_1));

	CameraSettings cameraSettings;

	cameraSettings.mode = CameraMode::PERSPECTIVE;
	cameraSettings.fovY = 95.f;
	cameraSettings.winWidth = 1200;
	cameraSettings.winHeight = 700;
	cameraSettings.ratio = cameraSettings.winWidth / cameraSettings.winHeight;


	sceneCamera = std::make_unique<SceneCamera>(cameraSettings);
	sceneShader = std::make_unique<Shader>("Intern/Shaders/lighting.shader");
	sceneShader->Bind();
	
//	InitLightUniforms();

	std::unique_ptr<Primitive> testCube = std::make_unique<Cube>();
	std::unique_ptr<Primitive> testSphere = std::make_unique<Sphere>(18, 36, 1.2f);
	//std::unique_ptr<Primitive> testPlane = std::make_unique<Plane>();

	m_EnviromentLight.Ambient = { 1.f, 1.f, 1.f};
	m_EnviromentLight.Energy = .19f;
	
	auto pLight = std::make_unique<DirectionalLight>();
	pLight->LightColor = { 1.0f, 1.f, 1.0f};
	pLight->Energy = 1.5f;
	pLight->Direction = { 0, -.1f, -0.3f };
	pLight->LightSource = Light::DIRECTIONAL_LIGHT;
	pLight->Position = { 0, .7f, -1.f };
//	pLight->Radius = 4.f;
	pLight->LightAttenuation = { 1.f, 1.f };
//	pLight->innerCutoff = Math::Cos(Math::Deg2Rad(30.f));
//	pLight->outerCutoff = Math::Cos(Math::Deg2Rad(35.f));;
	pLight->Use = true;
	m_lights[0] = std::move(pLight);

	std::shared_ptr<Actor> actor = std::make_shared<Actor>();
	std::shared_ptr<TransformComponent> tComponent = std::make_shared<TransformComponent>(actor);
	std::shared_ptr<MeshRendererComponent> renderComponent = std::make_shared<MeshRendererComponent>(actor, "./house.obj");
	actor->AddComponent(tComponent);
	actor->AddComponent(renderComponent);

	AddActor(actor);
	AddObject(testCube);
	//AddObject(testPlane);
}
