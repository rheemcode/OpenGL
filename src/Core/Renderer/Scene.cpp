#include "Scene.h"
#include <functional>
#include <Window/Window.h>
#include "Events/MouseEvent.h"


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
static float t = 0;
int Scene::GetLightCount()
{
	return 1;
}
void Scene::OnUpdate()
{

	t += 0.0001;
	if (t > 100)
	{
		t = 0;
	}

	
	sceneCamera->OnUpdate();

	Renderer::BeginScene(*sceneCamera);
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
	sceneShader = std::make_unique<Shader>("src/Shaders/lighting.shader");
	sceneShader->Bind();
	
//	InitLightUniforms();

	std::unique_ptr<Primitive> testCube = std::make_unique<Cube>();
	std::unique_ptr<Primitive> testSphere = std::make_unique<Sphere>(18, 36, 1.2f);
	std::unique_ptr<Primitive> testPlane = std::make_unique<Plane>();

	m_EnviromentLight.Ambient = { 1.f, 1.f, 1.f};
	m_EnviromentLight.Energy = .19f;
	
	auto pLight = std::make_unique<SpotLight>();
	pLight->LightColor = { .7f, .7f, .7f};
	pLight->Energy = 1.5f;
	pLight->Direction = { 0, -1.f, -.36f };
	pLight->LightSource = Light::SPOT_LIGHT;
	pLight->Position = { 0, 3.f, 0.f };
	pLight->Radius = 5.2f;
	pLight->LightAttenuation = { 1.004f, 1.002f };
	pLight->innerCutoff = Math::Cos(Math::Deg2Rad(45.f));
	pLight->outerCutoff = Math::Cos(Math::Deg2Rad(45.f));;
	pLight->Use = true;
	m_lights[0] = std::move(pLight);

	AddObject(testCube);
	AddObject(testSphere);
	AddObject(testPlane);

}
