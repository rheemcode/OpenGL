#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	m_transform =  std::make_shared<Transform>();
}

TransformComponent::TransformComponent(Transform&& p_transform)
{
	m_transform = std::make_shared<Transform>(p_transform);
}

TransformComponent::TransformComponent(const Matrix4x4& p_matrix)
{
	m_transform = std::make_shared<Transform>(p_matrix);
}

TransformComponent::TransformComponent(const Vector3& p_position, const Vector3& p_scale, const Quaternion& p_rotation)
{
	m_transform = std::make_shared<Transform>(p_position, p_scale, p_rotation);
}
