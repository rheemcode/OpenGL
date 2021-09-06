#include <glpch.h>
#include "Frustum.h"
#include <immintrin.h>

void Frustum::SetFrustum(float p_fov, float p_aspect, float p_zoom, float p_zNear, float p_zFar, const Matrix4x4& p_transform)
{
	p_zNear = MAX(p_zNear, 0.0f);
	p_zFar = MAX(p_zFar, p_zNear);
	float halfViewSize = Math::Tan(Math::Deg2Rad(p_fov)) / p_zoom;
	Vector3 lNear, lFar;

	lNear.z = p_zNear;
	lNear.y = lNear.z * halfViewSize;
	lNear.x = lNear.y * p_aspect;
	lFar.z = p_zFar;
	lFar.y = lFar.z * halfViewSize;
	lFar.x = lFar.y * p_aspect;

	SetFrustum(lNear, lFar, p_transform);
}

void Frustum::SetFrustum(const Vector3& lNear, const Vector3& lFar, const Matrix4x4& transform)
{
	//vertices[0] = transform * lNear;
	//vertices[1] = transform * Vector3(lNear.x, -lNear.y, lNear.z);
	//vertices[2] = transform * Vector3(-lNear.x, -lNear.y, lNear.z);
	//vertices[3] = transform * Vector3(-lNear.x, lNear.y, lNear.z);
	//vertices[4] = transform * lFar;
	//vertices[5] = transform * Vector3(lFar.x, -lFar.y, lFar.z);
	//vertices[6] = transform * Vector3(-lFar.x, -lFar.y, lFar.z);
	//vertices[7] = transform * Vector3(-lFar.x, lFar.y, lFar.z);

	//UpdatePlanes();
}

void Frustum::SetFrustum(const Matrix4x4& p_projMatrix)
{
	__m256 a = _mm256_set_ps(p_projMatrix[0][3], p_projMatrix[1][3], p_projMatrix[2][3], p_projMatrix[3][3], p_projMatrix[0][3], p_projMatrix[1][3], p_projMatrix[2][3], p_projMatrix[3][3]);
	__m256 b = _mm256_set_ps(p_projMatrix[0][0], p_projMatrix[1][0], p_projMatrix[2][0], p_projMatrix[3][0], p_projMatrix[0][1], p_projMatrix[1][1], p_projMatrix[2][1], p_projMatrix[3][1]);
	__m256 c = _mm256_set_ps(p_projMatrix[0][2], p_projMatrix[1][2], p_projMatrix[2][2], p_projMatrix[3][2], 0, 0, 0, 0);

	
	__m256 a_add = _mm256_add_ps(a, b);
	__m256 a_sub = _mm256_sub_ps(a, b);
	__m256 b_add = _mm256_add_ps(a, c);
	__m256 b_sub = _mm256_sub_ps(a, c);
	
	float* left_f = a_add.m256_f32;
	float* right_f = a_sub.m256_f32;
	float* top_f = b_add.m256_f32;
	float* bottom_f = b_sub.m256_f32;

#ifndef USE_INTRIN
	planes[FrustumPlane::Left].normal.x = left_f[7];
	planes[FrustumPlane::Left].normal.y = left_f[6];
	planes[FrustumPlane::Left].normal.z = left_f[5];
	planes[FrustumPlane::Left].d        = left_f[4];
	
	planes[FrustumPlane::Right].normal.x = right_f[7];
	planes[FrustumPlane::Right].normal.y = right_f[6];
	planes[FrustumPlane::Right].normal.z = right_f[5];
	planes[FrustumPlane::Right].d = right_f[4];


	planes[FrustumPlane::Bottom].normal.x = left_f[3];
	planes[FrustumPlane::Bottom].normal.y = left_f[2];
	planes[FrustumPlane::Bottom].normal.z = left_f[1];
	planes[FrustumPlane::Bottom].d =        left_f[0];

	planes[FrustumPlane::Top].normal.x = right_f[3];
	planes[FrustumPlane::Top].normal.y = right_f[2];
	planes[FrustumPlane::Top].normal.z = right_f[1];
	planes[FrustumPlane::Top].d        = right_f[0];

	planes[FrustumPlane::Near].normal.x = top_f[7];
	planes[FrustumPlane::Near].normal.y = top_f[6];
	planes[FrustumPlane::Near].normal.z = top_f[5];
	planes[FrustumPlane::Near].d = top_f[4];

	planes[FrustumPlane::Far].normal.x = bottom_f[7];
	planes[FrustumPlane::Far].normal.y = bottom_f[6];
	planes[FrustumPlane::Far].normal.z = bottom_f[5];
	planes[FrustumPlane::Far].d = bottom_f[4];
#else
	planes[FrustumPlane::Left].normal.y = p_projMatrix[1][3] + p_projMatrix[1][0];
	planes[FrustumPlane::Left].normal.z = p_projMatrix[2][3] + p_projMatrix[2][0];
	planes[FrustumPlane::Left].d = p_projMatrix[3][3] + p_projMatrix[3][0];

	planes[FrustumPlane::Right].normal.x = p_projMatrix[0][3] - p_projMatrix[0][0];
	planes[FrustumPlane::Right].normal.y = p_projMatrix[1][3] - p_projMatrix[1][0];
	planes[FrustumPlane::Right].normal.z = p_projMatrix[2][3] - p_projMatrix[2][0];
	planes[FrustumPlane::Right].d = p_projMatrix[3][3] - p_projMatrix[3][0];


	planes[FrustumPlane::Bottom].normal.x = p_projMatrix[0][3] + p_projMatrix[0][1];
	planes[FrustumPlane::Bottom].normal.y = p_projMatrix[1][3] + p_projMatrix[1][1];
	planes[FrustumPlane::Bottom].normal.z = p_projMatrix[2][3] + p_projMatrix[2][1];
	planes[FrustumPlane::Bottom].d = p_projMatrix[3][3] + p_projMatrix[3][1];

	planes[FrustumPlane::Top].normal.x = p_projMatrix[0][3] - p_projMatrix[0][1];
	planes[FrustumPlane::Top].normal.y = p_projMatrix[1][3] - p_projMatrix[1][1];
	planes[FrustumPlane::Top].normal.z = p_projMatrix[2][3] - p_projMatrix[2][1];
	planes[FrustumPlane::Top].d = p_projMatrix[3][3] - p_projMatrix[3][1];

	planes[FrustumPlane::Near].normal.x = p_projMatrix[0][3] + p_projMatrix[0][2];
	planes[FrustumPlane::Near].normal.y = p_projMatrix[1][3] + p_projMatrix[1][2];
	planes[FrustumPlane::Near].normal.z = p_projMatrix[2][3] + p_projMatrix[2][2];
	planes[FrustumPlane::Near].d = p_projMatrix[3][3] + p_projMatrix[3][2];

	planes[FrustumPlane::Far].normal.x = p_projMatrix[0][3] - p_projMatrix[0][2];
	planes[FrustumPlane::Far].normal.y = p_projMatrix[1][3] - p_projMatrix[1][2];
	planes[FrustumPlane::Far].normal.z = p_projMatrix[2][3] - p_projMatrix[2][2];
	planes[FrustumPlane::Far].d = p_projMatrix[3][3] - p_projMatrix[3][2];
#endif
	int i = 0;
	for (auto& plane : planes)
	{
		
		plane.normal = -plane.normal;
		plane.Normalize();
		PlaneSign ps(plane);
		planeSigns[i++] = ps;
	}

	//for (int i = 0; i < 6; i++)
	//{
	//	_mm256_dp_ps()
	//}
}

//void Frustum::SetFrustum(const Matrix4x4& p_projMatrix, const Matrix4x4& p_viewMatrix)
//{
//	SetFrustum(p_projMatrix);
//	XForm(p_viewMatrix);
//}
//
//void Frustum::XForm(const Matrix4x4& p_transform)
//{
//	for (auto& vertice : vertices)
//		vertice = p_transform * vertice;
//	UpdatePlanes();
//}
//void Frustum::XForm(const Matrix3x3& p_transform)
//{
//	for (auto& vertice : vertices)
//		vertice = p_transform * vertice;
//	UpdatePlanes();
//}
//
//void Frustum::UpdatePlanes()
//{
//	//planes[PLANE_NEAR] = Plane(vertices[2], vertices[1], vertices[0], 0);
//	//planes[PLANE_LEFT] = Plane(vertices[3], vertices[7], vertices[6], 0);
//	//planes[PLANE_RIGHT] = Plane(vertices[1], vertices[5], vertices[4], 0);
//	//planes[PLANE_UP] = Plane(vertices[0], vertices[4], vertices[7], 0);
//	//planes[PLANE_DOWN] = Plane(vertices[6], vertices[5], vertices[1], 0);
//	//planes[PLANE_FAR] = Plane(vertices[5], vertices[6], vertices[7], 0);
//
//	//if (planes[PLANE_NEAR].DistanceTo(vertices[5]) < 0.0f)
//	//{
//	//	for (auto& plane : planes)
//	//	{
//	//		plane = -plane;
//	//	}
//	//}
//}
