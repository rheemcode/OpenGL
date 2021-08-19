#pragma once
#include "Vector3.h"
#include "Math/Matrix3x3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

class Transform
{
	Vector3 m_localPosition;
	Vector3 m_localScale;
	Quaternion m_localRotation;
	Vector3 m_worldPosition;
	Vector3 m_worldScale;
	Quaternion m_worldRotation;
	
	Matrix4x4 m_localMatrix;
	Matrix4x4 m_worldMatrix;

	const Transform* m_parent;
	bool hasParent;
	bool transformDirty;

public:
	void UpdateTransform();
	void ApplyTransform();
	void GenerateMatrix(const Vector3& p_position, const Vector3& p_scale, const Quaternion& p_rotation);
	void SetParent(const Transform& p_parent);
	bool RemoveParent(const Transform& p_parent);

	const Matrix4x4& GetWorldMatrix() const { return m_worldMatrix; }

	void SetLocalMatrix(const Matrix4x4& p_matrix);
	const Matrix4x4& GetLocalMatrix() const { return m_localMatrix;  }

	void SetLocalPosition(const Vector3& p_localPosition);
	void SetLocalRotation(const Quaternion& p_localRotation);
	void SetLocalScale(const Vector3& p_localScale);

	const Vector3& GetLocalPosition() const { return m_localPosition;  }
	const Quaternion& GetLocalRotation() const { return m_localRotation;  }
	const Vector3& GetLocalScale() const { return m_localScale; }

	const Vector3& GetWorldPosition() const { return m_worldPosition; }
	const Quaternion& GetWorldRotation() const { return m_worldRotation; }
	const Vector3& GetWorldScale() const { return m_worldScale; }

	void operator=(const Transform& p_transform)
	{
		m_localPosition = (p_transform.m_localPosition);
		m_localRotation = (p_transform.m_localRotation);
		m_localScale = (p_transform.m_localScale);
		m_localMatrix = (p_transform.m_localMatrix);
		m_worldMatrix = (p_transform.m_worldMatrix);
		hasParent = p_transform.hasParent;
		transformDirty = p_transform.transformDirty;
	}
	void operator=(Transform&& p_transform) noexcept
	{
		m_localPosition = std::move(p_transform.m_localPosition);
		m_localRotation = std::move(p_transform.m_localRotation);
		m_localScale = std::move(p_transform.m_localScale);
		m_localMatrix = std::move(p_transform.m_localMatrix);
		m_worldMatrix = std::move(p_transform.m_worldMatrix);
		hasParent = p_transform.hasParent;
		transformDirty = p_transform.transformDirty;
	}


	Vector3 GetWorldForward() const 
	{
		return m_worldRotation * Vector3::Forward();
	}
	Vector3 GetWorldUp() const
	{
		return m_worldRotation * Vector3::Up();
	}

	Vector3 GetWorldRight() const
	{
		return m_worldRotation * Vector3::Right();
	}
	Vector3 GetLocalForward() const
	{
		return m_localRotation * Vector3::Forward();
	}
	Vector3 GetLocalUp() const
	{
		return m_localRotation * Vector3::Up();
	}
	Vector3 GetLocalRight() const
	{
		return m_localRotation * Vector3::Right();
	}

	Transform();
	Transform(Transform&& p_transform) noexcept;
	Transform(const Transform& p_transform);
	Transform(const Matrix4x4& p_matrix);
	Transform(const Vector3& p_position, const Vector3& p_scale, const Quaternion& p_rotation);

};