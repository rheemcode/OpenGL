#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "Tests/Object.h"
#include "Math/Quaternion.h"
#include "Math/Transform.h"
#include "SceneCameraController.h"
#include "glm/gtc/matrix_transform.hpp"


enum class CameraMode
{
	PERSPECTIVE = 1,
	ORTHOGRAPHIC
};

struct CameraSettings
{
	CameraSettings() {}

	CameraSettings(CameraMode _mode, float _ratio, float _fovY)
		: mode(_mode), ratio(_ratio), fovY(_fovY)

	{
	}

	CameraMode mode;
	float fovY, znear, zfar;
	float winWidth, winHeight;
	float ratio;
};

class Camera
{

protected:
	Matrix4x4 m_ProjectionMatrix;
	Matrix4x4 m_ViewMatrix;
	Matrix4x4 m_ViewProjectionMatrix;

	float m_Width, m_Height;

	Matrix4x4 MakeProjectionMatrix(const CameraMode& projectionMode);
	Matrix4x4 MakeProjectionMatrix(const CameraSettings& setting);
	Matrix4x4 MakeViewMatrix();

public:
	const Matrix4x4& GetViewMatrix() const;
	const Matrix4x4& GetProjectionMatrix() const;
	const Matrix4x4& GetViewProjectionMatrix() const;

	Camera();
	Camera(const CameraMode& mode);
	Camera(const CameraSettings& setting);

};

class SceneCamera : public Camera
{
	Transform transform;
	SceneCameraController cameraController;

public:
	SceneCamera(const CameraSettings& p_cameraSetting)
		: Camera(p_cameraSetting){}
	
	void OnEvent(const Event& event);
	void OnUpdate()
	{
		cameraController.HandleKeyboardInput(transform);
		UpdateView();
	}

	void UpdateView()
	{
		auto mat = glm::inverse(glm::mat4(1));
		m_ViewMatrix = Matrix4x4::Inverse(transform.GetWorldMatrix());
	}
};
