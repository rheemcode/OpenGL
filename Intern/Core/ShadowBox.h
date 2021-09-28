#pragma once
#include "Math/Matrix4x4.h"
#include "Camera.h"

#define MAX_SPLIT 4

class ShadowBox
{
	friend struct ShadowData;

	const float offset = 10;
	uint32_t splitCount = MAX_SPLIT;

	float minX[MAX_SPLIT], maxX[MAX_SPLIT];
	float minY[MAX_SPLIT], maxY[MAX_SPLIT];
	float minZ[MAX_SPLIT], maxZ[MAX_SPLIT];
	float nearDistance[MAX_SPLIT], farDistance[MAX_SPLIT];
	float farHeight[MAX_SPLIT], farWidth[MAX_SPLIT], nearWidth[MAX_SPLIT], nearHeight[MAX_SPLIT];


	CameraSettings* cameraSettings;
	const Matrix4x4* lightViewMatrix;
	const Transform* cameraTransform;

	void UpdateBounds()
	{
		constexpr float lambda = 0.75f;
		//hardcoded should change later
		const float far = cameraSettings->zfar / 5.f;
		const float ratio = far / 1.f;
		nearDistance[0] = 1.f;
		nearHeight[0] = nearDistance[0] * Math::Tan(Math::Deg2Rad(cameraSettings->fovY));
		nearWidth[0] = nearHeight[0] * cameraSettings->ratio;

		for (int i = 1; i < (int)splitCount; i++)
		{
			float si = i / (float)splitCount;
			nearDistance[i] = lambda * (1 * Math::Pow(ratio, si)) + (1.f - lambda) * (1.f + (far - 1.f) * si);
			farDistance[i - 1] = nearDistance[i] * 1.005f;


			nearHeight[i] = nearDistance[i] * Math::Tan(Math::Deg2Rad(cameraSettings->fovY));
			nearWidth[i] = nearHeight[i] * cameraSettings->ratio;

			farHeight[i - 1] = Math::Tan(Math::Deg2Rad(cameraSettings->fovY / 2.0f)) * farDistance[i - 1];
			farWidth[i - 1] = farHeight[i - 1] * cameraSettings->ratio;
		}
		
		farDistance[splitCount - 1] = far;
		farHeight[splitCount - 1] = Math::Tan(Math::Deg2Rad(cameraSettings->fovY / 2.0f)) * farDistance[splitCount - 1];
		farWidth[splitCount - 1] = farHeight[splitCount - 1] * cameraSettings->ratio;
	}


	Vector4* UpdateFrustumVertices(const int index)
	{
		const Vector3& UP = cameraTransform->GetLocalUp();
		const Vector3& RIGHT = cameraTransform->GetLocalRight();
		const Vector3& FORWARD = cameraTransform->GetLocalForward();


		const Vector3 toFar = FORWARD * farDistance[index];
		const Vector3 toNear = FORWARD * nearDistance[index];
		
		const Vector3 centerNear = toNear + cameraTransform->GetLocalPosition();
		const Vector3 centerFar = toFar + cameraTransform->GetLocalPosition();
		
		const Vector3 farTop = centerFar + (UP * farHeight[index]);
		const Vector3 farBottom = centerFar + (-UP * farHeight[index]);
		const Vector3 nearTop = centerNear + (UP * nearHeight[index]);
		const Vector3 nearBottom = centerNear + (-UP * nearHeight[index]);

		static Vector4 points[8];

		points[0] = CalculateLightSpaceFrustumCorner(farTop, RIGHT, farWidth[index], index);
		points[1] = CalculateLightSpaceFrustumCorner(farTop, -RIGHT, farWidth[index], index);
		points[2] = CalculateLightSpaceFrustumCorner(farBottom, RIGHT, farWidth[index], index);
		points[3] = CalculateLightSpaceFrustumCorner(farBottom, -RIGHT, farWidth[index], index);
		points[4] = CalculateLightSpaceFrustumCorner(nearTop, RIGHT, nearWidth[index], index);
		points[5] = CalculateLightSpaceFrustumCorner(nearTop, -RIGHT, nearWidth[index], index);
		points[6] = CalculateLightSpaceFrustumCorner(nearBottom, RIGHT, nearWidth[index], index);
		points[7] = CalculateLightSpaceFrustumCorner(nearBottom, -RIGHT, nearWidth[index], index);
		return points;
	}


	Vector4 CalculateLightSpaceFrustumCorner(const Vector3& startPoint, const Vector3& direction, const float& width, const int index)
	{
		Vector4 point = startPoint + (direction * width);
		point.w = 1.f;

		return lightViewMatrix[index] * point;
	}


public:

	void Update()
	{
		UpdateBounds();
		for (int i = 0; i < (int)splitCount; i++)
		{
			Vector4* points = UpdateFrustumVertices(i);

			bool first = true;
			for (int x = 0; x < 8; x++)
			{
				const Vector4& point = points[x];
				if (first)
				{
					minX[i] = point.x;
					maxX[i] = point.x;
					minY[i] = point.y;
					maxY[i] = point.y;
					minZ[i] = point.z;
					maxZ[i] = point.z;
					first = false;
					continue;
				}
				if (point.x > maxX[i]) {
					maxX[i] = point.x;
				}
				else if (point.x < minX[i]) {
					minX[i] = point.x;
				}
				if (point.y > maxY[i]) {
					maxY[i] = point.y;
				}
				else if (point.y < minY[i]) {
					minY[i] = point.y;
				}
				if (point.z > maxZ[i]) {
					maxZ[i] = point.z;
				}
				else if (point.z < minZ[i]) {
					minZ[i] = point.z;
				}
			}
	//		maxZ[i] += offset;
		}
		
	}

	Vector3 GetCenter(const int index) const
	{
		float x = (minX[index] + maxX[index]) * 0.5f;
		float y = (minY[index] + maxY[index]) * 0.5f;
		float z = (minZ[index] + maxZ[index]) * 0.5f;
		Vector4 center = Vector4(x, y, z, 1.f);
		Matrix4x4 invertedMat = Matrix4x4::Inverse(lightViewMatrix[index]);
		Vector4 res = invertedMat * center;
		return Vector3(res.x, res.y, res.z);
	}

	float GetWidth(const int index) const { return maxX[index] - minX[index]; }
	float GetHeight(const int index) const { return maxY[index] - minY[index]; }
	float GetLength(const int index) const { return maxZ[index] - minZ[index];  }

	void operator=(const ShadowBox& p_box)
	{
		lightViewMatrix = p_box.lightViewMatrix;
		cameraSettings = p_box.cameraSettings;
		cameraTransform = p_box.cameraTransform;

		UpdateBounds();
	}

	void operator=(ShadowBox&& p_box) noexcept
	{
		lightViewMatrix = p_box.lightViewMatrix;
		cameraTransform = p_box.cameraTransform;
		cameraSettings = std::move(p_box.cameraSettings);

		UpdateBounds();
	}
	
	ShadowBox(const ShadowBox& p_box)
	{
		lightViewMatrix = p_box.lightViewMatrix;
		cameraSettings = p_box.cameraSettings;
		cameraTransform = p_box.cameraTransform;

		UpdateBounds();
	}

	ShadowBox(ShadowBox&& p_box) noexcept
	{
		lightViewMatrix = p_box.lightViewMatrix;
		cameraTransform = p_box.cameraTransform;
		cameraSettings = std::move(p_box.cameraSettings);

		UpdateBounds();
	}

	ShadowBox(Matrix4x4* p_lightViewMatrix, const Transform& p_cameraTransform, CameraSettings& p_cameraSettings)
	{
		lightViewMatrix = p_lightViewMatrix;
		cameraTransform = &p_cameraTransform;
		cameraSettings = &p_cameraSettings;

		UpdateBounds();
	}

	ShadowBox() = default;
	~ShadowBox() = default;
};


struct GLIB_API ShadowData
{

	uint32_t splitCount = MAX_SPLIT;
	
	Vector4 farBound;
	Vector2 ShadowSize;
	Vector3 LightDir;

	Matrix4x4 View[MAX_SPLIT];
	Matrix4x4 Proj[MAX_SPLIT];

	Matrix4x4 Bias;
	Matrix4x4 ProjView;

	ShadowBox shadowBounds;


	void UpdateView(Vector3 direction)
	{
		direction = Vector3::Normalize(direction);
		float pitch = Math::ACos(Vector2::Length(Vector2(direction.x, direction.z)));
		float yaw = Math::Rad2deg((Math::ATan(direction.x / direction.z)));
		float yawRad = Math::Deg2Rad(yaw);
		for (int i = 0; i < int(splitCount); i++)
		{
			const auto& center = -shadowBounds.GetCenter(i);
			View[i] = Matrix4x4::Identity;
			View[i] = Matrix4x4::Rotate(View[i], Vector3(1, 0, 0), pitch);
			yaw = direction.z > 0 ? yaw - 180 : yaw;
			View[i] = Matrix4x4::Rotate(View[i], Vector3(0, 1, 0), -yawRad);

			View[i] = Matrix4x4::Translate(View[i], center);
		}
	}

	void UpdateFarBounds(const Matrix4x4& p_Proj)
	{
		for (int i = 0; i < (int)splitCount; i++)
		{
			farBound[i] = 0.5f * (-shadowBounds.farDistance[i] * p_Proj[2][2] + p_Proj[3][2]) / shadowBounds.farDistance[i] + 0.5f;
		}

	}

	void UpdateProjection()
	{
		const CameraSettings& camSettings = *shadowBounds.cameraSettings;
		for (int i = 0; i < (int)splitCount; i++)
		{
			Proj[i] = Matrix4x4::CreateOrtho(shadowBounds.minX[i], shadowBounds.maxX[i], shadowBounds.minY[i], shadowBounds.maxY[i], shadowBounds.minZ[i], shadowBounds.maxZ[i]);
		}
		//Proj = Matrix4x4::CreatePerspective(camSettings.fovY, camSettings.ratio, 0.1f, 20.f);
	}
};
