#pragma once

#include "Component.h"
#include <memory>
#include "Math/Transform.h"

class TransformComponent : public Component
{
	std::shared_ptr<Transform> m_transform;
protected:
	virtual ComponentCategory GetComponentCategoryImpl() const override { return ComponentCategory::Transformable; }
	virtual std::string GetComponentNameImpl() const override { return "Transform Component"; }
public:
	const std::shared_ptr<Transform>& GetTransform() const { return m_transform; }
	TransformComponent(std::shared_ptr<Actor> p_actor);
	TransformComponent(std::shared_ptr<Actor> p_actor, Transform&& p_transform);
	TransformComponent(std::shared_ptr<Actor> p_actor, const Matrix4x4& p_matrix);
	TransformComponent(std::shared_ptr<Actor> p_actor, const Vector3& p_position, const Vector3& p_scale, const Quaternion& p_rotation);
};