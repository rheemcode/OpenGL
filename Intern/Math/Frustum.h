#pragma once
#include <array>
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

class Frustum
{
	std::array<Plane, 6> planes;
	std::array<Vector3, 8> vertices;
public:
	void SetFrustum(float p_fov, float p_aspect, float p_zoom, float p_zNear, float p_zFar, const Matrix4x4& transform = Matrix4x4::Identity);
	void SetFrustum(const Vector3& p_near, const Vector3& p_far, const Matrix4x4& transform = Matrix4x4::Identity);
	void SetFrustum(const Matrix4x4& p_projMatrix);
//	void SetFrustum(const Matrix4x4& p_projMatrix, const Matrix4x4& p_viewMatrix);
	void XForm(const Matrix3x3& p_transform);
	void XForm(const Matrix4x4& p_transform);
	
	void UpdatePlanes();
	bool PointInFrustum(const Vector3& p_point) const
	{
		for (const auto& plane : planes)
		{
			if (plane.DistanceTo(p_point) < 0)
				return false;
		}

		return true;
	}

	bool SphereInFrustum(const Vector3& p_position, float radius) const
	{
		for (const auto& plane : planes)
		{
			if (plane.DistanceTo(p_position) < -radius)
				return false;
		}

		return true;
	}

	bool CubeInFrustum(const AABB& p_aabb) const
	{

		bool inside = true;
		Vector3 center = p_aabb.position;
		Vector3 edge = p_aabb.GetEnd();

		for (const auto& plane : planes)
		{
			float dist = plane.DistanceTo(center);
			float absDist = Vector3::Dot(Vector3::Abs(plane.normal), edge);

			if (dist < -absDist)
				return false;
			else if (dist < absDist)
				inside = true;
			
		}

		return inside ? true : false;
	};

	float Distance(const Vector3& p_point) const
	{
		float dist = 0.0f;
		for (const auto& plane : planes)
		{
			dist = MAX(-plane.DistanceTo(p_point), dist);
			dist = MAX(-plane.DistanceTo(p_point), dist);
		}

		return dist;
	}
};