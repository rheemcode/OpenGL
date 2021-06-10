#include "Camera.h"

Camera::Camera(const CameraMode& mode)
{
	m_ProjectionMatrix = MakeProjectionMatrix(mode);
}

Camera::Camera(const CameraSettings& setting)
{
	m_ProjectionMatrix = MakeProjectionMatrix(setting);
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
			break;
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
	m_ViewMatrix = glm::translate(m_ViewMatrix, position);
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