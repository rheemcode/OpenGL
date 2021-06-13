#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
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
	float fovY, near, far;
	float winWidth, winHeight;
	float ratio;
};

class Transform
{
public:
	glm::vec3 pos;
	glm::vec3 rot;
};


class Camera : public Transform
{
public:
	Camera();
	Camera(const CameraMode& mode);
	Camera(const CameraSettings& setting);

	glm::mat4x4 MakeProjectionMatrix(const CameraMode& projectionMode);
	glm::mat4x4 MakeProjectionMatrix(const CameraSettings& setting);

	glm::mat4x4 MakeViewMatrix();

	glm::mat4x4 GetViewMatrix() const;
	glm::mat4x4 GetProjectionMatrix() const;

	void Translate(glm::vec3 postion);
	void Rotate(float angle, glm::vec3 direction);

	void Update();


private:
	glm::mat4x4 m_ProjectionMatrix;
	glm::mat4x4 m_ViewMatrix;
	glm::mat4x4 m_ViewProjectionMatrix;

	float m_Width, m_Height;
	glm::vec3 m_Rotation;

	glm::vec3 velocity;
	
	class Window* m_Window;
};
