#include "Transform.h"
#include "Window/Window.h"
#include <sstream>

void Transform::UpdateTransform()
{
	m_localMatrix = Matrix4x4::CreateTranslation(m_localPosition) * Quaternion::ToMatrix4x4(m_localRotation) * Matrix4x4::CreateScale(m_localScale);
	m_worldMatrix = hasParent ? m_parent->GetWorldMatrix() * m_localMatrix : m_localMatrix;

	m_worldPosition = m_worldMatrix[3];
	m_worldRotation = Quaternion::FromMatrix4x4(m_worldMatrix);
	m_worldScale = m_worldMatrix.GetScale();
	transformDirty = false;
}

void Transform::ApplyTransform()
{
	m_localPosition = m_localMatrix[3];
	m_localRotation = Quaternion(m_localMatrix.GetRotation());
	m_localScale = m_localMatrix.GetScale();
}

void Transform::GenerateMatrix(const Vector3& p_position, const Vector3& p_scale, const Quaternion& p_rotation)
{
	m_localPosition = p_position;
	m_localRotation = p_rotation;
	m_localScale = p_scale;

	UpdateTransform();
	transformDirty = false;
}

void Transform::SetParent(const Transform& p_parent)
{
	m_parent = &p_parent;
	hasParent = true;
	UpdateTransform();
}

//void Transform::SetWorldMatrix(const Matrix4x4& p_matrix)
//{
//	auto mat = Matrix4x4(p_matrix);
//	m_parentWorldMatrix = &mat;
//	m_worldMatrix = p_matrix * m_localMatrix;
//	hasParent = true;
//}

void Transform::SetLocalMatrix(const Matrix4x4& p_matrix)
{
	m_localMatrix = p_matrix;
	ApplyTransform();
}

void Transform::SetLocalPosition(const Vector3& p_localPosition)
{
	GenerateMatrix(p_localPosition, m_localScale, m_localRotation);
}

void Transform::SetLocalScale(const Vector3& p_localScale)
{
	GenerateMatrix(m_localPosition, p_localScale, m_localRotation);
}

void Transform::SetLocalRotation(const Quaternion& p_localRotation)
{
	GenerateMatrix(m_localPosition, m_localScale, p_localRotation);
}


bool Transform::RemoveParent(const Transform& p_parent)
{
	return true;
}


Transform::Transform()
{
	m_localPosition = Vector3();
	m_localRotation = Quaternion();
	m_localScale = Vector3(1.f, 1.f, 1.f);
	m_localMatrix = Matrix4x4();
	m_worldMatrix = Matrix4x4();

	transformDirty = false;
	hasParent = false;
}

Transform::Transform(Transform&& p_transform) noexcept
{
	m_localPosition = std::move(p_transform.m_localPosition);
	m_localRotation = std::move(p_transform.m_localRotation);
	m_localScale    = std::move(p_transform.m_localScale);
	m_localMatrix   = std::move(p_transform.m_localMatrix);
	m_worldMatrix   = std::move(p_transform.m_worldMatrix);
}

Transform::Transform(const Transform& p_transform)
{
	m_localPosition = (p_transform.m_localPosition);
	m_localRotation = (p_transform.m_localRotation);
	m_localScale    = (p_transform.m_localScale);
	m_localMatrix   = (p_transform.m_localMatrix);
	m_worldMatrix   = (p_transform.m_worldMatrix);
}

Transform::Transform(const Matrix4x4& p_matrix)
{
	m_localMatrix = p_matrix;
	m_localPosition = p_matrix[3];
	m_localScale = m_localMatrix.GetScale();
	m_localRotation = Quaternion(p_matrix.GetRotation());

	transformDirty = false;
	hasParent = false;
}

Transform::Transform(const Vector3& p_position, const Vector3& p_scale, const Quaternion& p_rotation)
{
	m_localMatrix = Matrix4x4::CreateTranslation(p_position) * Quaternion::ToMatrix4x4(p_rotation) * Matrix4x4::CreateScale(p_scale);
	m_localPosition = p_position;
	m_localRotation = p_rotation;
	m_localScale = p_scale;

	hasParent = false;
}