#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Plane.h"


class GLIB_API AABB
{

public:
	Vector3 position;
	Vector3 size;

	float GetArea() const;
	__forceinline bool HasNoSurface() const
	{
		return (size.x <= 0 && size.y <= 0 && size.z <= 0);
	}	
	__forceinline bool HasNoArea() const
	{
		return (size.x <= 0 || size.y <= 0 || size.z <= 0);
	}

	const Vector3& GetPosition() const { return position; }
	void SetPosition(const Vector3& p_position) { position = p_position; }

	const Vector3& GetSize() const { return size; }
	void SetSize(const Vector3& p_size) { size = p_size; }

	bool operator==(const AABB& p_other) const;
	bool operator!=(const AABB& p_other) const;

	bool IsEqualApprox(const AABB& p_other) const;
	AABB Intersection(const AABB& p_aabb) const;
	__forceinline bool Intersects(const AABB& p_aabb) const;
	__forceinline bool IntersectsInclusive(const AABB& p_aabb) const;
	__forceinline bool Encloses(const AABB& p_aabb) const; // p_aabb is completely inside this

	AABB Merge(const AABB& p_other) const;
	void MergeWith(const AABB& p_other);
	bool IntersectsSegment(const Vector3& p_from, const Vector3& p_to, Vector3* r_clip = nullptr, Vector3* r_normal = nullptr) const;
	bool IntersectsRay(const Vector3& p_from, const Vector3& p_dir, Vector3* r_clip = nullptr, Vector3* r_normal = nullptr) const;
	__forceinline bool SmitsIntersectRay(const Vector3& p_from, const Vector3& p_dir, float t0, float t1) const;
	__forceinline bool IntersectsConvexShape(const Plane* p_planes, int p_plane_count, const Vector3* p_points, int p_point_count) const;
	__forceinline bool InsideConvexShape(const Plane* p_planes, int p_plane_count) const;
	bool IntersectsPlane(const Plane& p_plane) const;
	
	__forceinline bool HasPoint(const Vector3& p_point) const;

	Vector3 GetLongestAxis() const;
	int GetLongestAxisIndex() const;
	__forceinline float GetLongestAxisSize() const;

	Vector3 GetShortestAxis() const;
	int GetShortestAxisIndex() const;
	__forceinline float GetShortestAxisSize() const;

	AABB Grow(float p_scale) const;
	__forceinline void GrowBy(float p_scale);

	void GetEdge(int p_edge, Vector3& p_from, Vector3& p_to) const;
	__forceinline Vector3 GetEndpoint(int p_point) const;

	AABB Expand(const Vector3& p_vector) const;
	__forceinline void ExpandTo(const Vector3& p_vector);
	__forceinline void ProjectRangeInPlane(const Plane& plane, float& r_min, float& r_max) const;

	__forceinline void SetEnd(const Vector3& p_end)
	{
		size = p_end - position;
	}

	__forceinline Vector3 GetEnd() const { return position + size;  }
	__forceinline AABB Abs() const
	{
		return AABB(Vector3(position.x + MIN(size.x, 0), position.y + MIN(size.y, 0), position.z + MIN(size.z, 0)), Vector3::Abs(size));
	}

	AABB() {}
	AABB(const Vector3& p_pos, const Vector3& p_size)
		: position(p_pos), size(p_size){}

};


inline bool AABB::Intersects(const AABB& p_aabb) const
{
	if (position.x >= (p_aabb.position.x + p_aabb.size.x))
		return false;

	if ((position.x + size.x) <= p_aabb.position.x) 
		return false;
	
	if (position.y >= (p_aabb.position.y + p_aabb.size.y)) 
		return false;
	
	if ((position.y + size.y) <= p_aabb.position.y) 
		return false;
	
	if (position.z >= (p_aabb.position.z + p_aabb.size.z)) 
		return false;
	
	if ((position.z + size.z) <= p_aabb.position.z) 
		return false;
	

	return true;
}

inline bool AABB::IntersectsInclusive(const AABB& p_aabb) const
{
	if (position.x > (p_aabb.position.x + p_aabb.size.x))
		return false;

	if ((position.x + size.x) < p_aabb.position.x) 
		return false;
	
	if (position.y > (p_aabb.position.y + p_aabb.size.y)) 
		return false;
	
	if ((position.y + size.y) < p_aabb.position.y) 
		return false;
	
	if (position.z > (p_aabb.position.z + p_aabb.size.z)) 
		return false;
	
	if ((position.z + size.z) <=p_aabb.position.z) 
		return false;
	

	return true;
}

inline bool AABB::Encloses(const AABB& p_aabb) const
{
	Vector3 src_min = position;
	Vector3 src_max = position + size;
	Vector3 dst_min = p_aabb.position;
	Vector3 dst_max = p_aabb.position + p_aabb.size;

	return (
		(src_min.x <= dst_min.x) &&
		(src_max.x > dst_max.x) &&
		(src_min.y <= dst_min.y) &&
		(src_max.y > dst_max.y) &&
		(src_min.z <= dst_min.z) &&
		(src_max.z > dst_max.z));
}

Vector3 AABB::GetEndpoint(int p_point) const
{
	switch (p_point)
	{
		case 0:
			return Vector3(position.x, position.y, position.z);
		case 1:
			return Vector3(position.x, position.y, position.z + size.z);
		case 2:
			return Vector3(position.x,position.y + size.y, position.z);
		case 3:
			return Vector3(position.x, position.y + size.y, position.z + size.z);
		case 4:
			return Vector3(position.x + size.x, position.y, position.z);
		case 5:
			return Vector3(position.x + size.x, position.y, position.z + size.z);
		case 6:
			return Vector3(position.x + size.x, position.y + size.y, position.z);
		case 7:
			return Vector3(position.x + size.x, position.y + size.y, position.z + size.z);

	}
}

bool AABB::IntersectsConvexShape(const Plane* p_planes, int p_planeCount, const Vector3* p_points, int p_pointCount) const
{
	Vector3 halfExtents = size * 0.5f;
	Vector3 ofs = position + halfExtents;

	for (int i = 0; i < p_planeCount; i ++)
	{
		const Plane& plane = p_planes[i];
		Vector3 point(
			(plane.normal.x > 0) ? -halfExtents.x : halfExtents.x,
			(plane.normal.y > 0) ? -halfExtents.y : halfExtents.y,
			(plane.normal.z > 0) ? -halfExtents.z : halfExtents.z
		);
		point += ofs;
		if (plane.IsPointOver(point))
			return false;

	}

	int badPointCountsPositive[3] = { 0 };
	int badPointCountsNegative[3] = { 0 };


	for (int k = 0; k < 3; k++)
	{
		for (int i = 0; i < p_pointCount; i++)
		{
			if (p_points[i].coord[k] > ofs.coord[k] + halfExtents.coord[k]) {
				badPointCountsPositive[k]++;
			}
			if (p_points[i].coord[k] < ofs.coord[k] - halfExtents.coord[k]) {
				badPointCountsNegative[k]++;
			}
		}

		if (badPointCountsNegative[k] == p_pointCount) {
			return false;
		}
		if (badPointCountsPositive[k] == p_pointCount) {
			return false;
		}
	}

	return true;
}

bool AABB::InsideConvexShape(const Plane* p_planes, int p_planeCount) const
{
	Vector3 halfExtents = size * 0.5f;
	Vector3 ofs = position + halfExtents;

	for (int i = 0; i < p_planeCount; i++)
	{
		const Plane& plane = p_planes[i];
		Vector3 point(
			(plane.normal.x < 0) ? -halfExtents.x : halfExtents.x,
			(plane.normal.y < 0) ? -halfExtents.y : halfExtents.y,
			(plane.normal.z < 0) ? -halfExtents.z : halfExtents.z
		);

		point += ofs;

		if (plane.IsPointOver(point))
			return false;
	}

	return true;
}

bool AABB::HasPoint(const Vector3& p_point) const
{
	if (p_point.x < position.x) {
		return false;
	}
	if (p_point.y < position.y) {
		return false;
	}
	if (p_point.z < position.z) {
		return false;
	}
	if (p_point.x > position.x + size.x) {
		return false;
	}
	if (p_point.y > position.y + size.y) {
		return false;
	}
	if (p_point.z > position.z + size.z) {
		return false;
	}

	return true;
}

inline void AABB::ExpandTo(const Vector3& p_vector) {
	Vector3 begin = position;
	Vector3 end = position + size;

	if (p_vector.x < begin.x) {
		begin.x = p_vector.x;
	}
	if (p_vector.y < begin.y) {
		begin.y = p_vector.y;
	}
	if (p_vector.z < begin.z) {
		begin.z = p_vector.z;
	}

	if (p_vector.x > end.x) {
		end.x = p_vector.x;
	}
	if (p_vector.y > end.y) {
		end.y = p_vector.y;
	}
	if (p_vector.z > end.z) {
		end.z = p_vector.z;
	}

	position = begin;
	size = end - begin;
}

void AABB::ProjectRangeInPlane(const Plane& p_plane, float& r_min, float& r_max) const {
	Vector3 half_extents(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f);
	Vector3 center(position.x + half_extents.x, position.y + half_extents.y, position.z + half_extents.z);

	float length = Vector3::Dot(Vector3::Abs(p_plane.normal), half_extents);
	float distance = p_plane.DistanceTo(center);
	r_min = distance - length;
	r_max = distance + length;
}


inline float AABB::GetLongestAxisSize() const
{
	float max_size = size.x;

	if (size.y > max_size) {
		max_size = size.y;
	}

	if (size.z > max_size) {
		max_size = size.z;
	}

	return max_size;
}

inline float AABB::GetShortestAxisSize() const
{
	float max_size = size.x;

	if (size.y < max_size) {
		max_size = size.y;
	}

	if (size.z < max_size) {
		max_size = size.z;
	}

	return max_size;
}

bool AABB::SmitsIntersectRay(const Vector3& p_from, const Vector3& p_dir, float t0, float t1) const 
{
	float divx = (float) 1.0 / p_dir.x;
	float divy = (float) 1.0 / p_dir.y;
	float divz = (float) 1.0 / p_dir.z;

	Vector3 upbound = position + size;
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	if (p_dir.x >= 0) {
		tmin = (position.x - p_from.x) * divx;
		tmax = (upbound.x - p_from.x) * divx;
	}
	else {
		tmin = (upbound.x - p_from.x) * divx;
		tmax = (position.x - p_from.x) * divx;
	}
	if (p_dir.y >= 0) {
		tymin = (position.y - p_from.y) * divy;
		tymax = (upbound.y - p_from.y) * divy;
	}
	else {
		tymin = (upbound.y - p_from.y) * divy;
		tymax = (position.y - p_from.y) * divy;
	}
	if ((tmin > tymax) || (tymin > tmax)) {
		return false;
	}
	if (tymin > tmin) {
		tmin = tymin;
	}
	if (tymax < tmax) {
		tmax = tymax;
	}
	if (p_dir.z >= 0) {
		tzmin = (position.z - p_from.z) * divz;
		tzmax = (upbound.z - p_from.z) * divz;
	}
	else {
		tzmin = (upbound.z - p_from.z) * divz;
		tzmax = (position.z - p_from.z) * divz;
	}
	if ((tmin > tzmax) || (tzmin > tmax)) {
		return false;
	}
	if (tzmin > tmin) {
		tmin = tzmin;
	}
	if (tzmax < tmax) {
		tmax = tzmax;
	}
	return ((tmin < t1) && (tmax > t0));
}

void AABB::GrowBy(float p_amount) {
	position.x -= p_amount;
	position.y -= p_amount;
	position.z -= p_amount;
	size.x += 2.0f * p_amount;
	size.y += 2.0f * p_amount;
	size.z += 2.0f * p_amount;
}
