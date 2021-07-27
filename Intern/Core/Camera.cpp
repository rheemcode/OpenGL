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
	m_ProjectionMatrix = MakeProjectionMatrix(setting);
}

Matrix4x4 Camera::MakeProjectionMatrix(const CameraMode& projectionMode)
{
	switch (projectionMode)
	{
		case (CameraMode::ORTHOGRAPHIC):
		{
			break;
		}
		case (CameraMode::PERSPECTIVE):
		{
			return Matrix4x4::CreatePerspective(Math::Deg2Rad(65.f), 1.f, 0.1f, 2000.f);
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
			return Matrix4x4::CreateOrtho(-1 ,1, 1, -1, 1.f, -300.f);
		}
		case (CameraMode::PERSPECTIVE):
		{
			return Matrix4x4::CreatePerspective(Math::Deg2Rad(setting.fovY), setting.ratio, 0.1f, 2000.f);
		}
	}

	return Matrix4x4(1);
}

void SceneCamera::OnEvent(const Event& event)
{
	if (event.GetEventType() == EventType::MouseMoved)
	{
		auto& mm = (MouseMovedEvent&)event;
		cameraController.HandleMouseInput(transform, mm.GetX(), mm.GetY());
	}
}

//
//void Camera::Translate(Vector3 p_position)
//{	
//	glm::eye
//	glm::scale()
//	glm::quat q(rotation.w, rotation.x, rotation.y, rotation.z);
//	glm::mat4 mat = glm::translate(glm::mat4_cast(q), { p_position.x, p_position.y, p_position.z });
//	auto forward = q * glm::vec3( 0, 0, -1 );
//	m_ViewMatrix = Matrix4x4::Translate(Quaternion::ToMatrix4x4(rotation), -p_position);
//	position = p_position;
//	position2 = p_position;
//}
//
//void Camera::Rotate(float angle, Vector3 axis)
//{
//	rotation = Quaternion::Rotate(rotation, angle, axis);
//	m_ViewMatrix =  Quaternion::ToMatrix4x4(rotation) * Matrix4x4::CreateTranslation(-position);
//	eulerAngles = Quaternion::EulerAngles(rotation);
//
//}
//
//void Camera::SetRotation(const Quaternion& quat)
//{
//	rotation = quat;
//	m_ViewMatrix = Quaternion::ToMatrix4x4(rotation) * Matrix4x4::CreateTranslation(-position);
//	eulerAngles = Quaternion::EulerAngles(rotation);
//}

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