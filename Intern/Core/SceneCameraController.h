#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "Tests/Object.h"
#include "Math/Quaternion.h"
#include "Math/Transform.h"
#include "Events/Event.h"

class SceneCameraController
{
	float m_mouseSentivity;
	float m_speed;
	float m_rotationSpeed;
	float m_distance;
	float m_zoom;


	Vector3 m_focalPoint;
	Vector3 m_position;
	Vector3 m_velocity;
	Vector2 m_rotationVelocity;
	Vector2 m_oldMousePos;

public:
	void OnEvent(const Event& event);
	void HandleMouseInput(Transform& p_transform, float xPos, float yPos);
	void HandleKeyboardInput(Transform& p_transform, float p_delta);
	void UpdateZoom();

	SceneCameraController()
	{
		m_mouseSentivity = 0.1f;
		m_speed = 5.f;
		m_distance = 10.f;
		m_zoom = 0;

		m_focalPoint = Vector3::Zero();
		m_position = Vector3::Zero();
		m_velocity = Vector3::Zero();
		m_rotationVelocity = Vector3::Zero();
 		m_rotationSpeed = 5.f;
	}
};