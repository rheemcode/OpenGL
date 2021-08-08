#include <functional> 
#include "Scene.h"
#include "Actor.h"
#include "Window/Window.h"
#include "Events/MouseEvent.h"
#include "Components/TransformComponent.h"
#include "Components/MeshRendererComponent.h"
#include "Timestep.h"


Scene* Scene::s_instance = nullptr;

Scene::EnviromentLight Scene::m_EnviromentLight;
std::unique_ptr<Shader> Scene::sceneShader;
std::array<std::unique_ptr<Light>, 10> Scene::m_lights;
uint32_t Scene::lightCount;

uint64_t Scene::lastTicks = 0;
uint32_t Scene::frames = 0;
uint32_t Scene::frame = 0;

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

void Scene::_Render()
{
	THREAD_LOCK	
	culledMeshes.clear();
	for (auto& actor : m_actors)
	{
		if (const std::shared_ptr<Component> cmp = actor->GetComponent("Renderer Component").lock())
		{
			auto meshRenderer = std::dynamic_pointer_cast<MeshRendererComponent, Component>(cmp);
			meshRenderer->UpdateTransform();
			for (const auto& mesh : meshRenderer->GetMeshes())
			{
				if (mesh.GetInstanceBound().InFrustum(sceneCamera->GetFrustum()))
				{
					culledMeshes.push_back(mesh);
				}
			}
		}
	}
	THREAD_UNLOCK
}

const std::vector<Mesh>& Scene::GetCulledMeshes()
{
	THREAD_LOCK
	THREAD_UNLOCK
	return culledMeshes;
}

void Scene::_Update(float p_delta)
{
	sceneCamera->OnUpdate(p_delta);
}

void Scene::ThreadLoop()
{
	threadID = Thread::GetCallerID();

	while (running)
	{
		commandQueue.WaitAndFlushOne();
	}

	commandQueue.FlushAll();
}

void Scene::ThreadExit()
{
	running = false;
}

void Scene::ThreadFlush()
{
	drawPending--;
}

void Scene::Sync()
{
	drawPending++;
	commandQueue.PushAndSync(this, &Scene::ThreadFlush);
}

void Scene::Render()
{
	//_Render();
	//
	drawPending++;
	commandQueue.Push(this, &Scene::ThreadRender);
}

void Scene::ThreadRender()
{
	if (drawPending)
		_Render();
}

void Scene::ThreadUpdate(float p_delta)
{
	if (drawPending)
		_Update(p_delta);
}

void Scene::ThreadCallback(void* p_instance)
{
	Scene* scene = reinterpret_cast<Scene*>(p_instance);

	scene->ThreadLoop();
}

static float last = 0;

void Scene::OnUpdate(float p_delta)
{	
	//_Update(p_delta);
	drawPending++;
	commandQueue.Push(this, &Scene::ThreadUpdate, p_delta);
	
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
	running = false;
	commandQueue.Push(this, &Scene::ThreadFlush);
	thread.WaitToFinish();
}

Scene* Scene::GetSingleton()
{
	return s_instance;
}

void Scene::Init()
{
	if (!s_instance)
	{
		s_instance = new Scene();
	}


}

Scene::Scene()
	: commandQueue(true)
{

	Display* display = Display::GetSingleton();
	display->m_Windows[0]->BindEventCallback(std::bind(&Scene::OnEvent, this, std::placeholders::_1));

	CameraSettings cameraSettings;

	cameraSettings.mode = CameraMode::PERSPECTIVE;
	cameraSettings.fovY = 105.f;
	cameraSettings.winWidth = 1200;
	cameraSettings.winHeight = 700;
	cameraSettings.ratio = cameraSettings.winWidth / cameraSettings.winHeight;


	sceneCamera = std::make_unique<SceneCamera>(cameraSettings);
	sceneShader = std::make_unique<Shader>("Assets/Shaders/lighting.shader");
	sceneShader->Bind();
	
	SkyBox::Create();
	//skybox = SkyBox::GetSingleton();
//	InitLightUniforms();

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
	std::shared_ptr<MeshRendererComponent> renderComponent = std::make_shared<MeshRendererComponent>(actor, "./Assets/house.obj");
	actor->AddComponent(tComponent);
	actor->AddComponent(renderComponent);

	AddActor(actor);
	thread.Start(ThreadCallback, this);
	//AddObject(testCube);
	//AddObject(testPlane);
}
