#include "glpch.h"
#include "SceneCamera.h"

void SceneCamera::OnEvent(const Event& event)
{
	cameraController->OnEvent(event);
}

void SceneCamera::OnUpdate(float p_delta)
{
	if (cameraController != nullptr)
		cameraController->OnUpdate(p_delta);
	UpdateView();
}
