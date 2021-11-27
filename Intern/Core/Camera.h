#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "Tests/Object.h"
#include "Math/Quaternion.h"
#include "Math/Transform.h"
#include "ECS/SceneCameraController.h"
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
	int winWidth, winHeight;
	float ratio;
};

class GLIB_API Camera
{
protected:
	friend class Scene;
	float m_Width, m_Height;
	CameraSettings m_cameraSettings;

	Matrix4x4 m_ProjectionMatrix;
	Matrix4x4 m_ViewMatrix;
	mutable Matrix4x4 m_ViewProjectionMatrix;
	Frustum m_Frustum;
	Transform transform;

	std::shared_ptr<CameraController> cameraController;
	Matrix4x4 MakeProjectionMatrix(const CameraMode& projectionMode);
	Matrix4x4 MakeProjectionMatrix(const CameraSettings& setting);
	Matrix4x4 MakeViewMatrix();

public:
	const Matrix4x4& GetViewMatrix() const;
	const Matrix4x4& GetProjectionMatrix() const;
	const Matrix4x4& GetViewProjectionMatrix() const;
<<<<<<< HEAD

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
=======
>>>>>>> 9a09ef6660b98f11e584c8baced474b13e6f1ea0
	const CameraSettings GetCameraSettings() const { return m_cameraSettings; }
	const Transform& GetTransform() const { return transform; }

	virtual void OnUpdate(float p_delta) {};
	virtual void OnEvent(const Event& event) {};

	inline void UpdateView()
	{
		m_ViewMatrix = Matrix4x4::Inverse(transform.GetWorldMatrix());
	}

	inline const Frustum& GetFrustum() {
		UpdateView();
		m_Frustum.SetFrustum(m_ProjectionMatrix * m_ViewMatrix);
		return m_Frustum;
	};

	const CameraController* const GetController() const { return cameraController.get(); }
	void AttachController(CameraController* p_cameraController)
	{
		cameraController.reset(p_cameraController);
		cameraController->cameraTransform = &transform;
	}
	
	void AttachController(std::shared_ptr<CameraController> p_cameraController)
	{
		cameraController = p_cameraController;
		cameraController->cameraTransform = &transform;
	}

	Camera();
	Camera(const CameraMode& mode);
	Camera(const CameraSettings& setting);

};
