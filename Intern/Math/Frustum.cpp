#include "Frustum.h"

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

	planes[FrustumPlane::Left].normal.x = p_projMatrix[0][3] + p_projMatrix[0][0];
	planes[FrustumPlane::Left].normal.y = p_projMatrix[1][3] + p_projMatrix[1][0];
	planes[FrustumPlane::Left].normal.z = p_projMatrix[2][3] + p_projMatrix[2][0];
	planes[FrustumPlane::Left].d        = p_projMatrix[3][3] + p_projMatrix[3][0];	
	
	planes[FrustumPlane::Right].normal.x = p_projMatrix[0][3] - p_projMatrix[0][0];
	planes[FrustumPlane::Right].normal.y = p_projMatrix[1][3] - p_projMatrix[1][0];
	planes[FrustumPlane::Right].normal.z = p_projMatrix[2][3] - p_projMatrix[2][0];
	planes[FrustumPlane::Right].d        = p_projMatrix[3][3] - p_projMatrix[3][0];	


	planes[FrustumPlane::Bottom].normal.x = p_projMatrix[0][3] + p_projMatrix[0][1];
	planes[FrustumPlane::Bottom].normal.y = p_projMatrix[1][3] + p_projMatrix[1][1];
	planes[FrustumPlane::Bottom].normal.z = p_projMatrix[2][3] + p_projMatrix[2][1];
	planes[FrustumPlane::Bottom].d        = p_projMatrix[3][3] + p_projMatrix[3][1];

	planes[FrustumPlane::Top].normal.x = p_projMatrix[0][3] - p_projMatrix[0][1];
	planes[FrustumPlane::Top].normal.y = p_projMatrix[1][3] - p_projMatrix[1][1];
	planes[FrustumPlane::Top].normal.z = p_projMatrix[2][3] - p_projMatrix[2][1];
	planes[FrustumPlane::Top].d        = p_projMatrix[3][3] - p_projMatrix[3][1];

	planes[FrustumPlane::Near].normal.x = p_projMatrix[0][3] + p_projMatrix[0][2];
	planes[FrustumPlane::Near].normal.y = p_projMatrix[1][3] + p_projMatrix[1][2];
	planes[FrustumPlane::Near].normal.z = p_projMatrix[2][3] + p_projMatrix[2][2];
	planes[FrustumPlane::Near].d        = p_projMatrix[3][3] + p_projMatrix[3][2];

	planes[FrustumPlane::Far].normal.x = p_projMatrix[0][3] - p_projMatrix[0][2];
	planes[FrustumPlane::Far].normal.y = p_projMatrix[1][3] - p_projMatrix[1][2];
	planes[FrustumPlane::Far].normal.z = p_projMatrix[2][3] - p_projMatrix[2][2];
	planes[FrustumPlane::Far].d        = p_projMatrix[3][3] - p_projMatrix[3][2];

	int i = 0;
	for (auto& plane : planes)
	{
		plane.normal = -plane.normal;
		plane.Normalize();
		PlaneSign ps(plane);
		planeSigns[i++] = ps;
	}
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
