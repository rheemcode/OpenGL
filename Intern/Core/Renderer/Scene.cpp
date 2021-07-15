#include "Scene.h"
#include <functional>
#include <Window/Window.h>
#include "Events/MouseEvent.h"
#include "Actor.h"
#include "Components/TransformComponent.h"
#include "Components/MeshRendererComponent.h"

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

void Scene::OnUpdate()
{	
	sceneCamera->OnUpdate();

	Renderer::BeginScene(*sceneCamera);

	for (auto& actor : m_actors)
	{
		if (auto cmp = actor->GetComponent("Renderer Component").lock())
		{
			const MeshRendererComponent& meshRenderer = *(MeshRendererComponent*)cmp.get();
			Renderer::Render(meshRenderer);
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

//void Scene::InitLightUniforms()
//{
//	auto program = sceneShader->GetProgram();
//
//	lightsBufferBinding = glGetUniformBlockIndex(program, "U_DirectionalLight");
//
//	glGetActiveUniformBlockiv(program, lightsBufferBinding, GL_UNIFORM_BLOCK_DATA_SIZE, &lightsBufferSize);
//	lightsBufferData = new LightUniformBuffer();
//	auto si = sizeof(LightUniformBuffer);
//	const char* names[4] = {  "U_DirectionalLight.LightColor", "U_DirectionalLight.Direction", "U_DirectionalLight.Energy", "U_DirectionalLight.use" };
//	glGetUniformIndices(program, 4, names, indices);
//
//	glGetActiveUniformsiv(program, 4, indices, GL_UNIFORM_OFFSET, offset);
//	glGetActiveUniformsiv(program, 4, indices, GL_UNIFORM_SIZE, size);
//	glGetActiveUniformsiv(program, 4, indices, GL_UNIFORM_TYPE, type);
//	
//	m_LightsBuffer = std::make_unique<UniformBuffer>(lightsBufferSize, lightsBufferBinding);
//	delete names;
//}

Scene::Scene()
{

	Display* display = Display::GetSingleton();
	display->m_Windows[0]->BindEventCallback(std::bind(&Scene::OnEvent, this, std::placeholders::_1));

	CameraSettings cameraSettings;

	cameraSettings.mode = CameraMode::PERSPECTIVE;
	cameraSettings.fovY = 95.f;
	cameraSettings.winWidth = 1200;
	cameraSettings.winHeight = 700;
	cameraSettings.ratio = 1.f;


	sceneCamera = std::make_unique<SceneCamera>(cameraSettings);
	sceneShader = std::make_unique<Shader>("Intern/Shaders/lighting.shader");
	sceneShader->Bind();
	
//	InitLightUniforms();

	std::unique_ptr<Primitive> testCube = std::make_unique<Cube>();
	std::unique_ptr<Primitive> testSphere = std::make_unique<Sphere>(18, 36, 1.2f);
	std::unique_ptr<Primitive> testPlane = std::make_unique<Plane>();

	m_EnviromentLight.Ambient = { 1.f, 1.f, 1.f};
	m_EnviromentLight.Energy = .19f;
	
	auto pLight = std::make_unique<PointLight>();
	pLight->LightColor = { .7f, .7f, .7f};
	pLight->Energy = 1.5f;
	pLight->Direction = { 0, -1.f, -.76f };
	pLight->LightSource = Light::POINT_LIGHT;
	pLight->Position = { 0, .7f, -1.f };
	pLight->Radius = 4.f;
	pLight->LightAttenuation = { 1.f, 1.f };
//	pLight->innerCutoff = Math::Cos(Math::Deg2Rad(30.f));
//	pLight->outerCutoff = Math::Cos(Math::Deg2Rad(35.f));;
	pLight->Use = true;
	m_lights[0] = std::move(pLight);

	std::shared_ptr<Actor> actor = std::make_shared<Actor>();
	std::shared_ptr<TransformComponent> tComponent = std::make_shared<TransformComponent>(actor);
	std::shared_ptr<MeshRendererComponent> renderComponent = std::make_shared<MeshRendererComponent>(actor, "./Madara_Uchiha.obj");
	actor->AddComponent(tComponent);
	actor->AddComponent(renderComponent);

	AddActor(actor);
	AddObject(testCube);
	AddObject(testPlane);

}
