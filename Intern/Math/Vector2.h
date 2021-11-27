#pragma once

#define NO_FDECL
#include "GLCore.h"
#include "Math/Math.h"
#include <functional>

struct GLIB_API Vector2
{
	Vector2() :x(0), y(0) {};
	Vector2(float p_x, float p_y) : x(p_x), y(p_y) {}
	Vector2(const Vector2& p_vec) : x(p_vec.x), y(p_vec.y) {}
	Vector2(const struct Vector3& p_vec);

	static inline Vector2 Normalize(const Vector2& p_vec);
	static inline Vector2 Abs(Vector2& p_vec);
	static inline float Length(const Vector2& p_vec);
	static inline float LengthSq(const Vector2& p_vec);
	static inline float Distance(const Vector2& p_vecA, const Vector2& p_vecB);
	static inline float DistanceSq(const Vector2& p_vecA, const Vector2& p_vecB);
	static inline Vector2 Direction(const Vector2& p_vecA, const Vector2& p_vecB);
	static inline float AngleBetween(const Vector2& p_vecA, const Vector2& p_vecB);
	static inline Vector2 AngleToDir(float p_angleRad);
	static inline float DirToAngle(const Vector2& p_dir);
	static inline Vector2 Rotate(const Vector2& p_vec, const float angleRad);
	static inline float AngleToPoint(const Vector2& p_vecA, const Vector2& p_vec);
	static inline float Dot(const Vector2& p_vecA, const Vector2& p_vecB);
	static inline float Cross(const Vector2& p_vecA, const Vector2& p_vecB);
	static inline Vector2 Clamp(const Vector2& p_vec);
	static inline Vector2 Lerp(const Vector2& p_vecA, const Vector2& p_vecB, const float p_delta);
	static inline Vector2 Slerp(const Vector2& p_vecA, const Vector2& p_vecB, const float p_delta);
	static inline Vector2 MoveTowards(const Vector2& p_vecA, const Vector2& p_vecB, const float p_delta);
	static inline Vector2 Orthogonal(const Vector2& p_vec);
	static inline Vector2 Sign(const Vector2& p_vec);
	static inline Vector2 Floor(const Vector2& p_vec);
	static inline Vector2 Ceil(const Vector2& p_vec);
	static inline Vector2 Round(const Vector2& p_vec);
	inline float& operator[](int p_axis) { return p_axis ? x : y; }
	inline Vector2 operator=(const Vector2& p_vec)
	{
		this->x = p_vec.x;
		this->y = p_vec.y;
		return *this;
	}

	Vector2 operator=(const struct Vector3& p_vec);
	inline Vector2 operator+(const Vector2& p_v) const;
	inline void operator+=(const Vector2& p_v);
	inline Vector2 operator-(const Vector2& p_v) const;
	inline void operator-=(const Vector2& p_v);
	inline void operator*=(const float& rvalue);
	
	friend inline Vector2 operator*(float p_scalar, const Vector2& p_vec);
	friend inline Vector2 operator*(double p_scalar, const Vector2& p_vec);
	friend inline Vector2 operator*(int p_scalar, const Vector2& p_vec);
	
	friend inline Vector2 operator*( const Vector2& p_vec, float p_scalar);
	friend inline Vector2 operator*( const Vector2& p_vec, double p_scalar);
	friend inline Vector2 operator*( const Vector2& p_vec, int p_scalar);
	friend inline Vector2 operator*(const Vector2& p_vec, const Vector2& p_vecB);
	
	inline void operator*=(const Vector2& rvalue) { *this = *this * rvalue; }
	inline Vector2 operator/(const Vector2& p_v1) const;
	inline Vector2 operator/(const float& rvalue) const;
	inline void operator/=(const float& rvalue);
	inline void operator/=(const Vector2& rvalue) { *this = *this / rvalue; }
	inline Vector2 operator-() const;
	inline bool operator==(const Vector2& p_vec2) const;
	inline bool operator!=(const Vector2& p_vec2) const;

	inline bool operator<(const Vector2& p_vec2) const { return x == p_vec2.x ? (y < p_vec2.y) : (x < p_vec2.x); }
	inline bool operator>(const Vector2& p_vec2) const { return x == p_vec2.x ? (y > p_vec2.y) : (x > p_vec2.x); }
	inline bool operator<=(const Vector2& p_vec2) const { return x == p_vec2.x ? (y <= p_vec2.y) : (x < p_vec2.x); }
	inline bool operator>=(const Vector2& p_vec2) const { return x == p_vec2.x ? (y >= p_vec2.y) : (x > p_vec2.x); }

	union 
	{
		struct
		{
			float width;
			float height;
		};

		struct
		{
			float x, y;
		};
	};

};

typedef Vector2 Point2;
typedef Vector2 Size2;


float Vector2::Length(const Vector2& p_vec)
{
	return Math::Sqrt(Vector2::LengthSq(p_vec));
}

float Vector2::LengthSq(const Vector2& p_vec)
{
	return Vector2::Dot(p_vec, p_vec);
}

Vector2 Vector2::Normalize(const Vector2& p_vec)
{
	float length = LengthSq(p_vec);
	float _x = p_vec.x;
	float _y = p_vec.y;
	if (length != 0)
	{
		_x /= length;
		_y /= length;
	}

	return Vector2(_x, _y);
}

float Vector2::Distance(const Vector2& p_vecA, const Vector2& p_vecB)
{
	return Length((p_vecA - p_vecB));
}

float Vector2::DistanceSq(const Vector2& p_vecA, const Vector2& p_vecB)
{
	return LengthSq((p_vecA - p_vecB));
}


Vector2 Vector2::Direction(const Vector2& p_vecA, const Vector2& p_vecB)
{
	Vector2 res = (p_vecB - p_vecA);
	Normalize(res);
	return res;
}

Vector2 Vector2::Rotate(const Vector2& p_vec, const float p_angleRad)
{
	float posY = Math::Sin(p_angleRad);
	float posX = Math::Cos(p_angleRad);
	Vector2 res(p_vec.x * posX - p_vec.y * posY, p_vec.x * posY + p_vec.y * posX);
	return res;
}

float Vector2::AngleBetween(const Vector2& p_vecA, const Vector2& p_vecB)
{
	// making vectors unit vectors so wouldn't divide by magnitude
	return Math::ACos(Dot(Normalize(p_vecA), Normalize(p_vecB)));
}

//float Vector2::AngleBetween(const Vector2& p_vecA, const Vector2& p_vecB)
//{
//	float dotP = Dot(p_vecA, p_vecB);
//	float mag = Math::Sqrt(Dot(p_vecA, p_vecA)) * Math::Sqrt(Dot(p_vecB, p_vecB));
//
//	return Math::ACos(dotP / mag);
//}

Vector2 Vector2::AngleToDir(float p_angleRad)
{
	return Vector2(Math::Cos(p_angleRad), Math::Sin(p_angleRad));
}

float Vector2::DirToAngle(const Vector2& p_vec)
{
	return Math::ATan2(p_vec.y, p_vec.x);
}


float Vector2::AngleToPoint(const Vector2& p_vec, const Vector2& p_point)
{
	return Math::ATan2(p_vec.y - p_point.y, p_vec.x - p_point.x);
}

float Vector2::Dot(const Vector2& p_vecA, const Vector2& p_vecB)
{
	return p_vecA.x * p_vecB.x + p_vecA.y * p_vecB.y;
}

float Vector2::Cross(const Vector2& p_vecA, const Vector2& p_vecB)
{
	return p_vecA.x * p_vecB.x - p_vecA.y * p_vecB.y;
}


Vector2 Vector2::Clamp(const Vector2& p_vec)
{
	return Vector2(Math::ClampNeg1to1(p_vec.x), Math::ClampNeg1to1(p_vec.y));
}


Vector2 Vector2::MoveTowards(const Vector2& p_from, const Vector2& p_to, const float p_delta)
{
	return Vector2(Math::MoveTowards(p_from.x, p_to.x, p_delta), Math::MoveTowards(p_from.y, p_to.y, p_delta));
}

Vector2 Vector2::Orthogonal(const Vector2& p_vec)
{
	return Vector2(p_vec.y, -p_vec.x);
}

Vector2 Vector2::Lerp(const Vector2& p_vecA, const Vector2& p_vecB, const float p_delta)
{
	return Vector2(Math::Lerp(p_vecA.x, p_vecB.x, p_delta), Math::Lerp(p_vecA.y, p_vecB.y, p_delta));
}

Vector2 Vector2::Slerp(const Vector2& p_vecA, const Vector2& p_vecB, const float p_delta)
{
	Vector2 vec(Normalize(p_vecA));
	Vector2 vec2(Normalize(p_vecB));

	float theta = AngleBetween(vec, vec2);
	return Rotate(p_vecA, theta * p_delta);
}

Vector2 Vector2::Sign(const Vector2& p_vec) {
	return Vector2(float(Math::Sign(p_vec.x)), float(Math::Sign(p_vec.y)));
}

Vector2 Vector2::Floor(const Vector2& p_vec) {
	return Vector2(Math::Floor(p_vec.x), Math::Floor(p_vec.y));
}

Vector2 Vector2::Ceil(const Vector2& p_vec) {
	return Vector2(Math::Ceil(p_vec.x), Math::Ceil(p_vec.y));
}

Vector2 Vector2::Round(const Vector2& p_vec) {
	return Vector2(Math::Round(p_vec.x), Math::Round(p_vec.y));
}



inline Vector2 operator*(float p_scalar, const Vector2& p_vec) {
	return Vector2(p_vec.x * p_scalar, p_vec.x * p_scalar);
}

inline Vector2 operator*(double p_scalar, const Vector2& p_vec) {
	return Vector2(float(p_vec.x * p_scalar), float(p_vec.x * p_scalar));
}

inline Vector2 operator*(int p_scalar, const Vector2& p_vec) {
	return Vector2(float(p_vec.x * p_scalar), float(p_vec.x * p_scalar));
}

inline Vector2 operator*(const Vector2& p_vec, float p_scalar) {
	return Vector2(p_vec.x * p_scalar, p_vec.x * p_scalar);
}

inline Vector2 operator*(const Vector2& p_vec, double p_scalar) {
	return Vector2(float(p_vec.x * p_scalar), float(p_vec.x * p_scalar));
}

inline Vector2 operator*(const Vector2& p_vec, int p_scalar) {
	return Vector2(float(p_vec.x * p_scalar), float(p_vec.x * p_scalar));
}

inline Vector2 operator*(const Vector2& p_vec, const Vector2& p_vecB)
{
	return Vector2(p_vec.x * p_vecB.x, p_vec.y * p_vecB.y);
}

inline Vector2 Vector2::operator+(const Vector2& p_v) const {
	return Vector2(x + p_v.x, y + p_v.y);
}

inline void Vector2::operator+=(const Vector2& p_v) {
	x += p_v.x;
	y += p_v.y;
}

inline Vector2 Vector2::operator-(const Vector2& p_v) const {
	return Vector2(x - p_v.x, y - p_v.y);
}

inline void Vector2::operator-=(const Vector2& p_v) {
	x -= p_v.x;
	y -= p_v.y;
}

inline void Vector2::operator*=(const float& rvalue) {
	x *= rvalue;
	y *= rvalue;
}

inline Vector2 Vector2::operator/(const Vector2& p_v1) const {
	return Vector2(x / p_v1.x, y / p_v1.y);
}

inline Vector2 Vector2::operator/(const float& rvalue) const {
	return Vector2(x / rvalue, y / rvalue);
}

inline void Vector2::operator/=(const float& rvalue) {
	x /= rvalue;
	y /= rvalue;
}

inline Vector2 Vector2::operator-() const {
	return Vector2(-x, -y);
}

inline bool Vector2::operator==(const Vector2& p_vec2) const {
	return x == p_vec2.x && y == p_vec2.y;
}

inline bool Vector2::operator!=(const Vector2& p_vec2) const {
	return x != p_vec2.x || y != p_vec2.y;
}

namespace std
{
	template<>
	struct hash<Vector2>
	{
		size_t operator()(const Vector2& p_vec) const
		{
			return hash<float>()(p_vec.x) ^ (hash<float>()(p_vec.y) * 997u);
		}
	};
}
