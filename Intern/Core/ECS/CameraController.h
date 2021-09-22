#pragma once
#include "GLCore.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "Math/Quaternion.h"
#include "Math/Transform.h"
#include "Events/Event.h"


class GLIB_API CameraController
{
	friend class Camera;
protected:
	Transform* cameraTransform;

public:

	virtual void OnEvent(const Event& event) = 0;
	virtual void OnUpdate(float p_delta) = 0;

	CameraController() = default;
};
