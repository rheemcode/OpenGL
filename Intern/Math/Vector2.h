#pragma once

#define NO_FDECL
#include "Math/Math.h"
#include <functional>

struct Vector2
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


inline float Vector2::Length(const Vector2& p_vec)
{
	return Math::Sqrt(Vector2::LengthSq(p_vec));
}

inline float Vector2::LengthSq(const Vector2& p_vec)
{
	return Vector2::Dot(p_vec, p_vec);
}


inline float Vector2::Dot(const Vector2& p_vecA, const Vector2& p_vecB)
{
	return p_vecA.x * p_vecB.x + p_vecA.y * p_vecB.y;
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
