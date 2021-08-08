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
	inFreelook = true;
	if (Input::GetMouseDown(Mouse::RIGHT))
	{
		m_rotationVelocity.x = Input::GetAxis(Input::AXIS_X) * m_mouseSensitivity * m_rotationSpeed;
		m_rotationVelocity.y = Input::GetAxis(Input::AXIS_Y) * m_mouseSensitivity * m_rotationSpeed;

		Vector3 eulerAngles = Quaternion::EulerAngles(p_transform.GetLocalRotation());

		if (eulerAngles.z >= 179.0f || eulerAngles.z <= -179.0f)
		{
			eulerAngles.x += eulerAngles.z;
			eulerAngles.y = 180.0f - eulerAngles.y;
			eulerAngles.z = 0.0f;
		}

		if (eulerAngles.x > 180.0f) eulerAngles.x -= 360.0f;
		if (eulerAngles.x < -180.0f) eulerAngles.x += 360.0f;

		float pitch = eulerAngles.x - m_rotationVelocity.y;
		pitch = Math::Clamp(pitch, -84.f, 84.f);
		float yaw = eulerAngles.y - m_rotationVelocity.x;

		currentController.xRot = pitch;
		currentController.yRot = yaw;
	}

}

void SceneCameraController::Update(Transform& p_transform, float p_delta)
{
	Controller oldController = controller;
	controller = currentController;
	if (!Input::GetMouseDown(Mouse::RIGHT))
		inFreelook = false;

	if (inFreelook)
	{
		float factor = (1.0f / inertia) * p_delta;
		controller.position = Vector3::Lerp(oldController.position, currentController.position, Math::Clamp(factor, 0, 1));

		controller.xRot = Math::Lerp(oldController.xRot, currentController.xRot, MIN(1.f, p_delta * (1 / rotInertia)));
		controller.yRot = Math::Lerp(oldController.yRot, currentController.yRot, MIN(1.f, p_delta * (1 / rotInertia)));

		if (Math::Abs(controller.xRot - currentController.xRot) < 0.1f)
			controller.xRot = currentController.xRot;

		if (Math::Abs(controller.yRot - currentController.yRot) < 0.1f)
			controller.yRot = currentController.yRot;

		Vector3 forward = p_transform.GetWorldForward();
		controller.position = controller.position + forward * controller.distance;

	}
	else
	{
		bool manipulated = false;

		float _rotInertia = MAX(0.00001, manipulated ? manipRotInertia : freelookRotInertia);
		float _inertia = MAX(0.00001, manipulated ? manipIntertia : freelookInertia);

		controller.xRot = Math::Lerp(oldController.xRot, currentController.xRot, MIN(1.f, p_delta * (1 / _rotInertia)));
		controller.yRot = Math::Lerp(oldController.yRot, currentController.yRot, MIN(1.f, p_delta * (1 / _rotInertia)));


		if (Math::Abs(controller.xRot - currentController.xRot) < 0.1f)
			controller.xRot = currentController.xRot;

		if (Math::Abs(controller.yRot - currentController.yRot) < 0.1f)
			controller.yRot = currentController.yRot;

		controller.position = Vector3::Lerp(oldController.position, currentController.position, MIN(1.f, p_delta * (1 / _inertia)));
		controller.distance = Math::Lerp(oldController.distance, currentController.distance, MIN(1.f, p_delta * (1 / zoomInertia)));

	}

	float tolerance = 0.001;
	bool equal = true;

	if (!Math::IsEqualApprox(oldController.xRot, currentController.xRot, tolerance) || !Math::IsEqualApprox(oldController.yRot, currentController.yRot, tolerance))
		equal = false;
	else if (!Vector3::IsEqualApprox(oldController.position, currentController.position))
		equal = false;
	else if (!Math::IsEqualApprox(controller.distance, currentController.distance, tolerance))
		equal = false;

	if (!equal || inFreelook)
	{
		p_transform.SetLocalRotation(Quaternion::FromEulerAngles({ controller.xRot, controller.yRot, 0.f }));
		p_transform.SetLocalPosition(controller.position);
	}
}

void SceneCameraController::HandleKeyboardInput(Transform& p_transform, float p_delta)
{
	if (Input::GetMouseDown(Mouse::RIGHT))
	{
		m_velocity = Vector3();
		if (Input::IsKeyPressed(Key::W))
			m_velocity += p_transform.GetWorldForward();
		if (Input::IsKeyPressed(Key::S))
			m_velocity -= p_transform.GetWorldForward();
		if (Input::IsKeyPressed(Key::D))
			m_velocity += p_transform.GetWorldRight();
		if (Input::IsKeyPressed(Key::A))
			m_velocity -= p_transform.GetWorldRight();
		if (Input::IsKeyPressed(Key::Q))
			m_velocity += p_transform.GetWorldUp();
		if (Input::IsKeyPressed(Key::E))
			m_velocity -= p_transform.GetWorldUp();

		Vector3 position = p_transform.GetLocalPosition();
		m_velocity = Vector3::Normalize(m_velocity) * (m_speed * 5);
		Vector3 displacement = position + m_velocity;
		currentController.position = displacement;
	}
}

void SceneCameraController::UpdateZoom()
{
}


//
//void Node3DEditorViewport::_update_camera(float p_interp_delta) {
//	bool is_orthogonal = camera->get_projection() == Camera3D::PROJECTION_ORTHOGONAL;
//
//	Cursor old_camera_cursor = camera_cursor;
//	camera_cursor = cursor;
//
//	if (p_interp_delta > 0) {
//		//-------
//		// Perform smoothing
//
//		if (is_freelook_active()) {
//			// Higher inertia should increase "lag" (lerp with factor between 0 and 1)
//			// Inertia of zero should produce instant movement (lerp with factor of 1) in this case it returns a really high value and gets clamped to 1.
//			real_t inertia = EDITOR_GET("editors/3d/freelook/freelook_inertia");
//			inertia = MAX(0.001, inertia);
//			real_t factor = (1.0 / inertia) * p_interp_delta;
//
//			// We interpolate a different point here, because in freelook mode the focus point (cursor.pos) orbits around eye_pos
//			camera_cursor.eye_pos = old_camera_cursor.eye_pos.lerp(cursor.eye_pos, CLAMP(factor, 0, 1));
//
//			float orbit_inertia = EDITOR_GET("editors/3d/navigation_feel/orbit_inertia");
//			orbit_inertia = MAX(0.0001, orbit_inertia);
//			camera_cursor.x_rot = Math::lerp(old_camera_cursor.x_rot, cursor.x_rot, MIN(1.f, p_interp_delta * (1 / orbit_inertia)));
//			camera_cursor.y_rot = Math::lerp(old_camera_cursor.y_rot, cursor.y_rot, MIN(1.f, p_interp_delta * (1 / orbit_inertia)));
//
//			if (Math::abs(camera_cursor.x_rot - cursor.x_rot) < 0.1) {
//				camera_cursor.x_rot = cursor.x_rot;
//			}
//
//			if (Math::abs(camera_cursor.y_rot - cursor.y_rot) < 0.1) {
//				camera_cursor.y_rot = cursor.y_rot;
//			}
//
//			Vector3 forward = to_camera_transform(camera_cursor).basis.xform(Vector3(0, 0, -1));
//			camera_cursor.pos = camera_cursor.eye_pos + forward * camera_cursor.distance;
//
//		}
//		else {
//			//when not being manipulated, move softly
//			float free_orbit_inertia = EDITOR_GET("editors/3d/navigation_feel/orbit_inertia");
//			float free_translation_inertia = EDITOR_GET("editors/3d/navigation_feel/translation_inertia");
//			//when being manipulated, move more quickly
//			float manip_orbit_inertia = EDITOR_GET("editors/3d/navigation_feel/manipulation_orbit_inertia");
//			float manip_translation_inertia = EDITOR_GET("editors/3d/navigation_feel/manipulation_translation_inertia");
//
//			float zoom_inertia = EDITOR_GET("editors/3d/navigation_feel/zoom_inertia");
//
//			//determine if being manipulated
//			bool manipulated = Input::get_singleton()->get_mouse_button_mask() & (2 | 4);
//			manipulated |= Input::get_singleton()->is_key_pressed(KEY_SHIFT);
//			manipulated |= Input::get_singleton()->is_key_pressed(KEY_ALT);
//			manipulated |= Input::get_singleton()->is_key_pressed(KEY_CONTROL);
//
//			float orbit_inertia = MAX(0.00001, manipulated ? manip_orbit_inertia : free_orbit_inertia);
//			float translation_inertia = MAX(0.0001, manipulated ? manip_translation_inertia : free_translation_inertia);
//			zoom_inertia = MAX(0.0001, zoom_inertia);
//
//			camera_cursor.x_rot = Math::lerp(old_camera_cursor.x_rot, cursor.x_rot, MIN(1.f, p_interp_delta * (1 / orbit_inertia)));
//			camera_cursor.y_rot = Math::lerp(old_camera_cursor.y_rot, cursor.y_rot, MIN(1.f, p_interp_delta * (1 / orbit_inertia)));
//
//			if (Math::abs(camera_cursor.x_rot - cursor.x_rot) < 0.1) {
//				camera_cursor.x_rot = cursor.x_rot;
//			}
//
//			if (Math::abs(camera_cursor.y_rot - cursor.y_rot) < 0.1) {
//				camera_cursor.y_rot = cursor.y_rot;
//			}
//
//			camera_cursor.pos = old_camera_cursor.pos.lerp(cursor.pos, MIN(1.f, p_interp_delta * (1 / translation_inertia)));
//			camera_cursor.distance = Math::lerp(old_camera_cursor.distance, cursor.distance, MIN(1.f, p_interp_delta * (1 / zoom_inertia)));
//		}
//	}
//
//	//-------
//	// Apply camera transform
//
//	real_t tolerance = 0.001;
//	bool equal = true;
//	if (!Math::is_equal_approx(old_camera_cursor.x_rot, camera_cursor.x_rot, tolerance) || !Math::is_equal_approx(old_camera_cursor.y_rot, camera_cursor.y_rot, tolerance)) {
//		equal = false;
//	}
//	else if (!old_camera_cursor.pos.is_equal_approx(camera_cursor.pos)) {
//		equal = false;
//	}
//	else if (!Math::is_equal_approx(old_camera_cursor.distance, camera_cursor.distance, tolerance)) {
//		equal = false;
//	}
//
//	if (!equal || p_interp_delta == 0 || is_freelook_active() || is_orthogonal != orthogonal) {
//		camera->set_global_transform(to_camera_transform(camera_cursor));
//
//		if (orthogonal) {
//			float half_fov = Math::deg2rad(get_fov()) / 2.0;
//			float height = 2.0 * cursor.distance * Math::tan(half_fov);
//			camera->set_orthogonal(height, get_znear(), get_zfar());
//		}
//		else {
//			camera->set_perspective(get_fov(), get_znear(), get_zfar());
//		}
//
//		update_transform_gizmo_view();
//		rotation_control->update();
//		spatial_editor->update_grid();
//	}
//}
