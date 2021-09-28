#pragma once
#include "Camera.h"

class GLIB_API SceneCamera : public Camera
{

public:

	virtual void OnEvent(const Event& event) override;
	virtual void OnUpdate(float p_delta) override;

	SceneCamera(const CameraSettings& p_cameraSetting)
		: Camera(p_cameraSetting) {}

};


