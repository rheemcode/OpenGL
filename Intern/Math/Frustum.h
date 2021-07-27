#pragma once
#include <array>
#include "Vector3.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Plane.h"
#include "AABB.h"

enum FrustumPlane
{
	Near,
	Far,
	Left,
	Right,
	Top,
	Bottom,
};

struct PlaneSign
{
	uint32_t signs[3];
	PlaneSign() {}
	PlaneSign(const Plane& p_plane)
	{
		if (p_plane.normal.x > 0)
			signs[0] = 0;
		else
			signs[0] = 3;

		if (p_plane.normal.y > 0)
			signs[1] = 1;
		else
			signs[1] = 4;		

		if (p_plane.normal.z > 0)
			signs[2] = 2;
		else
			signs[2] = 5;
		
	}
};

struct Frustum
{
	std::array<Plane, 6> planes;
	std::array<PlaneSign, 6> planeSigns;

	void SetFrustum(float p_fov, float p_aspect, float p_zoom, float p_zNear, float p_zFar, const Matrix4x4& transform = Matrix4x4::Identity);
	void SetFrustum(const Vector3& p_near, const Vector3& p_far, const Matrix4x4& transform = Matrix4x4::Identity);
	void SetFrustum(const Matrix4x4& p_projMatrix);
//	void SetFrustum(const Matrix4x4& p_projMatrix, const Matrix4x4& p_viewMatrix);
//	void XForm(const Matrix3x3& p_transform);
//	void XForm(const Matrix4x4& p_transform);
	
	bool CubeInFrustum(const AABB& p_aabb) const
	{
		bool inside = true;

		Vector3 center = (p_aabb.position + p_aabb.size) / 2.f;
		Vector3 edge = p_aabb.GetEnd();

		for (const auto& plane : planes)
		{
			float dist = plane.DistanceTo(center);
			float absDist = Vector3::Dot(Vector3::Abs(plane.normal), edge);

			if (dist > absDist)
				return false;
		}

		return inside;
	}

	void operator=(const Frustum& p_frustum)
	{
		planes = p_frustum.planes;
		planeSigns = p_frustum.planeSigns;
	}

	Frustum() {};
	Frustum(const Frustum& p_frustum)
	{
		planes = p_frustum.planes;
		planeSigns = p_frustum.planeSigns;
	}
	Frustum(const Matrix4x4& p_projMatrix)
	{
		SetFrustum(p_projMatrix);
	}
};


struct InstanceBounds
{
	float bounds[6];

	__forceinline bool InFrustum(const Frustum& p_frustum) const
	{
		for (int i = 0; i < 6; i++)
		{
			Vector3 min = {
				bounds[p_frustum.planeSigns[i].signs[0]],
				bounds[p_frustum.planeSigns[i].signs[1]],
				bounds[p_frustum.planeSigns[i].signs[2]] };

			if (p_frustum.planes[i].DistanceTo(min) >= 0.f)
				return false;
		}

		return true;
	}

	__forceinline bool InAABB(const AABB& p_aabb) const
	{
		Vector3 end = p_aabb.GetSize();

		if (bounds[0] >= end.x)
			return false;
		if (bounds[3] <= p_aabb.position.x)
			return false;
		if (bounds[1] >= end.y)
			return false;
		if (bounds[4] <= p_aabb.position.y)
			return false;
		if (bounds[2] >= end.z)
			return false;
		if (bounds[5] <= p_aabb.position.z)
			return false;

		return true;
	}

	__forceinline void SetBounds(const AABB& p_aabb)
	{
		bounds[0] = p_aabb.position.x;
		bounds[1] = p_aabb.position.y;
		bounds[2] = p_aabb.position.z;
		bounds[3] = p_aabb.position.x + p_aabb.size.x;
		bounds[4] = p_aabb.position.y + p_aabb.size.y;
		bounds[5] = p_aabb.position.z + p_aabb.size.z;
	}


	__forceinline InstanceBounds() {}

	__forceinline InstanceBounds(const AABB& p_aabb)
	{
		SetBounds(p_aabb);
	}

};
