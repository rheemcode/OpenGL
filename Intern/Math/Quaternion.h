#pragma once
#include "Matrix3x3.h"
#include "Matrix4x4.h"

struct GLIB_API Quaternion
{
	union
	{
		struct { float x, y, z, w; };
		float component[4] = { 0 };
	};
	typedef Quaternion Quat;

	Quaternion()
		: x(0), y(0), z(0), w(1) {}
	Quaternion(float x)
		: x(0), y(0), z(0), w(1) {}
	Quaternion(const Quat& p_quat)
		: x(p_quat.x), y(p_quat.y), z(p_quat.z), w(p_quat.w){}
	Quaternion(const struct Vector3& p_eulerAngles);
	Quaternion(const float& p_s, const struct Vector3& p_eulerAngles);
	Quaternion(const float& w, const float& x, const float& y, const float& z)
		: x(x), y(y), z(z), w(w) {}
	explicit Quaternion(const class Matrix4x4& p_mat);
	explicit Quaternion(const class Matrix3x3& p_mat);

	Quat& operator=(const Quat& p_quat);
	Quat& operator+=(const Quat& p_quat);
	Quat& operator-=(const Quat& p_quat);
	Quat& operator*=(const Quat& p_quat);
	Quat& operator*=(const float& p_s);
	Quat& operator/=(const float& p_s);

	friend bool operator==(const Quat& q1, const Quat& q2) { return (q1.x == q2.x) && (q1.y == q2.y) && (q1.z == q2.z) && (q1.w == q2.w);}
	friend bool operator!=(const Quat& q1, const Quat& q2) { return (q1.x != q2.x) || (q1.y != q2.y) || (q1.z != q2.z) || (q1.w != q2.w); }

	friend Quaternion operator-(const Quaternion& p_quatB);
	friend Quaternion operator/(const Quaternion& p_quat, const Quaternion& p_quatB);
	friend Quaternion operator+(const Quaternion& p_quat, const Quaternion& p_quatB);
	friend Quaternion operator*(const Quaternion& p_quat, const Quaternion& p_quatB);
	friend Quaternion operator*(const float& s, const Quaternion& p_quat);
	friend Quaternion operator*(const Quaternion& p_quat, const float& s);
	friend Vector3 operator*( const Quaternion& p_quat, const struct Vector3& s);
	friend Vector3 operator*(const struct Vector3& p_vec, const Quaternion& p_quat);
	friend Quaternion operator/(const Quaternion& p_quat, const float& s);

	static float Length(const Quat& p_quat);
	static Quat Normalize(const Quat& p_quat);
	static float Dot(const Quat& a, const Quat& b);
	static Quat Cross(const Quat& p_quat, const Quat& p_quatB);
	static Quat Lerp(const Quat& p_quat, const Quat& p_quatB, const float& p_w);
	static Quat NLerp(const Quat& p_quat, const Quat& p_quatB, const float& p_w);
	static Quat Slerp(const Quat& p_quat, const Quat& p_quatB, const float& p_w);
	static Quat Mix(const Quat& p_quat, const Quat& p_quatB, const float& p_w);
	static Quat Conjugate(const Quat& p_quat);
	static Quat Inverse(const Quat& p_quat);
	static Quat Rotate(const Quat& p_quat, const float& p_theta, const struct Vector3& p_axis);
	static Quat RotateX(const float& p_theta);
	static Quat RotateY(const float& p_theta);
	static Quat RotateZ( const float& p_theta);
	static Quat RotatePoint(const struct Vector3& p_point, const Quat& p_quat);
	static Quat LookRotation(const struct Vector3& p_forward, const struct Vector3& p_up);
	static Quat FromEulerAngles(const struct Vector3& p_eulerAngle);
	static Vector3 EulerAngles(const Quat& p_quat);
	static float Roll(const Quat& p_quat);
	static float Pitch(const Quat& p_quat);
	static float Yaw(const Quat& p_quat);
	static float Angle(const Quat& p_quat);
	static Vector3 Axis(const Quat& p_quat);
	static Quaternion AngleAxis(const float p_theta, const struct Vector3& p_axis);

	static Matrix3x3 ToMatrix3x3(const Quat& p_quat);
	static Matrix4x4 ToMatrix4x4(const Quat& p_quat);
	static Quat FromMatrix4x4(const class Matrix4x4& p_quat);
	static Quat FromMatrix3x3(const class Matrix3x3& p_quat);
};

