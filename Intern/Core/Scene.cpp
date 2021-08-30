#include <glpch.h> 
#include "Scene.h"
#include "Actor.h"
#include "Renderer/Renderer.h"
#include "Buffers/FrameBuffer.h"
#include "Window/Window.h"
#include "Events/MouseEvent.h"
#include "Components/TransformComponent.h"
#include "Components/MeshRendererComponent.h"
#include "Components/StaticMeshRendererComponent.h"
#include "Timestep.h"
#include "SceneCamera.h"


Scene* Scene::s_activeScene = nullptr;
std::shared_ptr<Camera> Scene::sceneCamera;

const Scene::EnviromentLight& Scene::GetEnviromentLight()
{
	return m_EnviromentLight;
}

const std::array<std::unique_ptr<Light>, 10>& Scene::GetLight()
{
	return m_lights;
}

using namespace std::literals::chrono_literals;

void Scene::Render()
{
	Renderer::Clear();
	Renderer::BeginScene(GetSceneCamera());
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
#ifdef RENDER_TEST
    //Renderer::RenderTest(culledMeshes);
	//test->RenderTest(p_delta, 0);
#endif
	Renderer::Render(culledMeshes);
	Renderer::EndScene();
}


const std::vector<Mesh>& Scene::GetCulledMeshes()
{
	return culledMeshes;
}

void Scene::OnUpdate(float p_delta)
{
	sceneCamera->OnUpdate(p_delta);
	for (auto& actor : m_actors)
	{
		actor->UpdateComponents(p_delta);
		actor->OnUpdate(p_delta);
	}
}


void Scene::ThreadCallback(void* p_instance)
{
	Scene* scene = reinterpret_cast<Scene*>(p_instance);

//	scene->ThreadLoop();
}

static float last = 0;

void Scene::AddActor(std::shared_ptr<Actor>& p_actor)
{
	m_actors.push_back(p_actor);
}


void Scene::OnEvent(const Event& event)
{
	sceneCamera->OnEvent(event);

}

Scene* Scene::GetActiveScene()
{
	return s_activeScene;
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

void Scene::BindFBO(FrameBufferName::Type name)
{
	m_shadowBuffer->Bind(name); 
}

void Scene::BindFBOTex(FrameBufferTexture::Type name) 
{
	m_shadowBuffer->BindTexture(name); 
}

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

void Scene::CreateActor()
{

}

void Scene::CreateDefaultActor()
{
	std::shared_ptr<Actor> actor = std::make_shared<Actor>();
	std::shared_ptr<TransformComponent> tComponent = std::make_shared<TransformComponent>(actor);
	std::shared_ptr<MeshRendererComponent> renderComponent = std::make_shared<MeshRendererComponent>(actor, "./Assets/house.obj");
	actor->AddComponent(tComponent);
	actor->AddComponent(renderComponent);

	AddActor(actor);
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
	if (sceneCamera == nullptr)
	{
		sceneCamera = std::make_unique<SceneCamera>(cameraSettings);
	}
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


void Scene::CreateBuffers()
{

	m_shadowBuffer = std::make_unique<FrameBuffer>();
	m_shadowBuffer->CreateTexture();
	m_shadowBuffer->AttachDepthTexture();
}


void Scene::InitRenderer()
{

	Renderer::Init();
	/*Renderer2D renderer2D;
	renderer2D.Init();*/
}

void Scene::InitSceneShaders()
{
	sceneShader = std::make_unique<Shader>("Assets/Shaders/lighting.shader");
	shadowShader = std::make_unique<Shader>("Assets/Shaders/depth.glsl");
	//testShader = std::make_unique<Shader>("Assets/Shaders/envmap.glsl");
}

void Scene::InitScene()
{
	InitRenderer();
	InitSceneCamera();
	InitSceneShaders();
	CreateBuffers();
	CreateSkyLight();
	CreateTests();
	SkyBox::Create();
}

void Scene::CreateTests()
{
	/*test = new ShaderTest();
	test->InitTest();
	test->CreateShader("./Assets/Shaders/helloWorldTest.glsl", 0);*/
}


Scene::Scene(const std::string& p_sceneName)
	: commandQueue(true), sceneName(p_sceneName)
{
	thread.Start(ThreadCallback, this);
}

Scene::~Scene()
{
	//thread.WaitToFinish();
}