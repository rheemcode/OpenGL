#include <glpch.h> 
#include "Scene.h"
#include "Actor.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/Renderer.h"
#include "Buffers/FrameBuffer.h"
#include "Buffers/UniformBuffer.h"
#include "Effects/SSAO.h"
#include "Renderer/SkyBox.h"
#include "OpenGL/Renderer.h"
#include "Buffers/FrameBuffer.h"
#include "Buffers/GBuffer.h"
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

void Scene::PrepareMeshes()
{
	culledMeshes.clear();
	if (meshDirty)
		meshes.clear();
	const auto& frustum = sceneCamera->GetFrustum();
	for (auto& actor : m_actors)
	{

		if (const Component* cmp = actor->GetComponent("Renderer Component"))
		{
			const auto meshRenderer = (MeshRendererComponent*) cmp;
			meshRenderer->UpdateTransform();
			for (const auto& mesh : meshRenderer->GetMeshes())
			{
				if (meshDirty)
				{
					meshes.push_back(mesh);
				}
				if (mesh.GetInstanceBound().InFrustum(frustum))
				{
					culledMeshes.push_back(mesh);
				}
			}
		}
	}

	meshDirty = false;
}

void Scene::Render()
{
	PrepareMeshes();
	shadowData->shadowBounds.Update();
	shadowData->UpdateView(GetSkyLightDirection());
	shadowData->UpdateProjection();
	
	m_MatrixBuffer->UploadData(sceneCamera->GetViewMatrix(), 0);
	m_MatrixBuffer->UploadData(sceneCamera->GetProjectionMatrix(), 64);
	m_MatrixBuffer->UploadData(shadowData->Bias * shadowData->Proj[0] * shadowData->View[0], 128 + 64);
	m_MatrixBuffer->UploadData(shadowData->Bias * shadowData->Proj[1] * shadowData->View[1], 256);
	m_MatrixBuffer->UploadData(shadowData->Bias * shadowData->Proj[2] * shadowData->View[2], 320);
	m_MatrixBuffer->UploadData(shadowData->Bias * shadowData->Proj[3] * shadowData->View[3], 384);
	m_MatrixBuffer->FlushBuffer();


	Mesh* _culledMeshes = culledMeshes.data();
	Mesh* _meshes = meshes.data();

	{
		// Depth Pass
		RenderPass depthPass;
		depthPass.Pass = RenderPass::DEPTH_PASS;
		auto& renderData = depthPass.renderData;
		renderData.cameraData = cameraData;
		renderData.meshes = _meshes;
		renderData.meshCount = meshes.size();
		renderData.shader = shadowShader;
		renderData.shadowData = shadowData;
		renderData.framebuffer = m_shadowBuffer;
		Renderer::PushPass(std::move(depthPass));
	}

	{
		// Color Pass
		RenderPass colorPass;
		colorPass.Pass = RenderPass::COLOR_PASS;
		auto& renderData = colorPass.renderData;
		renderData.cameraData = cameraData;
		renderData.meshes = _culledMeshes;
		renderData.meshCount = culledMeshes.size();
		renderData.shader = sceneShader;
		renderData.shadowData = shadowData;
		renderData.framebuffer = m_shadowBuffer;
		renderData.uniformBuffer = m_MatrixBuffer;
	//	Renderer::PushPass(std::move(colorPass));
	}

	{
		// Skybox Pass
		//RenderPass skyboxPass;
		//skyboxPass.Pass = RenderPass::SKYBOX;
		//auto& renderData = skyboxPass.renderData;
		//renderData.cameraData = cameraData;
	//	Renderer::PushPass(std::move(skyboxPass));
	}

	{
		// GBuffer Pass
		RenderPass gBufferPass;
		gBufferPass.Pass = RenderPass::GBUFFER_PASS;
		auto& renderData = gBufferPass.renderData;
		renderData.cameraData = cameraData;
		renderData.meshes = _culledMeshes;
		renderData.meshCount = culledMeshes.size();
		renderData.shader = sceneShader;
		renderData.shadowData = shadowData;
		renderData.framebuffer = m_shadowBuffer;
		renderData.uniformBuffer = m_MatrixBuffer;
		renderData.gBuffer = m_Gbuffer;
		Renderer::PushPass(std::move(gBufferPass));
	}

	{
		// SSAO Pass
		RenderPass ssaoPass;
		ssaoPass.Pass = RenderPass::POSTPROCESS_PASS;
		auto& renderData = ssaoPass.renderData;
		renderData.cameraData = cameraData;
		renderData.meshes = _culledMeshes;
		renderData.meshCount = culledMeshes.size();
		renderData.gBuffer = m_Gbuffer;
		renderData.postProcessEffect = m_ssaoEffect;
		Renderer::PushPass(std::move(ssaoPass));
	}

	{
		// Deffered Pass
		RenderPass colorPass;
		colorPass.Pass = RenderPass::DEFFERED_PASS;
		auto& renderData = colorPass.renderData;
		renderData.cameraData = cameraData;
		renderData.meshes = _culledMeshes;
		renderData.meshCount = culledMeshes.size();
		renderData.shader = sceneShader;
		renderData.shadowData = shadowData;
		renderData.framebuffer = m_shadowBuffer;
		renderData.uniformBuffer = m_MatrixBuffer;
		renderData.gBuffer = m_Gbuffer;
		renderData.postProcessEffect = m_ssaoEffect;
		Renderer::PushPass(std::move(colorPass));
	}

	{
		//// AABB Pass
		//RenderPass colorPass;
		//colorPass.Pass = RenderPass::AABB;
		//auto& renderData = colorPass.renderData;
		//renderData.cameraData = cameraData;
		//renderData.meshes = culledMeshes;
		//renderData.shader = aabbShader;
		//renderData.vertexArray = aabbVertexArray;
		//renderData.vertexBuffer = aabbVertexBuffer;
	//	Renderer::PushPass(std::move(colorPass));
	}

	Renderer::FlushRenderQueue();
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

void Scene::InitLightBuffer()
{

	// retrieves the index not binding
	m_LightsBuffer->InitData(uniformsBufferShader.get(), "LightsUniform");
	
	LightData lightData;
	lightData.LightType = 1;
	lightData.Ambient = Vector4(.4f, .4f, .4f, 1.f);
	lightData.Color = {1.f, 1.f, 1.f, 1.f};
	lightData.Direction = { 0, -0.3f, -1.f, 1.f };
	lightData.AmbientEnergy = 1.f;
	lightData.Energy = .69f;

	m_LightsBuffer->UploadData(lightData.LightType, 0);
	m_LightsBuffer->UploadData(lightData.AmbientEnergy, 4);
	m_LightsBuffer->UploadData(lightData.Energy, 8);
	m_LightsBuffer->UploadData(lightData.Ambient, 16);
	m_LightsBuffer->UploadData(lightData.Color, 32);
	m_LightsBuffer->UploadData(lightData.Direction, 48);
	m_LightsBuffer->FlushBuffer();
}

void Scene::CreateActor()
{

}

void Scene::CreateDefaultActor()
{
	std::shared_ptr<Actor> actor = std::make_shared<Actor>();
	std::shared_ptr<TransformComponent> tComponent = std::make_shared<TransformComponent>(actor);
	std::shared_ptr<MeshRendererComponent> renderComponent = std::make_shared<MeshRendererComponent>(actor, "./Assets/Madara_Uchiha.obj");
	actor->AddComponent(tComponent);
	actor->AddComponent(renderComponent);
	meshDirty = true;
	AddActor(actor);
}

void Scene::InitSceneCamera()
{

	if (sceneCamera == nullptr)
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
	}
	cameraData = std::make_shared<CameraData>();
	cameraData->proj.reset(&sceneCamera->m_ProjectionMatrix);
	cameraData->view.reset(&sceneCamera->m_ViewMatrix);
	shadowData = std::make_shared<ShadowData>();
	shadowData->shadowBounds = ShadowBox(shadowData->View, sceneCamera->transform, sceneCamera->m_cameraSettings);
	auto Bias = Matrix4x4::CreateTranslation({ 0.5f, 0.5f, 0.5f });
	Bias = Matrix4x4::Scale(Bias, { 0.5f, 0.5f, 0.5f });
	shadowData->Bias = Bias;
}

void Scene::CreateSkyLight()
{

	//sceneShader->Bind();

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
	InitLightBuffer();
}


void Scene::CreateBuffers()
{
	m_Gbuffer = std::make_shared<GBuffer>();
	m_shadowBuffer = std::make_shared<Framebuffer>();
	m_shadowBuffer->CreateTexture(FramebufferTexture::SHADOWMAPARRAY);
	m_shadowBuffer->AttachArrayTexture(TEXTURE_MAX_SIZE / 2, TEXTURE_MAX_SIZE / 2, 4);
	shadowData->ShadowSize = Vector2((float)TEXTURE_MAX_SIZE /2, (float)TEXTURE_MAX_SIZE /2);

	m_LightsBuffer = std::make_shared<UniformBuffer>();
	m_MatrixBuffer = std::make_shared<UniformBuffer>();
	m_ssaoSamplesBuffer = std::make_shared<UniformBuffer>();
	m_MatrixBuffer->InitData(uniformsBufferShader.get(), "Matrices");
	m_ssaoSamplesBuffer->InitData(uniformsBufferShader.get(), "SSAOSamples");

	static uint32_t indices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 4, 5, 5, 1, 5, 6, 2, 6, 6, 7, 7, 3, 7, 4 };
	aabbVertexArray = std::make_shared<VertexArray>();
	aabbVertexBuffer = std::make_shared<VertexBuffer>(sizeof(Vector3) * 24);
	aabbVertexBuffer->SetLayout({ { AttribDataType::T_FLOAT, Attrib::VERTEXPOSITION, AttribCount::VEC3, false } });
	aabbVertexArray->SetIndices(indices, 24);
	aabbVertexArray->AddBuffer(*aabbVertexBuffer.get());
	m_ssaoEffect = std::make_shared<SSAO>();
	m_ssaoEffect->m_uniformBuffer = m_ssaoSamplesBuffer;
	m_ssaoEffect->CreateFramebuffer();
	m_ssaoEffect->GenerateKernel();
	m_ssaoEffect->GenerateNoiseTexture();
}


void Scene::InitRenderer()
{
	Renderer::Init();
	/*Renderer2D renderer2D;
	renderer2D.Init();*/
}

void Scene::InitSceneShaders()
{
	uniformsBufferShader = std::make_shared<Shader>("./Assets/Shaders/uniform_buffers.glsl");
	sceneShader = std::make_shared<Shader>("Assets/Shaders/lighting.shader");
	shadowShader = std::make_shared<Shader>("Assets/Shaders/depth.glsl");
	aabbShader = std::make_shared<Shader>("./Assets/Shaders/aabb.glsl");
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