#include <glpch.h>
#include "GLSandbox.h"

#include "GLApplication/GLCore.h"
#include "GLib.h"
#include "ECS/SceneCamera.h"

void GLSandboxApplication::OnStart()
{
	auto* scene = CreateScene("GLSandboxApplication");                     
	applicationName = std::string("GLSandboxApplication");

	Display* display = Display::GetSingleton();
	CameraSettings cameraSettings;
	cameraSettings.mode = CameraMode::PERSPECTIVE;
	cameraSettings.fovY = 70.f;
	cameraSettings.znear = 0.01f;
	cameraSettings.zfar = 1000.f;
	cameraSettings.winWidth = display->GetMainWindow()->GetWidth();
	cameraSettings.winHeight = display->GetMainWindow()->GetHeight();
	cameraSettings.ratio = cameraSettings.winWidth / cameraSettings.winHeight;
	std::shared_ptr<Camera> camera = std::make_shared<SceneCamera>(cameraSettings);
	std::shared_ptr<SceneCameraController> controller = std::make_shared<SceneCameraController>();
	camera->AttachController(controller);
	scene->SetMainCamera(camera);
	AttachScene(scene);									
	SetMainScene("GLSandboxApplication");

	ERR_FAIL_COND_MSG(mainScene == nullptr, "Please Attach A Main Scene")
	mainScene->CreateDefaultActor();
}