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

struct GLIB_API PlaneSign
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

struct GLIB_API Frustum
{
	std::array<Plane, 6> planes;
	std::array<PlaneSign, 6> planeSigns;
	std::array<Vector3, 8> vertices;

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

	__forceinline bool SweepIntersection(const InstanceBounds& instanceBounds, const Vector3& sweepDir)
	{
		const Vector3& frustumMin = Vector3(instanceBounds.bounds[0], instanceBounds.bounds[1], instanceBounds.bounds[2]);
		const Vector3& frustumMax = Vector3(instanceBounds.bounds[3], instanceBounds.bounds[4], instanceBounds.bounds[5]);
		const Vector3& objectMin = Vector3(bounds[0], bounds[1], bounds[2]);
		const Vector3& objectMax = Vector3(bounds[3], bounds[4], bounds[5]);

		Vector3 frustumCenter = (frustumMin + frustumMax) * 0.5f;
		Vector3 frustumHalfSize = (frustumMax - frustumMin) * 0.5f;
		float frustumCenterProj = Vector3::Dot(frustumCenter, sweepDir);

		
		float frustumHalfSizeProj = frustumHalfSize.x * Math::Abs(sweepDir.x) + frustumHalfSize.y * Math::Abs(sweepDir.y) + frustumHalfSize.z * Math::Abs(sweepDir.z);
		float frustumProjMin = frustumCenterProj - frustumHalfSizeProj;
		float frustumProjMax = frustumCenterProj + frustumHalfSizeProj;

		Vector3 objectCenter = (objectMin + objectMax) * 0.5f;
		Vector3 objectHalfSize = (objectMin - objectMax) * 0.5f;
		float objectCenterProj = Vector3::Dot(objectCenter, sweepDir);
		
		float objectHalfSizeProj = objectHalfSize.x * Math::Abs(sweepDir.x) + objectHalfSize.y * Math::Abs(sweepDir.y) + objectHalfSize.z * Math::Abs(sweepDir.z);
		float objectProjMin = objectCenterProj - objectHalfSizeProj;
		float objectProjMax = objectCenterProj + objectHalfSizeProj;

		float distMin = frustumProjMin - objectProjMax;
		float distMax = frustumProjMax - objectProjMin;

		if (distMin > distMax) SWAP(distMin, distMax);

		if (distMax < 0) return false;

		if (sweepDir.x == 0)
		{
			if (frustumMin.x > objectMax.x || objectMin.x > frustumMax.x) return false;
		}

		else
		{
			float distMinNew = (frustumMin.x - objectMax.x) / sweepDir.x;
			float distMaxNew = (frustumMax.x - objectMin.x) / sweepDir.x;
			if (distMinNew > distMaxNew) SWAP(distMinNew, distMaxNew);

			if (distMin > distMaxNew || distMinNew > distMaxNew) return false;

			distMin = MAX(distMin, distMinNew);
			distMax = MAX(distMax, distMaxNew);
		}
		
		if (sweepDir.y == 0)
		{
			if (frustumMin.y > objectMax.y || objectMin.y > frustumMax.y) return false;
		}

		else
		{
			float distMinNew = (frustumMin.y - objectMax.y) / sweepDir.y;
			float distMaxNew = (frustumMax.y - objectMin.y) / sweepDir.y;
			if (distMinNew > distMaxNew) SWAP(distMinNew, distMaxNew);

			if (distMin > distMaxNew || distMinNew > distMaxNew) return false;

			distMin = MAX(distMin, distMinNew);
			distMax = MAX(distMax, distMaxNew);
		}
		
		if (sweepDir.z == 0)
		{
			if (frustumMin.z > objectMax.z || objectMin.z > frustumMax.z) return false;
		}

		else
		{
			float distMinNew = (frustumMin.z - objectMax.z) / sweepDir.z;
			float distMaxNew = (frustumMax.z - objectMin.z) / sweepDir.z;
			if (distMinNew > distMaxNew) SWAP(distMinNew, distMaxNew);

			if (distMin > distMaxNew || distMinNew > distMaxNew) return false;
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

	__forceinline AABB GetAABB()
	{
		
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
