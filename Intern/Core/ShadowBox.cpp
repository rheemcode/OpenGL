#include <glpch.h>
#include "ShadowBox.h"

void ShadowData::UpdateProjection()
{
	for (int i = 0; i < 4; i++)
	{
		const CameraSettings& camSettings = *shadowBounds.cameraSettings;
		InstanceBounds bounds(shadowBounds.bounds[i]);
		//InstanceBounds bounds2(shadowBounds.bounds[3]);

		//Proj[0] = Matrix4x4(
		//	2.f / (bounds.bounds[3] - bounds.bounds[0]), 0.0f, 0.0f, 0.0f,
		//	0.0f, 2.f / (bounds.bounds[4] - bounds.bounds[1]), 0.0f, 0.0f,
		//	0.0f, 0.0f, -2.f / (bounds.bounds[5] - bounds.bounds[2]), 0.0f,
		//	0.0f, 0.0f, 0.0f, 1.0f);

	//	Proj[i] = Matrix4x4::CreateOrtho(bounds.bounds[3], bounds.bounds[0],  bounds.bounds[4], bounds.bounds[1], bounds.bounds[2], bounds.bounds[5]);
		//Proj = Matrix4x4::CreateOrtho(-680, 680, -380, 380, -600.f, 600.f);
		//Proj = Matrix4x4::CreatePerspective(camSettings.fovY, camSettings.ratio, 0.1f, 20.f);


	}
}

void ShadowData::CalculateCropMatrix(const std::vector<Mesh>& receivers, const std::vector<Mesh>& casters)
{
	AABB receiversBB, splitBB, castersBB[4];
	std::vector<Mesh> cast[4];
	//for (int j = 0; j < 4; j++)
	//{
		InstanceBounds frustumBounds0(shadowBounds.bounds[0]);
		InstanceBounds frustumBounds1(shadowBounds.bounds[1]);
		InstanceBounds frustumBounds2(shadowBounds.bounds[2]);
		InstanceBounds frustumBounds3(shadowBounds.bounds[3]);

		for (uint32_t i = 0; i < casters.size(); i++)
		{
			InstanceBounds objectBounds(casters[i].GetAABB());
			AABB objectBB = casters[i].GetAABB();

			if (objectBounds.SweepIntersection(frustumBounds0, LightDir))
			{
				cast[0].push_back(casters[i]);
				castersBB[0].MergeWith(CreateClipSpaceAABB(objectBB, View));
			}
			
			if (objectBounds.SweepIntersection(frustumBounds1, LightDir))
			{
				cast[1].push_back(casters[i]);
				castersBB[1].MergeWith(CreateClipSpaceAABB(objectBB, View));
			}
			
			if (objectBounds.SweepIntersection(frustumBounds2, LightDir))
			{
				cast[2].push_back(casters[i]);
				castersBB[2].MergeWith(CreateClipSpaceAABB(objectBB, View));
			}

			cast[3].push_back(casters[i]);
			castersBB[3].MergeWith(CreateClipSpaceAABB(objectBB, View));
			/*if (objectBounds.SweepIntersection(frustumBounds3, LightDir))
			{
			}*/
		}

//	}


	for (uint32_t i = 0; i < receivers.size(); i++)
	{

		receiversBB.MergeWith(CreateClipSpaceAABB(receivers[i].GetAABB(), View));
	}

	for (int i = 0; i < 4; i++)
	{
		splitBB = CreateClipSpaceAABB(shadowBounds.bounds[i], View);
		float minZ, maxZ;
		Vector4 transf;
		transf = View * shadowBounds.bounds[i].GetEndpoint(0);
		
		minZ = transf.z;
		maxZ = transf.z;

		for (int x = 1; x < 8; x++)
		{
			transf = View * shadowBounds.bounds[i].GetEndpoint(x);
			if (transf.z > maxZ) maxZ = transf.z;
			if (transf.z < minZ) minZ = transf.z;

		}

		//InstanceBounds receiversBounds(recieversBB);
		//InstanceBounds castersBounds(castersBB);

		//AABB finalSize = castersBB.Merge(recieversBB).Merge(splitBB);
		InstanceBounds bounds;
		bounds.bounds[0] = MAX(MAX(receiversBB.position.x, castersBB[i].position.x), splitBB.position.x);
		bounds.bounds[1] = MAX(MAX(receiversBB.position.y, castersBB[i].position.y), splitBB.position.y);
		bounds.bounds[2] = castersBB[i].position.z;
		bounds.bounds[3] = MIN(MIN(receiversBB.GetEnd().x, castersBB[i].GetEnd().x), splitBB.GetEnd().x);
		bounds.bounds[4] = MIN(IN(receiversBB.GetEnd().y, castersBB[i].GetEnd().y), splitBB.GetEnd().y);
		//bounds.bounds[2] = castersBB.position.z;
		bounds.bounds[5] = MIN(receiversBB.GetEnd().z, splitBB.GetEnd().z);
		
		if (transf.z + bounds.bounds[5] > maxZ) maxZ = transf.z + bounds.bounds[5];
	
		InstanceBounds frustumBounds(shadowBounds.bounds[i]);
		Proj[i] = Matrix4x4::CreateOrtho(-1.f, 1.f, -1.f, 1.f, castersBB[i].position.z + minZ, maxZ);

		float maxX = -1000.0f;
		float maxY = -1000.0f;
		float minX = 1000.0f;
		float minY = 1000.0f;

		for (int x = 0; x < 8; x++)
		{
			transf = Proj[i] * shadowBounds.bounds[i].GetEndpoint(x);
			
			transf.x /= transf.w;
			transf.y /= transf.w;

			if (transf.x > maxX) maxX = transf.x;
			if (transf.x < minX) minX = transf.x;
			if (transf.y > maxY) maxY = transf.y;
			if (transf.y < minY) minY = transf.y;
		}

		float scaleX = 2.0f / (bounds.bounds[3] - bounds.bounds[0]);
		float scaleY = 2.0f / (bounds.bounds[4] - bounds.bounds[1]);
		float offsetX = -0.5f * (bounds.bounds[3] + bounds.bounds[0]) * scaleX;
		float offsetY = -0.5f * (bounds.bounds[4] + bounds.bounds[1]) * scaleY;

		//CropMatrix[i] = Matrix4x4
		//				(scaleX, 0.0f, 0.0f, 0.0f,
		//				 0.0f, scaleY, 0.0f, 0.0f,
		//				 0.0f, 0.0f, 1.0f, 0.0f,
		//	             offsetX, offsetY, 0.0f, 1.f);

		Proj[i] = Matrix4x4::CreateOrtho(bounds.bounds[0], bounds.bounds[3], bounds.bounds[1], bounds.bounds[4], bounds.bounds[2], bounds.bounds[5]);
		Proj[1] = Matrix4x4::CreateOrtho(-160.f, 160.f,  -160.f, 160.f, -160.f, 160.f);

		//CropMatrix[i] = BuildCropMatrix(Vector3(bounds.bounds[0], bounds.bounds[1], bounds.bounds[2]), Vector3(bounds.bounds[3], bounds.bounds[4], bounds.bounds[5]));
	}
}

Matrix4x4 ShadowData::BuildCropMatrix(const Vector3& min, const Vector3& max)
{

	float scaleX, scaleY, scaleZ;
	float offsetX, offsetY, offsetZ;

	scaleX = 2.f / (max.x - min.x);
	scaleY = 2.f / (max.y - min.y);

	offsetX = -0.5f * (max.x + min.x) * scaleX;
	offsetY = -0.5f * (max.y + min.y) * scaleY;

	scaleZ = -2.f / (max.z - min.z);
	offsetZ = -min.z * scaleZ;

	return Matrix4x4
	(scaleX, 0.0f, 0.0f, 0.f,
		0.0f, scaleY, 0.0f, 0.0f,
		0.0f, 0.0f, 1.f, 0.0f,
		offsetX, offsetY, 0.f, 1.0f
	);
}

void ShadowData::UpdateView(Vector3 direction)
{
	LightDir = direction;
	direction = Vector3::Normalize(direction);
	float pitch = Math::ACos(Vector2::Length(Vector2(direction.x, direction.z)));
	float yaw = Math::Rad2deg((Math::ATan(direction.x / direction.z)));
	float yawRad = Math::Deg2Rad(yaw);
	const auto& center = -shadowBounds.GetCenter(0);
	View = Matrix4x4::Identity;
	View = Matrix4x4::Rotate(View, Vector3(1, 0, 0), pitch);
	yaw = direction.z > 0 ? yaw - 180 : yaw;
	View = Matrix4x4::Rotate(View, Vector3(0, 1, 0), -yawRad);

	View = Matrix4x4::CreateLookAt(-LightDir, Vector3(0, 0, 0), Vector3::Up());
	//iew = Matrix4x4::Translate(View, center);

}
