#include "Camera.h"
#include "Window/Window.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const CameraMode& mode)
{
	m_ProjectionMatrix = MakeProjectionMatrix(mode);
	m_ViewMatrix = Matrix4x4::Translate(m_ViewMatrix, -Vector3(0, 0, -20.f));
}

Camera::Camera(const CameraSettings& setting)
{
	m_ProjectionMatrix = MakeProjectionMatrix(setting);
	pos = Vector3(0, 0, 40.f);
	//m_ViewMatrix = Matrix4x4::Translate(m_ViewMatrix, Vector3(0, 0, -40.f));
}

void Camera::Update()
{/*
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
		pos.x += -Math::Cos(Math::Deg2Rad(rot.y + 90.f)) * .1f;
		pos.z += -Math::Sin(Math::Deg2Rad(rot.y + 90.f)) * .1f;
	}
	if (Window::isSkey)
	{
		pos.x += Math::Cos(Math::Deg2Rad(rot.y + 90.f)) * .1f;
		pos.z += Math::Sin(Math::Deg2Rad(rot.y + 90.f)) * .1f;
	}
	if (Window::isAkey)
	{	
		pos.x += -Math::Cos(Math::Deg2Rad(rot.y)) * .1f;
		pos.z += -Math::Sin(Math::Deg2Rad(rot.y)) * .1f;
	}
	if (Window::isDkey)
	{
		pos.x += Math::Cos(Math::Deg2Rad(rot.y)) * .1f;
		pos.z += Math::Sin(Math::Deg2Rad(rot.y)) * .1f;
	}*/
		Translate(pos);
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
			auto s = glm::perspective(Math::Deg2Rad(65.f), 1.f, 0.1f, 2000.f);
			auto r = Matrix4x4::CreatePerspective(Math::Deg2Rad(65.f), 1.f, 0.1f, 2000.f);
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
			return Matrix4x4::CreateOrtho(-4.f, 4.f, 3.f, -3.f, 3.f, -300.f);
		}
		case (CameraMode::PERSPECTIVE):
		{
			auto s = glm::perspective(Math::Deg2Rad(65.f), 1.f, 0.1f, 2000.f);
			auto r = Matrix4x4::CreatePerspective(Math::Deg2Rad(65.f), 1.f, 0.1f, 2000.f);
			return Matrix4x4::CreatePerspective(Math::Deg2Rad(setting.fovY), setting.ratio, 0.f, 200.f);
		}
	}

	return Matrix4x4(1);
}

void Camera::Translate(Vector3 position)
{
	Matrix4x4 mat;
	mat = Matrix4x4::Rotate(mat, Vector3( 0.f, 1.f, 0.f ), Math::Deg2Rad(rot.y));
	m_ViewMatrix = Matrix4x4::Translate(mat, -position);
}

void Camera::Rotate(float angle, Vector3 axis)
{
	Matrix4x4::Rotate(m_ViewMatrix,axis, angle);
}

const Matrix4x4& Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
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