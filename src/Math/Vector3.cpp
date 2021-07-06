

#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"

Vector3::Vector3(const Vector2& p_vec)
	: x(p_vec.x), y(p_vec.y), z(0) {}


Vector3 Vector3::RotateX(const Vector3& p_vec, float p_angleRad)
{
	return Vector3(
		p_vec.x, 
		p_vec.y * Math::Cos(p_angleRad) - p_vec.z * Math::Sin(p_angleRad),
		p_vec.y * Math::Sin(p_angleRad) + p_vec.z * Math::Cos(p_angleRad)
		);
}

Vector3 Vector3::RotateY(const Vector3& p_vec, float p_angleRad)
{
	return Vector3(
		p_vec.x * Math::Cos(p_angleRad) + p_vec.z * Math::Sin(p_angleRad),
		p_vec.y,
		-p_vec.x * Math::Sin(p_angleRad) + p_vec.z * Math::Cos(p_angleRad)
		);
}

Vector3 Vector3::RotateZ(const Vector3& p_vec, float p_angleRad)
{
	return Vector3(
		p_vec.x * Math::Cos(p_angleRad) - p_vec.y * Math::Sin(p_angleRad),
		p_vec.x * Math::Sin(p_angleRad) + p_vec.y * Math::Cos(p_angleRad),
		p_vec.z
		);
}

Vector3 Vector3::Lerp(const Vector3& p_from, const Vector3& p_to, const float p_delta)
{
	return Vector3
	(
		Math::Lerp(p_from.x, p_to.x, p_delta),
		Math::Lerp(p_from.y, p_to.y, p_delta),
		Math::Lerp(p_from.z, p_to.z, p_delta)
	);
}

Vector3 Vector3::MoveTowards(const Vector3& p_from, const Vector3& p_to, const float p_delta)
{
	return Vector3();
}



//float Vector3::AngleBetween(const Vector3& p_vecA, const Vector3& p_vecB)
//{
//	float dotP = Dot(p_vecA, p_vecB);
//	float mag = Math::Sqrt(Dot(p_vecA, p_vecA)) * Math::Sqrt(Dot(p_vecB, p_vecB));
//
//	return Math::ACos(dotP / mag);
//}


Vector3 Vector3::Rotate(const Vector3& p_vec, float p_angleRad)
{
	return Quaternion::Axis(Quaternion::AngleAxis(p_angleRad, p_vec));
}


Vector3 operator*(const float& p_scalar, const Vector3& p_vec)
{
	return Vector3(p_vec.x * p_scalar, p_vec.y * p_scalar, p_vec.z * p_scalar);
}

