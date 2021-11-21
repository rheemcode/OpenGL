#include <glpch.h>
//#include "ShadowBox.h"
//
//void ShadowData::UpdateProjection()
//{
//	for (int i = 0; i < 4; i++)
//	{
//		const CameraSettings& camSettings = *shadowBounds.cameraSettings;
//		InstanceBounds bounds(shadowBounds.bounds[i]);
//		//InstanceBounds bounds2(shadowBounds.bounds[3]);
//
//		//Proj[0] = Matrix4x4(
//		//	2.f / (bounds.bounds[3] - bounds.bounds[0]), 0.0f, 0.0f, 0.0f,
//		//	0.0f, 2.f / (bounds.bounds[4] - bounds.bounds[1]), 0.0f, 0.0f,
//		//	0.0f, 0.0f, -2.f / (bounds.bounds[5] - bounds.bounds[2]), 0.0f,
//		//	0.0f, 0.0f, 0.0f, 1.0f);
//
//	//	Proj[i] = Matrix4x4::CreateOrtho(bounds.bounds[3], bounds.bounds[0],  bounds.bounds[4], bounds.bounds[1], bounds.bounds[2], bounds.bounds[5]);
//		//Proj = Matrix4x4::CreateOrtho(-680, 680, -380, 380, -600.f, 600.f);
//		//Proj = Matrix4x4::CreatePerspective(camSettings.fovY, camSettings.ratio, 0.1f, 20.f);
//
//
//	}
//}
//
//void ShadowData::CalculateCropMatrix(const std::vector<Mesh>& receivers, const std::vector<Mesh>& casters)
//{
//	Vector4 depthSplits(0.04f, 0.15f, 0.4f, 1.0f);
//	Vector4 splits[4];
//	float nearClip = 0.1f;
//	float farClip = 500.f;
//	float clipRange = farClip - nearClip;
//
//	float minZ = nearClip;
//	float maxZ = nearClip + clipRange;
//	float range = maxZ - minZ;
//	float ratio = maxZ / minZ;
//	// Calculate split depths based on view camera frustum
//	// Based on method presented in https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch10.html
//	for (uint32_t i = 0; i < 4; i++)
//	{
//		float p = static_cast<float>(i + 1) / static_cast<float>(4);
//		float log = minZ * std::pow(ratio, p);
//		float uniform = minZ + range * p;
//		float d = 0.5f * (log - uniform) + uniform;
//		depthSplits[i] = (d - nearClip) / clipRange;
//	}
//	
//	Matrix4x4 modifiedProj = shadowBounds.camera->GetProjectionMatrix();
//	Matrix4x4 invCam =  Matrix4x4::Inverse(modifiedProj * shadowBounds.camera->GetViewMatrix());
//
//		
//	float lastSplitDist = 0.0f;
//	for (int c = 0; c < (int)4; ++c)
//	{
//		float splitDist = depthSplits[c];
//
//		Vector3 frustumCorners[8] = {
//			{ -1.0f,  1.0f, -1.0f },
//			{  1.0f,  1.0f, -1.0f },
//			{  1.0f, -1.0f, -1.0f },
//			{ -1.0f, -1.0f, -1.0f },
//			{ -1.0f,  1.0f,  1.0f },
//			{  1.0f,  1.0f,  1.0f },
//			{  1.0f, -1.0f,  1.0f },
//			{ -1.0f, -1.0f,  1.0f },
//		};
//
//		// Transform frustum corners from clip space to world space
//		for (Vector3& frustumCorner : frustumCorners)
//		{
//			Vector4 invCorner = invCam * Vector4(frustumCorner, 1.0f);
//			frustumCorner = invCorner / invCorner.w;
//		}
//
//		for (int i = 0; i < 4; ++i)
//		{
//			Vector3 dist = frustumCorners[i + 4] - frustumCorners[i];
//			frustumCorners[i + 4] = frustumCorners[i] + (dist * splitDist);
//			frustumCorners[i] = frustumCorners[i] + (dist * lastSplitDist);
//		}
//
//		Vector3 frustumCenter;
//		for (const Vector3& frustumCorner : frustumCorners)
//		{
//			frustumCenter += frustumCorner;
//		}
//		frustumCenter /= 8.0f;
//
//		float radius = 0.0f;
//		for (const Vector3& frustumCorner : frustumCorners)
//		{
//			float distance = Vector3::Length(frustumCorner - frustumCenter);
//			radius = MAX(radius, distance);
//		}
//		radius = std::ceil(radius * 16.0f) / 16.0f;
//
//		Vector3 maxExtents = Vector3(radius, radius, radius);
//		Vector3 minExtents = -maxExtents;
//		float zFar = shadowBounds.cameraSettings->zfar;
//		float width = shadowBounds.GetWidth(c);
//		float height = shadowBounds.GetHeight(c);
//		float length = shadowBounds.GetLength(c);
//		auto center = shadowBounds.GetCenter(c);
//		
//		float pitch = Math::ACos(Vector2::Length(Vector2(LightDir.x, LightDir.z)));
//		float yaw = Math::Rad2deg((Math::ATan(LightDir.x / LightDir.z)));
//		float yawRad = Math::Deg2Rad(yaw);
//	
//		View = Matrix4x4::Identity;
//		View = Matrix4x4::Rotate(View, Vector3(1, 0, 0), pitch);
//
//		yaw = LightDir.z > 0 ? yaw - 180 : yaw;
//		View = Matrix4x4::Rotate(View, Vector3(0, 1, 0), -yawRad);    
//		//View = Matrix4x4::CreateLookAt(frustumCenter - (-LightDir) * minExtents.z, Vector3::Zero(), Vector3::Up());
//		View = Matrix4x4::Translate(View, -frustumCenter);
//		Proj = Matrix4x4::CreateOrtho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y,  -(maxExtents.z - minExtents.z), maxExtents.z - minExtents.z);
//
//		TextureMatrix[c] = Proj * View;
//
//		lastSplitDist = depthSplits[c];
//	}
//}
//
//Matrix4x4 ShadowData::BuildCropMatrix(const Vector3& min, const Vector3& max)
//{
//
//	float scaleX, scaleY, scaleZ;
//	float offsetX, offsetY, offsetZ;
//
//	scaleX = 2.f / (max.x - min.x);
//	scaleY = 2.f / (max.y - min.y);
//
//	offsetX = -0.5f * (max.x + min.x) * scaleX;
//	offsetY = -0.5f * (max.y + min.y) * scaleY;
//
//	scaleZ = -2.f / (max.z - min.z);
//	offsetZ = -min.z * scaleZ;
//
//	return Matrix4x4
//	(scaleX, 0.0f, 0.0f, 0.f,
//		0.0f, scaleY, 0.0f, 0.0f,
//		0.0f, 0.0f, 1.f, 0.0f,
//		offsetX, offsetY, 0.f, 1.0f
//	);
//}
//
//void ShadowData::UpdateView(Vector3 direction)
//{
//	LightDir = direction;
//	/*direction = Vector3::Normalize(direction);
//	float pitch = Math::ACos(Vector2::Length(Vector2(direction.x, direction.z)));
//	float yaw = Math::Rad2deg((Math::ATan(direction.x / direction.z)));
//	float yawRad = Math::Deg2Rad(yaw);
//	const auto& center = -shadowBounds.GetCenter(0);
//	View = Matrix4x4::Identity;
//	View = Matrix4x4::Rotate(View, Vector3(1, 0, 0), pitch);
//	yaw = direction.z > 0 ? yaw - 180 : yaw;
//	View = Matrix4x4::Rotate(View, Vector3(0, 1, 0), -yawRad);
//
//	View = Matrix4x4::CreateLookAt(-LightDir, Vector3(0, 0, 0), Vector3::Up());*/
//	//iew = Matrix4x4::Translate(View, center);
//
//}
