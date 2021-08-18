#include "Camera.h"
#include "Window/Window.h"
#include "Math/Quaternion.h"
#include "Events/MouseEvent.h"

Camera::Camera(const CameraMode& mode)
{
	m_ProjectionMatrix = MakeProjectionMatrix(mode);
}

Camera::Camera(const CameraSettings& setting)
{
	m_cameraSettings = setting;
	m_ProjectionMatrix = MakeProjectionMatrix(setting);
}

Matrix4x4 Camera::MakeProjectionMatrix(const CameraMode& projectionMode)
{
	switch (projectionMode)
	{
		case (CameraMode::ORTHOGRAPHIC):
		{
			return Matrix4x4::CreateOrtho(m_cameraSettings.left, m_cameraSettings.right, m_cameraSettings.top, m_cameraSettings.bottom, m_cameraSettings.znear, m_cameraSettings.zfar);
		}
		case (CameraMode::PERSPECTIVE):
		{
			return Matrix4x4::CreatePerspective(Math::Deg2Rad(m_cameraSettings.fovY), m_cameraSettings.ratio, m_cameraSettings.znear, m_cameraSettings.zfar);
		}
	}

	return Matrix4x4(1);
}

Matrix4x4 Camera::MakeProjectionMatrix(const CameraSettings& setting)
{
	switch (setting.mode)
	{
		case (CameraMode::ORTHOGRAPHIC):
		{
			return Matrix4x4::CreateOrtho(m_cameraSettings.left, m_cameraSettings.right, m_cameraSettings.top, m_cameraSettings.bottom, m_cameraSettings.znear, m_cameraSettings.zfar);
		}
		case (CameraMode::PERSPECTIVE):
		{
			return Matrix4x4::CreatePerspective(Math::Deg2Rad(m_cameraSettings.fovY), m_cameraSettings.ratio, m_cameraSettings.znear, m_cameraSettings.zfar);
		}
	}

	return Matrix4x4(1);
}

void SceneCamera::OnEvent(const Event& event)
{
	cameraController.OnEvent(event);
}

const Matrix4x4& Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

const Matrix4x4& Camera::GetViewProjectionMatrix() const
{
	return  m_ProjectionMatrix * m_ViewMatrix;
}

const Matrix4x4& Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

Matrix4x4 Camera::MakeViewMatrix()
{
	return Matrix4x4();
}

Camera::Camera()
{

}