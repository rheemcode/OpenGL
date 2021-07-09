#include "Scene.h"
#include <functional>
#include <Window/Window.h>
#include "Events/MouseEvent.h"

Scene::EnviromentLight Scene::m_EnviromentLight;
std::unique_ptr<Shader> Scene::sceneShader;
Light Scene::m_Light;

const Scene::EnviromentLight& Scene::GetEnviromentLight()
{
	return m_EnviromentLight;
}

const Light& Scene::GetLight()
{
	return m_Light;
}

static float t = 0;


void Scene::OnUpdate()
{
	lightsBufferOffsetData = lightsBufferData;

	lightsBufferOffsetData->Direction = m_Light.Direction;
	lightsBufferOffsetData->LightColor = m_Light.LightColor;
	lightsBufferOffsetData->Energy = m_Light.Energy;
	lightsBufferOffsetData->Use = m_Light.Use;
	

	auto size = sizeof(LightUniformBuffer);
	m_LightsBuffer->SetData(size, lightsBufferData, 0);

	t += 0.0001;
	if (t > 100)
	{
		t = 0;
	}
	Renderer::BeginScene(*sceneCamera);

	m_Light.Position = Vector4(m_Light.Position.x, 10.f * Math::Cos(t * 20.f), m_Light.Position.z, 1.f);
	for (auto &object : m_Primitives)
	{
		object->OnUpdate();
		Renderer::Render(object);
	}

	Renderer::EndScene();
}


void Scene::AddObject(std::unique_ptr<Primitive>& primitive)
{
	m_Primitives.push_back(std::move(primitive));
}

void Scene::OnEvent(const Event& event)
{
	if (event.GetEventType() == EventType::MouseMoved)
	{
		const MouseMovedEvent& mm = (MouseMovedEvent&)event;
		sceneCamera->Rotate(Math::Deg2Rad(mm.GetRelativeX()), { 0, 1, 0 });
		sceneCamera->Rotate(Math::Deg2Rad(mm.GetRelativeY()), { 1, 0, 0 });
	}
}

void Scene::InitLightUniforms()
{
	auto program = sceneShader->GetProgram();
	lightsBufferBinding = glGetUniformBlockIndex(program, "U_DirectionalLight");
	glGetActiveUniformBlockiv(program, lightsBufferBinding, GL_UNIFORM_BLOCK_DATA_SIZE, &lightsBufferSize);
	lightsBufferData = new LightUniformBuffer();
	auto si = sizeof(LightUniformBuffer);
	const char* names[4] = { "U_DirectionalLight.Direction", "U_DirectionalLight.LightColor", "U_DirectionalLight.Energy", "U_DirectionalLight.use" };
	glGetUniformIndices(program, 4, names, indices);

	glGetActiveUniformsiv(program, 4, indices, GL_UNIFORM_OFFSET, offset);
	glGetActiveUniformsiv(program, 4, indices, GL_UNIFORM_SIZE, size);
	glGetActiveUniformsiv(program, 4, indices, GL_UNIFORM_TYPE, type);
	
	m_LightsBuffer = std::make_unique<UniformBuffer>(lightsBufferSize, lightsBufferBinding);
}

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


	sceneCamera = std::make_unique<Camera>(cameraSettings);
	sceneCamera->Translate(Vector3(0.f, 3.f, 2.f));

	sceneShader = std::make_unique<Shader>("src/Shaders/lighting.shader");
	sceneShader->Bind();
	
	InitLightUniforms();

	std::unique_ptr<Primitive> testCube = std::make_unique<Cube>();
	std::unique_ptr<Primitive> testSphere = std::make_unique<Sphere>(18, 36, 1.2f);
	std::unique_ptr<Primitive> testPlane = std::make_unique<Plane>();

	m_EnviromentLight.Ambient = { .8f, .8f, .8f, 1.0f };
	m_EnviromentLight.Energy = .19f;
	DirectionalLight pLight;

	pLight.LightColor = { 1.0f, 1.f, 1.0f, 1.0f };
	pLight.SpecularStrength = 0.f;
	pLight.Energy = 1.5f;
	pLight.Direction = { 0, -0.6f, -1.f };
	pLight.LightSource = Light::DIRECTIONAL_LIGHT;
	m_Light = pLight;

	AddObject(testCube);
	AddObject(testSphere);
	AddObject(testPlane);

}
