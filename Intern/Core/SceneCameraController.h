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

	struct Controller
	{
		Vector3 position;
		float xRot = 0.5f, yRot = -0.5f, distance;
		float rotationSpeed;

	};
	
	Controller controller;
	Controller currentController;

	bool inFreelook = false;
	
	float m_speed;
	float inertia = 0.15f;
	float rotInertia = 0.05f;
	float zoomInertia = 0.08f;
	float freelookRotInertia = 0.1f;
	float freelookInertia = 0.1f;
	float manipIntertia= 0.08f;
	float manipRotInertia = 0.08f;

	float m_mouseSensitivity;
	float m_rotationSpeed = 1.f;

	Vector3 m_focalPoint;
	Vector3 m_position;
	Vector3 m_velocity;
	Vector2 m_rotationVelocity;
	Vector2 m_oldMousePos;

public:
	void OnEvent(const Event& event);
	void HandleMouseInput(Transform& p_transform, float xPos, float yPos);
	void Update(Transform& p_transform, float p_delta);
	void HandleKeyboardInput(Transform& p_transform, float p_delta);
	void UpdateZoom();

	SceneCameraController()
	{
		m_speed = 5.f;
		m_mouseSensitivity = 0.1f;
		controller.distance = 4.f;
		controller.rotationSpeed = 3.f;

		m_position = Vector3::Zero();
		m_velocity = Vector3::Zero();
		m_rotationVelocity = Vector3::Zero();
	}
};