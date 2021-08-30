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

struct GLIB_API CameraSettings
{
	CameraMode mode;
	float fovY, znear, zfar;
	float left, right, top, bottom;
	float winWidth, winHeight;
	float ratio;
};

class GLIB_API Camera
{
protected:

	Transform transform;
	std::shared_ptr<CameraController> cameraController;
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
	const CameraSettings GetCameraSettings() const { return m_cameraSettings; }
	const Transform& GetTransform() const { return transform; }

	virtual Frustum& GetFrustum() { return m_Frustum; };
	virtual void OnUpdate(float p_delta) {};
	virtual void OnEvent(const Event& event) {};


	const CameraController* const GetController() const { cameraController.get(); }
	void AttachController(CameraController* p_cameraController)
	{
		cameraController.reset(p_cameraController);
	}

	Camera();
	Camera(const CameraMode& mode);
	Camera(const CameraSettings& setting);

};
