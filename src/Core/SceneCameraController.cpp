#include "SceneCameraController.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Input/Input.h"
#include "Math/Math.h"
#include <Window/Window.h>

void SceneCameraController::OnEvent(const Event& event)
{
}

void SceneCameraController::HandleMouseInput(Transform& p_transform, float xPos, float yPos)
{
	static bool mouseHeld;
	m_rotationVelocity = Vector3();
	if (Input::GetMouseDown(Mouse::RIGHT))
	{
		mouseHeld = true;
		m_rotationVelocity.x = Input::GetAxis(Input::AXIS_X) *m_mouseSentivity* m_rotationSpeed;
		m_rotationVelocity.y = Input::GetAxis(Input::AXIS_Y) *m_mouseSentivity* m_rotationSpeed;

		Vector3 eulerAngles = Quaternion::EulerAngles(p_transform.GetLocalRotation());

		if (eulerAngles.z >= 179.0f || eulerAngles.z <= -179.0f)
		{
			eulerAngles.x += eulerAngles.z;
			eulerAngles.y = 180.0f - eulerAngles.y;
			eulerAngles.z = 0.0f;
		}

		if (eulerAngles.x > 180.0f) eulerAngles.x -= 360.0f;
		if (eulerAngles.x < -180.0f) eulerAngles.x += 360.0f;

		std::stringstream ss;
		ss << "Setting World Rotation: Euler Angles (" << eulerAngles.x << "," << eulerAngles.y << "," << eulerAngles.z << ")\n";
		Console::Log(ss.str().c_str());
		float pitch = eulerAngles.x - m_rotationVelocity.y;
		pitch = Math::Clamp(pitch, -84.f, 84.f);
		float yaw = eulerAngles.y - m_rotationVelocity.x;
		p_transform.SetLocalRotation(Quaternion::FromEulerAngles({ pitch, yaw, 0.f }));
	}



}

void SceneCameraController::HandleKeyboardInput(Transform& p_transform)
{
	if (Input::GetMouseDown(Mouse::RIGHT))
	{
		m_velocity = Vector3();
		if (Input::IsKeyPressed(Key::W))
			m_velocity += p_transform.GetWorldForward() * m_speed;
		if (Input::IsKeyPressed(Key::S))
			m_velocity -= p_transform.GetWorldForward() * m_speed;
		if (Input::IsKeyPressed(Key::D))
			m_velocity += p_transform.GetWorldRight() * m_speed;
		if (Input::IsKeyPressed(Key::A))
			m_velocity -= p_transform.GetWorldRight() * m_speed;
		if (Input::IsKeyPressed(Key::Q))
			m_velocity += p_transform.GetWorldUp() * m_speed;
		if (Input::IsKeyPressed(Key::E))
			m_velocity -= p_transform.GetWorldUp() * m_speed;

		Vector3 position = p_transform.GetLocalPosition();
		position += m_velocity;
		p_transform.SetLocalPosition(position);
	}
}

void SceneCameraController::UpdateZoom()
{
}
