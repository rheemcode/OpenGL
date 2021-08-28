#pragma once
#include "Camera.h"

class SceneCamera : public Camera
{

public:

	virtual void OnEvent(const Event& event) override;
	virtual void OnUpdate(float p_delta) override;

	virtual Frustum& GetFrustum() override
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
		: Camera(p_cameraSetting) {}

};


