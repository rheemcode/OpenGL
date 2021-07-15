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
	TransformComponent();
	TransformComponent(Transform&& p_transform);
	TransformComponent(const Matrix4x4& p_matrix);
	TransformComponent(const Vector3& p_position, const Vector3& p_scale, const Quaternion& p_rotation);
};