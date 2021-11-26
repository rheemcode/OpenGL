//#pragma once
//#include "Math/Matrix4x4.h"
//#include "Camera.h"
//#include <Mesh.h>
//
//#define MAX_SPLIT 4
//
//class ShadowBox
//{
//	friend struct ShadowData;
//
//	const float offset = 10;
//	uint32_t splitCount = MAX_SPLIT;
//
//	AABB bounds[MAX_SPLIT];
//	Vector4 splitDistances;
//	float nearDistance[MAX_SPLIT], farDistance[MAX_SPLIT];
//	float farHeight[MAX_SPLIT], farWidth[MAX_SPLIT], nearWidth[MAX_SPLIT], nearHeight[MAX_SPLIT];
//
//	Camera* camera;
//	CameraSettings* cameraSettings;
//	const Matrix4x4* lightViewMatrix;
//	const Transform* cameraTransform;
//	float g_fSplitSchemeWeight = 0.5f;
//
//	void UpdateBounds()
//	{
//		constexpr float lambda = 0.75f;
//		//hardcoded should change later
//		const float near = 12.f;
//		const float far = cameraSettings->zfar;
//
//		nearDistance[0] = near;
//		splitDistances[0] = nearDistance[0];
//		nearHeight[0] = nearDistance[0] * Math::Tan(Math::Deg2Rad(cameraSettings->fovY / 2.f));
//		nearWidth[0] = nearHeight[0] * cameraSettings->ratio;
//
//		for (int i = 1; i < (int)splitCount; i++)
//		{
//			float fIDM = i / (float)splitCount;
//			float fLog = near * powf(far / near, fIDM);
//			float fUniform = near + (far - near) * fIDM;
//			//pDistances[i] = fLog * g_fSplitSchemeWeight + fUniform * (1 - g_fSplitSchemeWeight);
//
//			//float si = i / (float)splitCount;
//			nearDistance[i] = fLog * g_fSplitSchemeWeight + fUniform * (1 - g_fSplitSchemeWeight);
//			splitDistances[i] = nearDistance[i];
//			farDistance[i - 1] = nearDistance[i];
//
//
//			nearHeight[i] = nearDistance[i] * Math::Tan(Math::Deg2Rad(cameraSettings->fovY / 2.f));
//			nearWidth[i] = nearHeight[i] * cameraSettings->ratio;
//
//			farHeight[i - 1] = Math::Tan(Math::Deg2Rad(cameraSettings->fovY / 2.0f)) * farDistance[i - 1];
//			farWidth[i - 1] = farHeight[i - 1] * cameraSettings->ratio;
//		}
//		
//		farDistance[splitCount - 1] = far;
//		splitDistances[3] = far;
//		farHeight[splitCount - 1] = Math::Tan(Math::Deg2Rad(cameraSettings->fovY / 2.0f)) * farDistance[splitCount - 1];
//		farWidth[splitCount - 1] = farHeight[splitCount - 1] * cameraSettings->ratio;
//	}
//
//
//	Vector4* UpdateFrustumVertices(const int index)
//	{
//		Matrix4x4 rotationMatrix;
//
//		const Vector3& UP = cameraTransform->GetLocalUp();
//		const Vector3& RIGHT = cameraTransform->GetLocalRight();
//		const Vector3& FORWARD = cameraTransform->GetLocalForward();
//
//
//		const Vector3 toFar = FORWARD * farDistance[index];
//		const Vector3 toNear = FORWARD * nearDistance[index];
//		
//		const Vector3 centerNear = toNear + cameraTransform->GetLocalPosition();
//		const Vector3 centerFar = toFar + cameraTransform->GetLocalPosition();
//		
//		const Vector3 farTop = centerFar + (UP * farHeight[index]);
//		const Vector3 farBottom = centerFar + (-UP * farHeight[index]);
//		const Vector3 nearTop = centerNear + (UP * nearHeight[index]);
//		const Vector3 nearBottom = centerNear - (UP * nearHeight[index]);
//
//		static Vector4 points[8];
//
//		//points[0] = centerNear - UP * nearHeight[index] - RIGHT * nearWidth[index];
//		//points[1] = CalculateLightSpaceFrustumCorner(farTop, -RIGHT, farWidth[index], index);
//		//points[2] = CalculateLightSpaceFrustumCorner(farBottom, RIGHT, farWidth[index], index);
//		//points[3] = CalculateLightSpaceFrustumCorner(farBottom, -RIGHT, farWidth[index], index);
//		//points[4] = CalculateLightSpaceFrustumCorner(nearTop, RIGHT, nearWidth[index], index);
//		//points[5] = CalculateLightSpaceFrustumCorner(nearTop, -RIGHT, nearWidth[index], index);
//		//points[6] = CalculateLightSpaceFrustumCorner(nearBottom, RIGHT, nearWidth[index], index);
//		//points[7] = CalculateLightSpaceFrustumCorner(nearBottom, -RIGHT, nearWidth[index], index);
//
//		points[0] = CalculateLightSpaceFrustumCorner(nearBottom, -RIGHT, nearWidth[index], index);
//		points[1] = CalculateLightSpaceFrustumCorner(nearTop, -RIGHT, nearWidth[index], index);
//		points[2] = CalculateLightSpaceFrustumCorner(nearTop, RIGHT, nearWidth[index], index);
//		points[3] = CalculateLightSpaceFrustumCorner(nearBottom, RIGHT, nearWidth[index], index);
//
//		points[4] = CalculateLightSpaceFrustumCorner(farBottom, -RIGHT, farWidth[index], index);
//		points[5] = CalculateLightSpaceFrustumCorner(farTop, -RIGHT, farWidth[index], index);
//		points[6] = CalculateLightSpaceFrustumCorner(farTop, RIGHT, farWidth[index], index);
//		points[7] = CalculateLightSpaceFrustumCorner(farBottom, RIGHT, farWidth[index], index);
//
//		return points;
//	}
//
//
//	Vector4 CalculateLightSpaceFrustumCorner(const Vector3& startPoint, const Vector3& direction, const float& width, const int index)
//	{
//		Vector4 point = startPoint + (direction * width);
//		point.w = 1.f;
//
//		return  point;
//	}
//
//
//public:
//
//	void Update()
//	{
//		UpdateBounds();
//		for (int i = 0; i < (int)splitCount; i++)
//		{
//			Vector4* points = UpdateFrustumVertices(i);
//
//			bool first = true;
//			for (int x = 0; x < 8; x++)
//			{
//				const Vector3& point = Vector3(points[x].x, points[x].y, points[x].z);
//				if (first)
//				{
//					bounds[i].position = point;
//					bounds[i].size = Vector3::Zero();
//					first = false;
//					continue;
//				}
//				bounds[i].ExpandTo(point);
//			}
//		}
//		
//	}
//
//	
//	Vector3 GetCenter(const int index) const
//	{
//		InstanceBounds instanceBounds(bounds[index]);
//		float x = (instanceBounds.bounds[0] + instanceBounds.bounds[3]) * 0.5f;
//		float y = (instanceBounds.bounds[1] + instanceBounds.bounds[4]) * 0.5f;
//		float z = (instanceBounds.bounds[2] + instanceBounds.bounds[5]) * 0.5f;
//		Vector4 center = Vector4(x, y, z, 1.f);
//		Matrix4x4 invertedMat = *lightViewMatrix;
//		Vector4 res = invertedMat * center;
//		return Vector3(res.x, res.y, res.z);
//	}
//
//	float GetWidth(const int index) const { return (bounds[index].size - (bounds[index].position + bounds[index].size)).x; }
//	float GetHeight(const int index) const { return (bounds[index].size - (bounds[index].position + bounds[index].size)).y; }
//	float GetLength(const int index) const { return (bounds[index].size - (bounds[index].position + bounds[index].size)).z;  }
//
//	void operator=(const ShadowBox& p_box)
//	{
//		lightViewMatrix = p_box.lightViewMatrix;
//		cameraSettings = p_box.cameraSettings;
//		cameraTransform = p_box.cameraTransform;
//		camera = p_box.camera;
//		UpdateBounds();
//	}
//
//	void operator=(ShadowBox&& p_box) noexcept
//	{
//		lightViewMatrix = p_box.lightViewMatrix;
//		cameraTransform = p_box.cameraTransform;
//		cameraSettings = std::move(p_box.cameraSettings);
//		camera = p_box.camera;
//		UpdateBounds();
//	}
//	
//	ShadowBox(const ShadowBox& p_box)
//	{
//		lightViewMatrix = p_box.lightViewMatrix;
//		cameraSettings = p_box.cameraSettings;
//		cameraTransform = p_box.cameraTransform;
//		camera = p_box.camera;
//		UpdateBounds();
//	}
//
//	ShadowBox(ShadowBox&& p_box) noexcept
//	{
//		lightViewMatrix = p_box.lightViewMatrix;
//		cameraTransform = p_box.cameraTransform;
//		cameraSettings = std::move(p_box.cameraSettings);
//		camera = p_box.camera;
//		UpdateBounds();
//	}
//
//	ShadowBox(Matrix4x4* p_lightViewMatrix, const Transform& p_cameraTransform, Camera* p_camera, CameraSettings& p_cameraSettings)
//	{
//		lightViewMatrix = p_lightViewMatrix;
//		cameraTransform = &p_cameraTransform;
//		cameraSettings = &p_cameraSettings;
//		camera = p_camera;
//		UpdateBounds();
//	}
//
//	ShadowBox() = default;
//	~ShadowBox() = default;
//};
//
