#include "Camera.h"
#include "Window/Window.h"

Camera::Camera(const CameraMode& mode)
{
	m_ProjectionMatrix = MakeProjectionMatrix(mode);
}

Camera::Camera(const CameraSettings& setting)
{
	m_ProjectionMatrix = MakeProjectionMatrix(setting);
}

void Camera::Update()
{
	if (Window::isQkey)
	{
		rot.y += .6f;

	}
	if (Window::isEkey)
	{
		rot.y -= .6f;

	}
	if (Window::isWkey)
	{
		pos.x += -glm::cos(glm::radians(rot.y + 90)) * .1f;
		pos.z += -glm::sin(glm::radians(rot.y + 90)) * .1f;
	}
	if (Window::isSkey)
	{
		pos.x += glm::cos(glm::radians(rot.y + 90)) * .1f;
		pos.z += glm::sin(glm::radians(rot.y + 90)) * .1f;
	}
	if (Window::isAkey)
	{	
		pos.x += -glm::cos(glm::radians(rot.y)) * .1f;
		pos.z += -glm::sin(glm::radians(rot.y)) * .1f;
	}
	if (Window::isDkey)
	{
		pos.x += glm::cos(glm::radians(rot.y)) * .1f;
		pos.z += glm::sin(glm::radians(rot.y)) * .1f;
	}
		Translate(pos);
}

glm::mat4x4 Camera::MakeProjectionMatrix(const CameraMode& projectionMode)
{
	switch (projectionMode)
	{
		case (CameraMode::ORTHOGRAPHIC):
		{
			break;
		}
		case (CameraMode::PERSPECTIVE):
		{
			return glm::perspective(glm::radians(65.f), 1.f, 0.1f, 2000.f);
		}
	}

	return glm::mat4x4(1);
}

glm::mat4x4 Camera::MakeProjectionMatrix(const CameraSettings& setting)
{
	switch (setting.mode)
	{
		case (CameraMode::ORTHOGRAPHIC):
		{
			return glm::ortho(-4.f, 4.f, 3.f, -3.f, 3.f, -300.f);
		}
		case (CameraMode::PERSPECTIVE):
		{
			return glm::perspective(glm::radians(setting.fovY), setting.ratio, 0.f, 200.f);
		}
	}

	return glm::mat4x4(1);
}

void Camera::Translate(glm::vec3 position)
{
	glm::mat4x4 mat;
	mat = glm::rotate(mat, glm::radians(rot.y), { 0, 1.f, 0 });
	m_ViewMatrix = glm::translate(mat, -position);
}

void Camera::Rotate(float angle, glm::vec3 direction)
{
	glm::rotate(m_ViewMatrix, angle, direction);
}

glm::mat4x4 Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

glm::mat4x4 Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

glm::mat4x4 Camera::MakeViewMatrix()
{
	return *(glm::mat4x4*)(void*) 0;
}

Camera::Camera()
{

}