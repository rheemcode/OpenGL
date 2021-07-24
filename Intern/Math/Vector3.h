#pragma once

#define NO_FDECL
#include "Math/Math.h"
#include "Vector2.h"
#include "SimpleVec.h"
#include <functional>


struct Vector3
{
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float p_x, float p_y, float p_z) 
			:x (p_x), y(p_y), z(p_z) {}	
	Vector3(const struct Vector2& p_vec);
	Vector3(const Vector3& p_vec) 
		: x(p_vec.x), y(p_vec.y), z(p_vec.z) {}

	inline float operator[](int p_axis) const { return coord[p_axis]; }
	inline float& operator[](int p_axis) { return coord[p_axis]; }

	static inline Vector3 Abs(const Vector3& p_vec);
	static inline Vector3 Floor(const Vector3& p_vec);
	static inline Vector3 Sign(const Vector3& p_vec);
	static inline Vector3 Round(const Vector3& p_vec);
	static inline Vector3 Ceil(const Vector3& p_vec);
	static inline Vector3 Cos(const Vector3& p_vec);
	static inline Vector3 Sin(const Vector3& p_val);
	static inline float Length(const Vector3& p_vec);
	static inline float LengthSq(const Vector3& p_vec);
	static inline Vector3 Normalize(const Vector3& p_vec);
	static inline Vector3 Inverse(const Vector3& p_vec);
	static inline Vector3 Zero();
	static Vector3 Rotate(const Vector3& p_vec,  float p_angleRad);
	static Vector3 RotateX(const Vector3& p_vec, float p_angleRad);
	static Vector3 RotateY(const Vector3& p_vec, float p_angleRad);
	static Vector3 RotateZ(const Vector3& p_vec, float p_angleRad);
	static Vector3 Lerp(const Vector3& p_from, const Vector3& p_to, const float p_delta);
	//static Vector3 Slerp(const Vector3& p_from, const Vector3& p_to, const float p_delta);
	static Vector3 MoveTowards(const Vector3& p_from, const Vector3& p_to, const float p_delta);
	static inline Vector3 Cross(const Vector3& p_vecA, const Vector3& p_vecB);
	static inline float Dot(const Vector3& p_vecA, const Vector3& p_vecB);
	static inline float Distance(const Vector3& p_vecA, const Vector3& p_vecB);
	static inline float DistanceSq(const Vector3& p_vecA, const Vector3& p_vecB);
	static inline float AngleBetween(const Vector3& p_vecA, const Vector3& p_vecB);
	static inline float SignedAngleBetween(const Vector3& p_vecA, const Vector3& p_vecB, const Vector3& p_axis);
	static inline Vector3 Direction(const Vector3& p_vec, const Vector3& p_vecB);
	static inline Vector3 Project(const Vector3& p_vec, const Vector3& p_to);
	static inline Vector3 Slide(const Vector3& p_vec, const Vector3& p_normal);
	static inline Vector3 Bounce(const Vector3& p_vec, const Vector3& p_normal);
	static inline Vector3 Reflect(const Vector3& p_vec, const Vector3& p_normal);

	static inline Vector3 Forward();
	static inline Vector3 Right();
	static inline Vector3 Up();

	inline Vector3& operator+=(const Vector3& p_v);
	inline Vector3 operator+(const Vector3& p_v) const;
	inline Vector3& operator-=(const Vector3& p_v);
	inline Vector3 operator-(const Vector3& p_v) const;
	inline Vector3& operator*=(const Vector3& p_v);
	inline Vector3 operator*(const Vector3& p_v) const;
	inline Vector3& operator/=(const Vector3& p_v);
	inline Vector3 operator/(const Vector3& p_v) const;
	inline Vector3& operator=(const SimpleVec4& p_v);
	inline Vector3& operator*=(float p_scalar);
	inline Vector3 operator*(float p_scalar) const;
	friend Vector3 operator*(const float& p_scalar, const Vector3& p_vec);
	inline Vector3& operator/=(float p_scalar);
	inline Vector3 operator/(float p_scalar) const;

	inline Vector3 operator-() const;

	inline bool operator==(const Vector3& p_v) const;
	inline bool operator!=(const Vector3& p_v) const;
	inline bool operator<(const Vector3& p_v) const;
	inline bool operator<=(const Vector3& p_v) const;
	inline bool operator>(const Vector3& p_v) const;
	inline bool operator>=(const Vector3& p_v) const;

	union {
		struct {
			float x;
			float y;
			float z;
		};

		float coord[3] = { 0 };
	};
};


inline Vector3 Vector3::Abs(const Vector3& p_vec)
{
	return Vector3(Math::Abs(p_vec.x), Math::Abs(p_vec.y), Math::Abs(p_vec.z));
}

inline Vector3 Vector3::Sign(const Vector3& p_vec)
{
	return Vector3(float(Math::Sign(p_vec.x)), float(Math::Sign(p_vec.y)), float(Math::Sign(p_vec.z)));
}

inline Vector3 Vector3::Floor(const Vector3& p_vec)
{
	return Vector3(Math::Floor(p_vec.x), Math::Floor(p_vec.y), Math::Floor(p_vec.z));
}

inline Vector3 Vector3::Ceil(const Vector3& p_vec)
{
	return Vector3(Math::Ceil(p_vec.x), Math::Ceil(p_vec.y), Math::Ceil(p_vec.z));
}

inline Vector3 Vector3::Round(const Vector3& p_vec)
{
	return Vector3(Math::Round(p_vec.x), Math::Round(p_vec.y), Math::Round(p_vec.z));
}

inline Vector3 Vector3::Cos(const Vector3& p_val)
{
	return Vector3(Math::Cos(p_val.x), Math::Cos(p_val.y), Math::Cos(p_val.z));
}

inline Vector3 Vector3::Sin(const Vector3& p_val)
{
	return Vector3(Math::Sin(p_val.x), Math::Sin(p_val.y), Math::Sin(p_val.z));
}

inline Vector3 Vector3::Cross(const Vector3& p_a, const Vector3& p_b)
{
	Vector3 temp;
	temp.x = p_a.y * p_b.z - p_a.z * p_b.y;
	temp.y = p_a.z * p_b.x - p_a.x * p_b.z;
	temp.z = p_a.x * p_b.y - p_a.y * p_b.x;
	return temp;
}

inline float Vector3::Dot(const Vector3& p_a, const Vector3& p_b)
{
	return p_a.x * p_b.x + p_a.y * p_b.y + p_a.z * p_b.z;
}

inline Vector3 Vector3::Normalize(const Vector3& p_vec)
{
	float length = Vector3::Length(p_vec);
	return Vector3(p_vec.x / length, p_vec.y / length, p_vec.z / length);
}


inline float Vector3::Length(const Vector3& p_vec)
{
	return Math::Sqrt(Vector3::Dot(p_vec, p_vec));
}

inline float Vector3::LengthSq(const Vector3& p_vec)
{
	return Vector3::Dot(p_vec, p_vec);
}

inline Vector3 Vector3::Direction(const Vector3& p_from, const Vector3& p_to)
{
	return Normalize(p_to - p_from);
}

inline float Vector3::Distance(const Vector3& p_from, const Vector3& p_to)
{
	return Vector3::Length((p_from - p_to));
}

inline float Vector3::DistanceSq(const Vector3& p_from, const Vector3& p_to)
{
	return LengthSq((p_from - p_to));
}


inline float Vector3::AngleBetween(const Vector3& p_vecA, const Vector3& p_vecB)
{
	return Math::ACos(Dot(Normalize(p_vecA), Normalize(p_vecB)));
}

inline float Vector3::SignedAngleBetween(const Vector3& p_vecA, const Vector3& p_vecB, const Vector3& p_axis)
{
	Vector3 crossTo = Cross(p_vecA, p_vecB);
	float unsignedAngle = AngleBetween(p_vecA, p_vecB);

	float sign = Dot(crossTo, p_axis);
	return (sign < 0) ? -unsignedAngle : unsignedAngle;

}

inline Vector3 Vector3::Project(const Vector3& p_vec, const Vector3& p_to)
{
	return p_to * (Vector3::Dot(p_vec, p_to)) / Vector3::LengthSq(p_to);
}

inline Vector3 Vector3::Slide(const Vector3& p_vec, const Vector3& p_normal)
{
	return p_vec - p_normal * Dot(p_vec, p_normal);
}

inline Vector3 Vector3::Bounce(const Vector3& p_vec, const Vector3& p_normal)
{
	return -Reflect(p_vec, p_normal);
}

inline Vector3 Vector3::Reflect(const Vector3& p_vec, const Vector3& p_normal)
{
	return  (p_normal * 2.0f) * Dot(p_vec, p_normal) - p_vec;
}

inline Vector3 Vector3::Inverse(const Vector3& p_vec)
{
	return Vector3(1 / p_vec.x, 1 / p_vec.y, 1 / p_vec.z);
}

inline Vector3 Vector3::Zero()
{
	return Vector3(0, 0, 0);
}


inline Vector3 Vector3::Forward()
{
	return Vector3(0, 0, -1.f);
}
inline Vector3 Vector3::Right()
{
	return Vector3(1.f, 0, 0);
}
inline Vector3 Vector3::Up()
{
	return Vector3(0, 1.f, 0);
}

inline Vector3& Vector3::operator+=(const Vector3& p_v) {
	x += p_v.x;
	y += p_v.y;
	z += p_v.z;
	return *this;
}

inline Vector3 Vector3::operator+(const Vector3& p_v) const {
	return Vector3(x + p_v.x, y + p_v.y, z + p_v.z);
}

inline Vector3& Vector3::operator-=(const Vector3& p_v) {
	x -= p_v.x;
	y -= p_v.y;
	z -= p_v.z;
	return *this;
}

inline Vector3 Vector3::operator-(const Vector3& p_v) const {
	return Vector3(x - p_v.x, y - p_v.y, z - p_v.z);
}

inline Vector3& Vector3::operator*=(const Vector3& p_v) {
	x *= p_v.x;
	y *= p_v.y;
	z *= p_v.z;
	return *this;
}

inline Vector3 Vector3::operator*(const Vector3& p_v) const {
	return Vector3(x * p_v.x, y * p_v.y, z * p_v.z);
}

inline Vector3& Vector3::operator/=(const Vector3& p_v) {
	x /= p_v.x;
	y /= p_v.y;
	z /= p_v.z;
	return *this;
}

inline Vector3 Vector3::operator/(const Vector3& p_v) const {
	return Vector3(x / p_v.x, y / p_v.y, z / p_v.z);
}


inline Vector3& Vector3::operator*=(float p_scalar) {
	x *= p_scalar;
	y *= p_scalar;
	z *= p_scalar;
	return *this;
}


inline Vector3 Vector3::operator*(float p_scalar) const {
	return Vector3(x * p_scalar, y * p_scalar, z * p_scalar);
}

inline Vector3& Vector3::operator/=(float p_scalar) {
	x /= p_scalar;
	y /= p_scalar;
	z /= p_scalar;
	return *this;
}

inline Vector3 Vector3::operator/(float p_scalar) const {
	return Vector3(x / p_scalar, y / p_scalar, z / p_scalar);
}

inline Vector3 Vector3::operator-() const {
	return Vector3(-x, -y, -z);
}

inline bool Vector3::operator==(const Vector3& p_v) const {
	return x == p_v.x && y == p_v.y && z == p_v.z;
}

inline bool Vector3::operator!=(const Vector3& p_v) const {
	return x != p_v.x || y != p_v.y || z != p_v.z;
}

inline bool Vector3::operator<(const Vector3& p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			return z < p_v.z;
		}
		return y < p_v.y;
	}
	return x < p_v.x;
}

inline bool Vector3::operator>(const Vector3& p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			return z > p_v.z;
		}
		return y > p_v.y;
	}
	return x > p_v.x;
}

inline bool Vector3::operator<=(const Vector3& p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			return z <= p_v.z;
		}
		return y < p_v.y;
	}
	return x < p_v.x;
}

inline bool Vector3::operator>=(const Vector3& p_v) const {
	if (x == p_v.x) {
		if (y == p_v.y) {
			return z >= p_v.z;
		}
		return y > p_v.y;
	}
	return x > p_v.x;
}

inline Vector3& Vector3::operator=(const SimpleVec4& p_v)
{
	this->x = p_v.x;
	this->y = p_v.y;
	this->z = p_v.z;
	return *this;
}

namespace std
{
	template<>
	struct hash<Vector3>
	{
		size_t operator()(const Vector3& p_vec) const
		{
			return hash<float>()(p_vec.x) ^ (hash<float>()(p_vec.y) * 997u) ^ (hash<float>()(p_vec.z) * 999983u);
		}
	};
}
