#include <glpch.h>
#include "Quaternion.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Matrix3x3.h"
#include <cassert>

Quaternion::Quaternion(const Vector3& p_eulerAngles)
{
	auto c = Vector3::Cos(p_eulerAngles * 0.5f);
	auto s = Vector3::Sin(p_eulerAngles * 0.5f);

	this->w = c.x * c.y * c.z + s.x * s.y * s.z;
	this->x = s.x * c.y * c.z - c.x * s.y * s.z;
	this->y = c.x * s.y * c.z + s.x * c.y * s.z;
	this->z = c.x * c.y * s.z - s.x * s.y * c.z;
}

Quaternion::Quaternion(const float& p_s, const Vector3& p_eulerAngles)
	: x(p_eulerAngles.x), y(p_eulerAngles.y), z(p_eulerAngles.z), w(p_s) {}

Quaternion::Quaternion(const Matrix4x4& p_mat)
{
	*this = FromMatrix4x4(p_mat);
}

Quaternion::Quaternion(const Matrix3x3& p_mat)
{
	*this = FromMatrix3x3(p_mat);
}

Quaternion& Quaternion::operator=(const Quat& p_quat)
{
	this->w = (p_quat.w);
	this->x = (p_quat.x);
	this->y = (p_quat.y);
	this->z = (p_quat.z);
	return *this;
}

Quaternion& Quaternion::operator+=(const Quat& p_quat)
{
	this->w += (p_quat.w);
	this->x += (p_quat.x);
	this->y += (p_quat.y);
	this->z += (p_quat.z);
	return *this;
}

Quaternion& Quaternion::operator-=(const Quat& p_quat)
{
	this->w -= (p_quat.w);
	this->x -= (p_quat.x);
	this->y -= (p_quat.y);
	this->z -= (p_quat.z);
	return *this;
}

Quaternion& Quaternion::operator*=(const float& s)
{
	this->w *= s;
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}
Quaternion& Quaternion::operator/=(const float& s)
{
	this->w /= s;
	this->x /= s;
	this->y /= s;
	this->z /= s;
	return *this;
}

Quaternion operator-(const Quaternion& p_quat)
{
	return Quaternion(-p_quat.w, -p_quat.x, -p_quat.y, -p_quat.z);
}


Quaternion& Quaternion::operator*=(const Quat& p_quat)
{
	const Quat p(*this);
	const Quat q(p_quat);
	this->w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
	this->x = p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y;
	this->y = p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z;
	this->z = p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x;
	return *this;
}

float Quaternion::Length(const Quat& p_quat)
{
	return Math::Sqrt(Dot(p_quat, p_quat));
}

Quaternion Quaternion::Normalize(const Quat& p_quat)
{
	float length = Length(p_quat);
	if (length <= 0)
		return Quat(1.f, 0, 0, 0);
	float inv = 1.f / length;
	return Quat(p_quat.w * inv, p_quat.x * inv, p_quat.y * inv, p_quat.z * inv);
}

Quaternion Quaternion::Cross(const Quat& p_quat, const Quat& p_quatB)
{
	return Quat(
		p_quat.w * p_quatB.w - p_quat.x * p_quatB.x - p_quat.y * p_quatB.y - p_quat.z * p_quatB.z,
		p_quat.w * p_quatB.w + p_quat.x * p_quatB.x + p_quat.y * p_quatB.y - p_quat.z * p_quatB.z,
		p_quat.w * p_quatB.w + p_quat.x * p_quatB.x + p_quat.y * p_quatB.y - p_quat.z * p_quatB.z,
		p_quat.w * p_quatB.w + p_quat.x * p_quatB.x + p_quat.y * p_quatB.y - p_quat.z * p_quatB.z
	);
}

Quaternion Quaternion::Mix(const Quat& p_quat, const Quat& p_quatB, const float& p_w)
{
	float cosTheta = Dot(p_quat, p_quatB);

	if (cosTheta > 1.0f - EPSILON)
	{
		return Quat(
			p_quat.w + p_w * (p_quatB.w - p_quat.w),
			p_quat.x + p_w * (p_quatB.x - p_quat.w),
			p_quat.y + p_w * (p_quatB.y - p_quat.w),
			p_quat.z + p_w * (p_quatB.z - p_quat.w)
		);
	} 
	else
	{
		float angle = Math::Abs(cosTheta);
		return (Math::Sin((1.f - p_w) * angle) * p_quat + Math::Sin(p_w * angle) * p_quatB) / Math::Sin(angle);
	}
}

float Quaternion::Dot(const Quat& a, const Quat& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Quaternion Quaternion::Lerp(const Quat& p_quat, const Quat& p_quatB, const float& p_w)
{
	assert(p_w >= 0.f);
	assert(p_w <= 1.f);

	return p_quat * (1.f - p_w) + (p_quatB * p_w);
}
Quaternion Quaternion::NLerp(const Quat& p_quat, const Quat& p_quatB, const float& p_w)
{
	return Normalize(Quat(Lerp(p_quat, p_quatB, p_w)));
}

Quaternion Quaternion::Slerp(const Quat& p_quat, const Quat& p_quatB, const float& p_w)
{
	Quat z = p_quatB;

	float cosTheta = Dot(p_quat, p_quatB);
	
	if (cosTheta < 1.0f)
	{
		z = -p_quatB;
		cosTheta = -cosTheta;
	}

	if (cosTheta > 1.f - EPSILON)
	{
		return Quat(
			p_quat.w + p_w * (z.w - p_quat.w),
			p_quat.x + p_w * (z.x - p_quat.w),
			p_quat.y + p_w * (z.y - p_quat.w),
			p_quat.z + p_w * (z.z - p_quat.w)
		);
	}
	else
	{
		float angle = Math::ACos(cosTheta);
		return (Math::Sin((1.f - p_w) * angle) * p_quat + Math::Sin(p_w * angle) * z) / Math::Sin(angle);
	}

}

Quaternion Quaternion::FromEulerAngles(const Vector3& p_angles)
{
	Vector3 s, c;
	s = Vector3::Sin(Vector3(0.5f * Math::Deg2Rad(p_angles.x), 0.5f * Math::Deg2Rad(p_angles.y), 0.5f * Math::Deg2Rad(p_angles.z)));
	c = Vector3::Cos(Vector3(0.5f * Math::Deg2Rad(p_angles.x), 0.5f * Math::Deg2Rad(p_angles.y), 0.5f * Math::Deg2Rad(p_angles.z)));

	return Quat
	(
		c.x * c.y * c.z + s.x * s.y * s.z,
		s.x * c.y * c.z - c.x * s.y * s.z,
		c.x * s.y * c.z + s.x * c.y * s.z,
		c.x * c.y * s.z - s.x * s.y * c.z
	);

}

Vector3 Quaternion::EulerAngles(const Quat& p_quat)
{
	return Vector3(Math::Rad2deg(Pitch(p_quat)), Math::Rad2deg(Yaw(p_quat)), Math::Rad2deg(Roll(p_quat)));
}

Quaternion Quaternion::LookRotation(const Vector3& p_forward, const Vector3& p_up)
{
	const Vector3 t = Vector3::Normalize(Vector3::Cross(p_up, p_forward));
	const Vector3 c = Vector3::Cross(p_forward, t);

	return Quaternion(Matrix3x3(t, c, p_forward));
}

Quaternion Quaternion::Rotate(const Quat& p_quat, const float& angle, const Vector3& p_vec)
{

	Vector3 tmp = p_vec;

	float len = Vector3::Length(tmp);
	if (Math::Abs(len - 1.f) > EPSILON)
	{
		float inv = 1.f / len;
		tmp.x *= inv;
		tmp.y *= inv;
		tmp.z *= inv;
	}
	
	const float s = Math::Sin(angle * 0.5f);
	return p_quat * Quat(Math::Cos(angle * 0.5f), tmp.x * s, tmp.y * s, tmp.z * s);
}

Quaternion Quaternion::RotateX( const float& angle)
{
	float c(Math::Cos(0.5f * angle));
	float s(Math::Sin(0.5f * angle));
	return Quat(c, s, 0.f, 0.f);
}
Quaternion Quaternion::RotateY( const float& angle)
{
	float c(Math::Cos(0.5f * angle));
	float s(Math::Sin(0.5f * angle));
	return Quat(c, 0.f, s, 0.f);
}
Quaternion Quaternion::RotateZ( const float& angle)
{
	float c(Math::Cos(0.5f * angle));
	float s(Math::Sin(0.5f * angle));
	return Quat(c, 0.f, 0.f, s);
}

float Quaternion::Angle(const Quat& p_quat)
{
	return Math::ACos(p_quat.w) * 2.f;
}

Vector3 Quaternion::Axis(const Quat& p_quat)
{
	float tmp1 = 1.f - p_quat.w * p_quat.w;
	if (tmp1 <= 0.f)
		return Vector3(0, 0, 1);
	float tmp2 = 1.f / Math::Sqrt(tmp1);
	return Vector3(p_quat.x * tmp2, p_quat.y * tmp2, p_quat.z * tmp2);
}

Quaternion Quaternion::AngleAxis(const float p_theta, const Vector3& p_axis)
{
	Quat Result(0);

	const float a(p_theta);
	const float s = Math::Sin(a * 0.5f);

	Result.w = Math::Cos(a *0.5f);
	Result.x = p_axis.x * s;
	Result.y = p_axis.y * s;
	Result.z = p_axis.z * s;
	return Result;
}

float Quaternion::Pitch(const Quat& p_quat)
{
	return Math::ATan2(2.f * (p_quat.y * p_quat.z + p_quat.w * p_quat.x), p_quat.w * p_quat.w - p_quat.x * p_quat.x - p_quat.y * p_quat.y + p_quat.z * p_quat.z);
}

float Quaternion::Yaw(const Quat& p_quat)
{
	float s = Math::Clamp(-2.f * (p_quat.x * p_quat.z - p_quat.w * p_quat.y), -1.f, 1.f);
	return Math::ASin(s);
}

float Quaternion::Roll(const Quat& p_quat)
{
	return Math::ATan2(2.f * (p_quat.x * p_quat.y + p_quat.w * p_quat.z), p_quat.w * p_quat.w + p_quat.x * p_quat.x - p_quat.y * p_quat.y - p_quat.z * p_quat.z);
}

Quaternion Quaternion::Conjugate(const Quat& p_quat)
{
	return Quat(p_quat.w, -p_quat.x, -p_quat.y, -p_quat.z);
}

Quaternion Quaternion::Inverse(const Quat& p_quat)
{
	return Conjugate(p_quat) / Dot(p_quat, p_quat);
}

Quaternion operator+(const Quaternion& p_quat, const Quaternion& p_quatB)
{
	return Quaternion(p_quat) += p_quatB;
}

Quaternion operator*(const Quaternion& p_quat, const Quaternion& p_quatB)
{
	return Quaternion(p_quat) *= p_quatB;
}

// transforming a vector by quaternion
Vector3 operator*(const Quaternion& p_quat, const Vector3& p_vec)
{
	const Vector3 quatVec(p_quat.x, p_quat.y, p_quat.z);
	const Vector3 uv(Vector3::Cross(quatVec, p_vec));
	const Vector3 uuv(Vector3::Cross(quatVec, uv));
	
	return p_vec + ((uv * p_quat.w) + uuv) * 2.f;
}

Vector3 operator*(const Vector3& p_vec, const Quaternion& p_quat)
{
	return Quaternion::Inverse(p_quat) * p_vec;
}

Quaternion operator*(const Quaternion& p_quat, const float& s)
{
	return Quaternion(p_quat.w * s, p_quat.x * s, p_quat.y * s, p_quat.z * s);
}

Quaternion operator/(const Quaternion& p_quat, const float& s)
{
	return Quaternion(p_quat.w / s, p_quat.x / s, p_quat.y / s, p_quat.z / s);
}

Quaternion operator*(const float& s, const Quaternion& p_quat)
{
	return p_quat * s;
}

Matrix3x3 Quaternion::ToMatrix3x3(const Quat& p_quat)
{
	Matrix3x3 res(1);
	float xx(p_quat.x * p_quat.x);
	float yy(p_quat.y * p_quat.y);
	float zz(p_quat.z * p_quat.z);
	float xz(p_quat.x * p_quat.z);
	float xy(p_quat.x * p_quat.y);
	float yz(p_quat.y * p_quat.z);
	float wx(p_quat.w * p_quat.x);
	float wy(p_quat.w * p_quat.y);
	float wz(p_quat.w * p_quat.z);

	res[0][0] = 1.f - 2.f * (yy + zz);
	res[0][1] = 2.f * (xy + wz);
	res[0][2] = 2.f * (xz - wy);

	res[1][0] = 2.f * (xy - wz);
	res[1][1] = 1.f - 2.f * (xx + zz);
	res[1][2] = 2.f * (yz + wx);
	
	res[2][0] = 2.f * (xz + wy);
	res[2][1] = 2.f * (yz - wx);
	res[2][2] = 1.f - 2.f * (xx + yy);

	return res;
}

Matrix4x4 Quaternion::ToMatrix4x4(const Quat& p_quat)
{
	return Matrix4x4(ToMatrix3x3(p_quat));
}

Quaternion Quaternion::FromMatrix3x3(const Matrix3x3& m)
{
	float fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
	float fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
	float fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
	float fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

	int biggestIndex = 0;
	float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
	if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex = 1;
	}
	if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex = 2;
	}
	if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex = 3;
	}

	float biggestVal = sqrt(fourBiggestSquaredMinus1 + 1.f) * 0.5f;
	float mult = 0.25f / biggestVal;

	Quat Result(0);
	switch (biggestIndex)
	{
	case 0:
		Result.w = biggestVal;
		Result.x = (m[1][2] - m[2][1]) * mult;
		Result.y = (m[2][0] - m[0][2]) * mult;
		Result.z = (m[0][1] - m[1][0]) * mult;
		break;
	case 1:
		Result.w = (m[1][2] - m[2][1]) * mult;
		Result.x = biggestVal;
		Result.y = (m[0][1] + m[1][0]) * mult;
		Result.z = (m[2][0] + m[0][2]) * mult;
		break;
	case 2:
		Result.w = (m[2][0] - m[0][2]) * mult;
		Result.x = (m[0][1] + m[1][0]) * mult;
		Result.y = biggestVal;
		Result.z = (m[1][2] + m[2][1]) * mult;
		break;
	case 3:
		Result.w = (m[0][1] - m[1][0]) * mult;
		Result.x = (m[2][0] + m[0][2]) * mult;
		Result.y = (m[1][2] + m[2][1]) * mult;
		Result.z = biggestVal;
		break;

	default:					// Silence a -Wswitch-default warning in GCC. Should never actually get here. Assert is just for sanity.
		assert(false);
		break;
	}
	return Result;
}

Quaternion Quaternion::FromMatrix4x4(const Matrix4x4& m)
{
	return Quat(FromMatrix3x3(m));
}

