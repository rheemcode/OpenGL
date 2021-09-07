#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "Tests/Object.h"
#include "Math/Quaternion.h"
#include "Math/Transform.h"
#include "SceneCameraController.h"
#include "Math/Frustum.h"


enum class CameraMode
{
	PERSPECTIVE = 1,
	ORTHOGRAPHIC
};

struct CameraSettings
{
	CameraMode mode;
	float fovY, znear, zfar;
	float left, right, top, bottom;
	float winWidth, winHeight;
	float ratio;
};

class Camera
{
protected:
	CameraSettings m_cameraSettings;

	Matrix4x4 m_ProjectionMatrix;
	Matrix4x4 m_ViewMatrix;
	Matrix4x4 m_ViewProjectionMatrix;

	Frustum m_Frustum;
	float m_Width, m_Height;
	Matrix4x4 MakeProjectionMatrix(const CameraMode& projectionMode);
	Matrix4x4 MakeProjectionMatrix(const CameraSettings& setting);
	Matrix4x4 MakeViewMatrix();

public:
	const Matrix4x4& GetViewMatrix() const;
	const Matrix4x4& GetProjectionMatrix() const;
	const Matrix4x4& GetViewProjectionMatrix() const;

	virtual Frustum& GetFrustum() { return m_Frustum; };

	Camera();
	Camera(const CameraMode& mode);
	Camera(const CameraSettings& setting);

};

class SceneCamera : public Camera
{
	friend class Scene;
	Transform transform;
	SceneCameraController cameraController;

public:
	const CameraSettings GetCameraSettings() const { return m_cameraSettings; }
	const Transform& GetTransform() const { return transform; }

	void OnEvent(const Event& event);
	void OnUpdate(float p_delta)
	{
		cameraController.HandleKeyboardInput(transform, p_delta);
		cameraController.Update(transform, p_delta);
		UpdateView();
	}

	virtual Frustum& GetFrustum()
	{
		UpdateView();
		m_Frustum.SetFrustum(m_ProjectionMatrix * m_ViewMatrix);
		return m_Frustum;
	}

	void UpdateView()
	{
		m_ViewMatrix = Matrix4x4::Inverse(transform.GetWorldMatrix());
	}

	SceneCamera(const CameraSettings& p_cameraSetting)
		: Camera(p_cameraSetting) {
		cameraController.transform = &transform;
	}

};
