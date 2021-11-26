#include <glpch.h>
#include "TransformComponent.h"

TransformComponent::TransformComponent(std::shared_ptr<Actor> p_actor)
	: Component(p_actor)
{
	m_transform =  std::make_shared<Transform>();
}

TransformComponent::TransformComponent(std::shared_ptr<Actor> p_actor, Transform&& p_transform)
	: Component(p_actor)
{
	m_transform = std::make_shared<Transform>(p_transform);
}

TransformComponent::TransformComponent(std::shared_ptr<Actor> p_actor, const Matrix4x4& p_matrix)
	: Component(p_actor)
{
	m_transform = std::make_shared<Transform>(p_matrix);
}

TransformComponent::TransformComponent(std::shared_ptr<Actor> p_actor, const Vector3& p_position, const Vector3& p_scale, const Quaternion& p_rotation)
	: Component(p_actor)
{
	m_transform = std::make_shared<Transform>(p_position, p_scale, p_rotation);
}
