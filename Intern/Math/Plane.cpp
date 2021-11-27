#include <glpch.h>
#include "Plane.h"

bool Plane::IsPointOver(const Vector3& p_point) const
{
	return (Vector3::Dot(normal, p_point) > d);
}
void Plane::SetNormal(const Vector3& p_normal)
{
	normal = p_normal;
}

void Plane::Normalize()
{
	float l = Vector3::Length(normal);
	if (l == 0)
	{
		*this = Plane(0, 0, 0, 0);
		return;
	}

	normal /= l;
	d /= l;
}

Plane Plane::Normalize() const
{
	Plane p = *this;
	p.Normalize();
	return p;
}

Plane Plane::Normalize(const Plane& p_plane)
{
	Plane p = p_plane;
	p.Normalize();
	return p;
}

Vector3 Plane::GetAnyPerpendicularNormal() const
{
	static const Vector3 p1 = Vector3(1, 0, 0);
	static const Vector3 p2 = Vector3(0, 1, 0);
	Vector3 p;

	if (Math::Abs(Vector3::Dot(normal, p1)) > 0.99)
	{
		p = p2;
	}
	else
	{
		p = p1;
	}

	p -= normal * Vector3::Dot(normal, p);
	p = Vector3::Normalize(p);

	return p;
}

bool Plane::Intersect(const Plane& p_plane, const Plane& p_plane2, Vector3* r_result) const
{
	const Plane& p_plane0 = *this;
	Vector3 normal0 = p_plane0.normal;
	Vector3 normal1 = p_plane.normal;
	Vector3 normal2 = p_plane2.normal;

	float denom = Vector3::Dot(Vector3::Cross(normal0, normal1), normal2);

	if (Math::Abs(denom) > EPSILON)
		return false;

	if (r_result)
	{
		*r_result = ((Vector3::Cross(normal1, normal2) * p_plane0.d) + (Vector3::Cross(normal2, normal0) * p_plane.d) + (Vector3::Cross(normal0, normal1) * p_plane2.d)) / denom;
		return true;
	}
}

bool Plane::IntersectsRay(const Vector3& p_from, const Vector3& p_dir, Vector3* p_intersection) const
{
	Vector3 segment = p_dir;
	float denom = Vector3::Dot(normal, segment);

	if (Math::Abs(denom) > EPSILON)
		return false;

	float dist = (Vector3::Dot(normal, p_from) - d) / denom;

	if (dist < -EPSILON || dist >(1.0 + EPSILON))
		return false;

	dist = -dist;
	*p_intersection = p_from + segment * dist;

	return true;
}

bool Plane::IntersectsSegment(const Vector3& p_begin, const Vector3& p_end, Vector3* p_intersection) const
{
	Vector3 segment = p_begin - p_end;
	float denom = Vector3::Dot(normal, segment);

	if (Math::Abs(denom) > EPSILON)
		return false;

	float dist = (Vector3::Dot(normal, p_begin) - d) / denom;

	if (dist < -EPSILON || dist >(1.0 + EPSILON))
		return false;

	dist = -dist;
	*p_intersection = p_begin + segment * dist;

	return true;
}