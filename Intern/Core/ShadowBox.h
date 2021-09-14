#pragma once
#include "Math/Matrix4x4.h"
#include "Camera.h"

class ShadowBox
{
	friend struct ShadowData;

	const float offset = 10;
	const float shadowDistance = 10;

	float minX = 0, maxX = 0;
	float minY = 0, maxY = 0;
	float minZ = 0, maxZ = 0;
	float farHeight = 0, farWidth = 0, nearWidth = 0, nearHeight = 0;

	uint32_t splitCount = 0;

	CameraSettings* cameraSettings;
	const Matrix4x4* lightViewMatrix;
	const Transform* cameraTransform;

	void CalculateBounds()
	{
		farWidth = shadowDistance * Math::Tan(Math::Deg2Rad(cameraSettings->fovY));
		nearWidth = cameraSettings->znear * Math::Tan(Math::Deg2Rad(cameraSettings->fovY));

		farHeight = farWidth / cameraSettings->ratio;
		nearHeight = nearWidth / cameraSettings->ratio;
	}


	Vector4* CalculateFrustumVertices()
	{
		const Vector3& UP = cameraTransform->GetLocalUp();
		const Vector3& RIGHT = cameraTransform->GetLocalRight();
		const Vector3& FORWARD = cameraTransform->GetLocalForward();


		Vector3 toFar = FORWARD * shadowDistance;
		Vector3 toNear = FORWARD * cameraSettings->znear;

		Vector3 centerNear = toNear + cameraTransform->GetLocalPosition();
		Vector3 centerFar = toFar + cameraTransform->GetLocalPosition();

		Vector3 farTop = centerFar + (UP * farHeight);
		Vector3 farBottom = centerFar + (-UP * farHeight);
		Vector3 nearTop = centerNear + (UP * nearHeight);
		Vector3 nearBottom = centerNear + (-UP * nearHeight);

		static Vector4 points[8];

		points[0] = CalculateLightSpaceFrustumCorner(farTop, RIGHT, farWidth);
		points[1] = CalculateLightSpaceFrustumCorner(farTop, -RIGHT, farWidth);
		points[2] = CalculateLightSpaceFrustumCorner(farBottom, RIGHT, farWidth);
		points[3] = CalculateLightSpaceFrustumCorner(farBottom, -RIGHT, farWidth);
		points[4] = CalculateLightSpaceFrustumCorner(nearTop, RIGHT, nearWidth);
		points[5] = CalculateLightSpaceFrustumCorner(nearTop, -RIGHT, nearWidth);
		points[6] = CalculateLightSpaceFrustumCorner(nearBottom, RIGHT, nearWidth);
		points[7] = CalculateLightSpaceFrustumCorner(nearBottom, -RIGHT, nearWidth);
		return points;
	}


	Vector4 CalculateLightSpaceFrustumCorner(const Vector3& startPoint, const Vector3& direction, const float& width)
	{
		Vector4 point = startPoint + (direction * width);
		point.w = 1.f;

		return *lightViewMatrix * point;
	}


public:

	void UpdateBounds()
	{
		Vector4* points = CalculateFrustumVertices();

		bool first = true;
		for (int i = 0; i < 8; i++) 
		{
			const Vector4& point = points[i];
			if (first) 
			{
				minX = point.x;
				maxX = point.x;
				minY = point.y;
				maxY = point.y;
				minZ = point.z;
				maxZ = point.z;
				first = false;
				continue;
			}
			if (point.x > maxX) {
				maxX = point.x;
			}
			else if (point.x < minX) {
				minX = point.x;
			}
			if (point.y > maxY) {
				maxY = point.y;
			}
			else if (point.y < minY) {
				minY = point.y;
			}
			if (point.z > maxZ) {
				maxZ = point.z;
			}
			else if (point.z < minZ) {
				minZ = point.z;
			}
		}
		maxZ += offset;
	}

	Vector3 GetCenter() const
	{
		float x = (minX + maxX) * 0.5f;
		float y = (minY + maxY) * 0.5f;
		float z = (minZ + maxZ) * 0.5f;
		Vector4 center = Vector4(x, y, z, 1.f);
		Matrix4x4 invertedMat = Matrix4x4::Inverse(*lightViewMatrix);
		Vector4 res = invertedMat * center;
		return Vector3(res.x, res.y, res.z);
	}

	float GetWidth() const { return maxX - minX; }
	float GetHeight() const { return maxY - minY; }
	float GetLength() const { return maxZ - minZ;  }

	void operator=(const ShadowBox& p_box)
	{
		lightViewMatrix = p_box.lightViewMatrix;
		cameraSettings = p_box.cameraSettings;
		cameraTransform = p_box.cameraTransform;

		CalculateBounds();
		UpdateBounds();
	}

	void operator=(ShadowBox&& p_box) noexcept
	{
		lightViewMatrix = p_box.lightViewMatrix;
		cameraTransform = p_box.cameraTransform;
		cameraSettings = std::move(p_box.cameraSettings);

		CalculateBounds();
	}
	
	ShadowBox(const ShadowBox& p_box)
	{
		lightViewMatrix = p_box.lightViewMatrix;
		cameraSettings = p_box.cameraSettings;
		cameraTransform = p_box.cameraTransform;

		CalculateBounds();
		UpdateBounds();
	}

	ShadowBox(ShadowBox&& p_box) noexcept
	{
		lightViewMatrix = p_box.lightViewMatrix;
		cameraTransform = p_box.cameraTransform;
		cameraSettings = std::move(p_box.cameraSettings);

		CalculateBounds();
	}

	ShadowBox(const Matrix4x4& p_lightViewMatrix, const Transform& p_cameraTransform, CameraSettings& const p_cameraSettings)
	{
		lightViewMatrix = &p_lightViewMatrix;
		cameraTransform = &p_cameraTransform;
		cameraSettings = &p_cameraSettings;

		CalculateBounds();
	}

	ShadowBox() = default;
	~ShadowBox() = default;
};

