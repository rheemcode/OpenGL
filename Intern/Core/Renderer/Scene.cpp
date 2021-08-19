#include <functional> 
#include "Scene.h"
#include "Actor.h"
#include "Window/Window.h"
#include "Events/MouseEvent.h"
#include "Components/TransformComponent.h"
#include "Components/MeshRendererComponent.h"
#include "Components/StaticMeshRendererComponent.h"
#include "Timestep.h"


Scene* Scene::s_instance = nullptr;

Scene::EnviromentLight Scene::m_EnviromentLight;

std::unique_ptr<Shader> Scene::sceneShader;
std::unique_ptr<Shader> Scene::shadowShader;

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
	m_shadowBox.UpdateBounds();
	Renderer::BeginScene(GetSceneCamera());
	Renderer::Render(culledMeshes);
}

void Scene::ThreadRunLoop(float p_delta)
{
	//Renderer::SetClearColor(.4f, .4f, .4f, 1);
	Renderer::Clear();
	drawPending += 2;
	ThreadUpdate(p_delta);
	ThreadRender();
	Renderer::EndScene();
	Display::GetSingleton()->SwapBuffer();
}

void Scene::RunLoop(float p_delta)
{
	commandQueue.Push(this, &Scene::ThreadRunLoop, p_delta);
}

std::vector<Mesh>& Scene::GetCulledMeshes()
{
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
	Renderer::ShutDown();
}

void Scene::ThreadFlush()
{
	drawPending-=2;
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
	_Update(p_delta);

	//drawPending++;
	//commandQueue.Push(this, &Scene::ThreadUpdate, p_delta);
	
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
	commandQueue.Push(this, &Scene::ThreadFlush);
	commandQueue.Push(this, &Scene::ThreadExit);
	thread.WaitToFinish();
}

Scene* Scene::GetSingleton()
{
	return s_instance;
}

struct LightData
{
	int LightType;
	Vector4 Ambient;
	Vector4 Color;
	//   vec4 Position;
	Vector4 Direction;
	float AmbientEnergy;
	float Energy;
};

void Scene::InitLightUniforms()
{
	const uint32_t NumUniforms = 6;
	GLuint indices[NumUniforms];
	GLint offset[NumUniforms];

	const char* names[NumUniforms] = {
	"Lights.LightType",
	 "Lights.Ambient",
	 "Lights.Color",
	 "Lights.Direction",
	 "Lights.AmbientEnergy",
	 "Lights.Energy",
	};
	uint32_t uboIndex = glGetUniformBlockIndex(sceneShader->GetProgram(), "LightsUniform");
	int32_t uboSize = sizeof(GLboolean);;
	m_LightsBuffer = std::make_unique<UniformBuffer>(80, uboIndex);
	glGetActiveUniformBlockiv(sceneShader->GetProgram(), uboIndex,
		GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);
	
	glGetUniformIndices(sceneShader->GetProgram(), NumUniforms, names, indices);
	glGetActiveUniformsiv(sceneShader->GetProgram(), NumUniforms, indices, GL_UNIFORM_OFFSET, offset);

	LightData lightData;
	lightData.LightType = 1;
	lightData.Ambient = Vector4(.4f, .4f, .4f, 1.f);
	lightData.Color = {1.f, 1.f, 1.f, 1.f};
	lightData.Direction = { 0, -0.3f, -1.f, 1.f };
	lightData.AmbientEnergy = 1.f;
	lightData.Energy = .69;
	if (1 and 2)
		auto i = 2;
	auto* buffer = malloc(uboSize);
	memcpy((char*)buffer + offset[0], &lightData.LightType, 4);
	memcpy((char*)buffer + offset[1], &lightData.Ambient, 16);
	memcpy((char*)buffer + offset[2], &lightData.Color, 16);
	memcpy((char*)buffer + offset[3], &lightData.Direction, 16);
	memcpy((char*)buffer + offset[4], &lightData.AmbientEnergy, 4);
	memcpy((char*)buffer + offset[5], &lightData.Energy, 4);
	m_LightsBuffer->SetData(80, buffer, 0);
	free(buffer);
}

void Scene::Init()
{
	if (!s_instance)
	{
		s_instance = new Scene();
	}
}



void Scene::CreateActor()
{

}

void Scene::ThreadCreateDefaultActor()
{
	std::shared_ptr<Actor> actor = std::make_shared<Actor>();
	std::shared_ptr<TransformComponent> tComponent = std::make_shared<TransformComponent>(actor);
	std::shared_ptr<MeshRendererComponent> renderComponent = std::make_shared<MeshRendererComponent>(actor, "./Assets/test2.obj");
	actor->AddComponent(tComponent);
	actor->AddComponent(renderComponent);

	AddActor(actor);
}

void Scene::CreateDefaultActor()
{
	commandQueue.Push(this, &Scene::ThreadCreateDefaultActor);
}

void Scene::InitSceneCamera()
{
	Display* display = Display::GetSingleton();
	CameraSettings cameraSettings;
	cameraSettings.mode = CameraMode::PERSPECTIVE;
	cameraSettings.fovY = 70.f;
	cameraSettings.znear = 0.1f;
	cameraSettings.zfar = 1000.f;
	cameraSettings.winWidth = display->GetMainWindow()->GetWidth();
	cameraSettings.winHeight = display->GetMainWindow()->GetHeight();
	cameraSettings.ratio = cameraSettings.winWidth / cameraSettings.winHeight;


	sceneCamera = std::make_unique<SceneCamera>(cameraSettings);
	m_shadowBox = ShadowBox(Matrix4x4(), sceneCamera->GetTransform(), cameraSettings);
}

void Scene::CreateSkyLight()
{

	sceneShader->Bind();

	m_EnviromentLight.Ambient = { 1.f, 1.f, 1.f };
	m_EnviromentLight.Energy = .19f;

	auto pLight = std::make_unique<DirectionalLight>();
	pLight->LightColor = { 1.0f, 1.f, 1.0f };
	pLight->Energy = 1.5f;
	pLight->Direction = { 0, -.1f, -0.3f };
	pLight->LightSource = Light::DIRECTIONAL_LIGHT;
	pLight->Position = { 0, .7f, -1.f };
	pLight->LightAttenuation = { 1.f, 1.f };

	pLight->Use = true;
	m_lights[0] = std::move(pLight);
	InitLightUniforms();
}

void Scene::ThreadBeginScene()
{
	Display* display = Display::GetSingleton();
	display->GetMainWindow()->Init();
	display->GetMainWindow()->BindEventCallback(std::bind(&Scene::OnEvent, this, std::placeholders::_1));
	display->GetMainWindow()->ReleaseCurrent();
	display->GetMainWindow()->MakeCurrent();

	Renderer::Init();
	/*Renderer2D renderer2D;
	renderer2D.Init();*/

	InitSceneCamera();

	sceneShader = std::make_unique<Shader>("Assets/Shaders/lighting.shader");
	shadowShader = std::make_unique<Shader>("Assets/Shaders/depth.glsl");

	m_shadowBuffer = std::make_unique<FrameBuffer>();
	m_shadowBuffer->CreateTexture();
	m_shadowBuffer->AttachDepthTexture();

	CreateSkyLight();

	SkyBox::Create();
}

void Scene::BeginScene()
{
	commandQueue.Push(this, &Scene::ThreadBeginScene);
}

Scene::Scene()
	: commandQueue(true)
{
	thread.Start(ThreadCallback, this);
}

Scene::~Scene()
{
	//thread.WaitToFinish();
}