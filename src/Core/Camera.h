#pragma once
#include "Math/Matrix4x4.h"
#include "Tests/Object.h"

enum class CameraMode
{
	PERSPECTIVE = 1,
	ORTHOGRAPHIC
};

struct CameraSettings
{
	CameraSettings(CameraMode _mode, float _ratio, float _fovY)
		: mode(_mode), ratio(_ratio), fovY(_fovY)

	{
	}

	CameraMode mode;
	float fovY, znear, zfar;
	float winWidth, winHeight;
	float ratio;
};

class Transform
{
public:
	Vector3 pos;
	Vector3 rot;
};


class Camera : public Transform
{
public:
	Camera();
	Camera(const CameraMode& mode);
	Camera(const CameraSettings& setting);

	Matrix4x4 MakeProjectionMatrix(const CameraMode& projectionMode);
	Matrix4x4 MakeProjectionMatrix(const CameraSettings& setting);

	Matrix4x4 MakeViewMatrix();

	const Matrix4x4& GetViewMatrix() const;
	const Matrix4x4& GetProjectionMatrix() const;

	void Translate(Vector3 postion);
	void Rotate(float angle, Vector3 direction);

	void Update();


private:
	Matrix4x4 m_ProjectionMatrix;
	Matrix4x4 m_ViewMatrix;
	Matrix4x4 m_ViewProjectionMatrix;

	float m_Width, m_Height;
	Vector3 m_Rotation;

	Vector3 velocity;
	
	class Window* m_Window;
};
