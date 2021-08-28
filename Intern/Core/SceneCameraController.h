#pragma once
#include "CameraController.h"

class SceneCameraController : CameraController
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
	float inertia = 0.85f;
	float rotInertia = 0.1f;
	float zoomInertia = 0.08f;
	float freelookRotInertia = 0.1f;
	float freelookInertia = 0.8f;
	float manipIntertia= 0.08f;
	float manipRotInertia = 0.08f;

	float m_mouseSensitivity;
	float m_rotationSpeed = 12.f;

	Vector3 m_focalPoint;
	Vector3 m_position;
	Vector3 m_velocity;
	Vector2 m_rotationVelocity;
	Vector2 m_oldMousePos;

public:
	virtual void OnEvent(const Event& event) override;
	virtual void OnUpdate(float p_delta) override;

	void HandleMouseInput(float xPos, float yPos);
	void Update(float p_delta);
	void HandleKeyboardInput(float p_delta);
	void UpdateZoom();

	SceneCameraController()
	{
		m_speed = 2.f;
		m_mouseSensitivity = 0.8f;
		controller.distance = 1.f;
		controller.rotationSpeed = 3.f;

		m_position = Vector3::Zero();
		m_velocity = Vector3::Zero();
		m_rotationVelocity = Vector3::Zero();
	}
};