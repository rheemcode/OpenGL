#pragma once

#include "Vector3.h"

class GLIB_API Plane
{
public:
	Vector3 normal;
	float d = 0;

	void SetNormal(const Vector3& p_normal);
	__forceinline Vector3 GetNormal() const { return normal; }

	void Normalize();
	static Plane Normalize(const Plane& p_plane);
	Plane Normalize() const;

	__forceinline Vector3 Center() const { return normal * d; }
	Vector3 GetAnyPerpendicularNormal() const;

	bool IsPointOver(const Vector3& p_point) const;

	__forceinline float DistanceTo(const Vector3& p_point) const;
	__forceinline bool HasPoint(const Vector3& p_point, float p_epsilon = 0.0001f) const;

	bool Intersect(const Plane& p_plane, const Plane& p_plane2, Vector3* r_result = nullptr) const;
	bool IntersectsRay(const Vector3& p_from, const Vector3& p_dir, Vector3* p_intersection) const;
	bool IntersectsSegment(const Vector3& p_begin, const Vector3& p_end, Vector3* p_intersection) const;

	__forceinline Vector3 Project(const Vector3& p_point) const
	{
		return p_point - normal * DistanceTo(p_point);
	}

	Plane operator -() const { return Plane(-normal, -d); }
	bool IsEqualApprox(const Plane& p_plane) const;
	bool IsEqualAnySide(const Plane& p_plane) const;

	__forceinline bool operator==(const Plane& p_plane) const;
	__forceinline bool operator!=(const Plane& p_plane) const;

	__forceinline Plane() {}
	__forceinline Plane(float p_a, float p_b, float p_c, float p_d)
		: normal(p_a, p_b, p_c),
		  d(p_d){}

	__forceinline Plane(const Vector3& p_normal, float p_d);
	__forceinline Plane(const Vector3& p_point, const Vector3& p_normal);
	__forceinline Plane(const Vector3& p_point, const Vector3& p_point2, const Vector3& p_point3, int p_dir);

};



float Plane::DistanceTo(const Vector3& p_point) const
{
	return (Vector3::Dot(normal, p_point) - d);
}

bool Plane::HasPoint(const Vector3& p_point, float p_epsilon) const
{
	float distance = Vector3::Dot(normal, p_point) - d;
	distance = Math::Abs(distance);
	return (distance <= EPSILON);
}

Plane::Plane(const Vector3& p_normal, float p_d)
	: normal(p_normal), d(p_d) {}

Plane::Plane(const Vector3& p_point, const Vector3& p_normal)
	: normal(p_normal), d(Vector3::Dot(p_normal, p_point)) {}

Plane::Plane(const Vector3& p_point, const Vector3& p_point2, const Vector3& p_point3, int p_dir)
{
	if (p_dir == 0)
	{
		normal = Vector3::Cross((p_point - p_point3), p_point - p_point2);
	}
	else
	{
		normal = Vector3::Cross((p_point - p_point2),(p_point - p_point3));
	}

	Vector3::Normalize(normal);
	d = Vector3::Dot(normal, p_point);
}


bool Plane::operator==(const Plane& p_plane) const
{
	return normal == p_plane.normal && d == p_plane.d;
}

bool Plane::operator!=(const Plane& p_plane) const
{
	return normal != p_plane.normal || d != p_plane.d;
}