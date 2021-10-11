#include <glpch.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Model.h"
#include "Components/MeshRendererComponent.h"
#include <filesystem>
#include <Profiler.h>

void CreateModelCache()
{

}

bool ModelLoader::LoadModelFromCache(Model* p_model)
{
		uint64_t meshCount = 0;
		ReadFromCache(&meshCount, 1);

		uint64_t texCount = 0;
		ReadFromCache(&texCount, 1);

		p_model->CreateDiffuseTextures(texCount);


		TextureParameters texParam;
		texParam.dataFormat = TextureFormat::RGB;
		texParam.internalFormat = TextureFormat::RGB8;
		texParam.magFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
		texParam.minFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
		texParam.wrap = TextureWrap::REPEAT;


		for (int i = 0; i < texCount; i++)
		{
			uint64_t texSize = 0;
			ReadFromCache(&texSize, 1);
			char* texPath = new char[texSize];
			ReadFromCache(texPath, texSize);

			auto tex = p_model->GetTexture(Model::TEX_DIFFUSE, i).lock();
			tex->CreateFromFile(texPath, texParam);
			delete[] texPath;
		}

		for (uint64_t i = 0; i < meshCount; i++)
		{
			uint64_t vertexAttribCount = 0;
			ReadFromCache(&vertexAttribCount, 1);
			VertexAttrib* vertexAttrib = new VertexAttrib[vertexAttribCount];
			ReadFromCache(vertexAttrib, vertexAttribCount);
			uint64_t count = 0;
			ReadFromCache(&count, 1);
			uint32_t* indices = new uint32_t[count];
			ReadFromCache(indices, count);
			AABB* aabb = new AABB;
			Material* material = new Material;
			ReadFromCache(aabb, 1);
			ReadFromCache(material, 1);

			Mesh mesh(vertexAttrib, indices, vertexAttribCount, *material, *aabb);
			p_model->AddMesh(std::forward<Mesh>(mesh));

			delete[] vertexAttrib;
			delete[] indices;
			delete aabb;
			delete material;
		}

		return true;
}

bool ModelLoader::LoadModel(ModelFormat modelFormat, const std::string& p_filePath, Model* p_model)
{
	PROFILE_FUNCTION
	uint64_t start_pos;
	if ((start_pos = p_filePath.find_last_of("/\\")) != p_filePath.npos)
	{
		modelName = p_filePath.substr(start_pos + 1, p_filePath.size()) + ".bin";
	}
	else
	{
		modelName = p_filePath + ".bin";
	}

	{
		if (std::filesystem::exists(modelName))
		{
			return LoadModelFromCache(p_model);
		}
	}
	
	switch (modelFormat)
	{
	case ModelFormat::OBJ:
	{


		tinyobj::ObjReaderConfig readerConfig;
		readerConfig.mtl_search_path = "./Assets/";
		readerConfig.triangulate = true;
		readerConfig.vertex_color = false;

		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(p_filePath.data(), readerConfig))
		{
			if (!reader.Error().empty())
				Console::Log(LogMode::ERROR, reader.Error());
			return false;
		}

		if (!reader.Warning().empty())
			Console::Log(LogMode::ERROR, reader.Warning());

		auto& attribs = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();
		
		WriteToCache(shapes.size());

		uint32_t diffuseTextureCount = 0;
		uint32_t specularTextureCount = 0;

		// TODO: not just diffuse textures;
		for (int i = 0; i < materials.size(); i++)
		{
			if (materials[i].diffuse_texname != "")
			{
				auto result = std::find(std::begin(p_model->m_textureNames), std::end(p_model->m_textureNames), materials[i].diffuse_texname);

				if (result != p_model->m_textureNames.end())
					continue;

				diffuseTextureCount++;
				p_model->m_textureNames.push_back({ -1, false, materials[i].diffuse_texname });
			}
		}
		WriteToCache(diffuseTextureCount, 1);
		p_model->CreateDiffuseTextures(diffuseTextureCount);
		//p_model->CreateSpecularTextures(textureCount);

		TextureParameters texParam;
		texParam.dataFormat = TextureFormat::RGB;
		texParam.internalFormat = TextureFormat::RGB8;
		texParam.magFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
		texParam.minFilter = TextureFilter::NEAREST;
		texParam.wrap = TextureWrap::REPEAT;

		for (int i = 0; i < int(diffuseTextureCount); i++)
		{
			auto tex = p_model->GetTexture(Model::TEX_DIFFUSE, i).lock();
			std::string texPath = "./Assets/Textures/" + p_model->m_textureNames[i].name;
			WriteToCache(texPath.size() + 1, 1);
			WriteToCache((texPath + "\0").c_str(), texPath.size() + 1);
			tex->CreateFromFile(texPath, texParam);
			p_model->m_textureNames[i].loaded = true;
			p_model->m_textureNames[i].id = i;
		}
		
		//WriteToCache("Mesh_Count", 10);

		for (size_t s = 0; s < shapes.size(); s++)
		{
			
			size_t indexOffset = 0;
			const int vertexAttribCount = (int)shapes[s].mesh.indices.size();
			
			std::vector<VertexAttrib> vertexAttribs;
			
			uint32_t* indices  = new uint32_t[vertexAttribCount];
			Vector3* tangent   = new Vector3[vertexAttribCount];
			Vector3* bitangent = new Vector3[vertexAttribCount];

			std::array<VertexAttrib, 3> vAttrib;
			std::unordered_map<VertexAttrib, uint32_t> uniqueVertices;

			AABB boundindBox;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

				for (size_t v = 0; v < fv; v++)
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];

					vAttrib[v].vertices = { attribs.vertices[3 * size_t(idx.vertex_index) + 0], attribs.vertices[3 * size_t(idx.vertex_index) + 1], attribs.vertices[3 * size_t(idx.vertex_index) + 2] };
					//vAttrib[v].vertices /= 10.f;
					if (f == 0)
						boundindBox.position = vAttrib[v].vertices;
					boundindBox.ExpandTo(vAttrib[v].vertices);


					if (idx.normal_index >= 0)
					{
						vAttrib[v].normals = { attribs.normals[3 * size_t(idx.normal_index) + 0], attribs.normals[3 * size_t(idx.normal_index) + 1], attribs.normals[3 * size_t(idx.normal_index) + 2] };
					}

					if (idx.texcoord_index >= 0)
					{
						vAttrib[v].uv = { attribs.texcoords[2 * size_t(idx.texcoord_index) + 0], attribs.texcoords[2 * size_t(idx.texcoord_index) + 1] };
					}

					if (v == fv - 1)
					{
						ComputeTangentBasis(vAttrib.data());
					}


					vertexAttribs.push_back(vAttrib[v]);
					if (uniqueVertices.count(vAttrib[v]) == 0)
					{
			
						uniqueVertices[vAttrib[v]] = (uint32_t)(indexOffset + v);
					}

					else
					{
						
						vAttrib[v].tangent +=  vAttrib[v].tangent;
						vAttrib[v].bitangent += vAttrib[v].bitangent;
						
					}

					indices[indexOffset + v] = uniqueVertices[vAttrib[v]];
					tangent[indexOffset + v] = vAttrib[v].tangent;
					bitangent[indexOffset + v] = vAttrib[v].bitangent;
				}

				shapes[s].mesh.indices;
				indexOffset += fv;
			}


			Material material;
			material.Diffuse = -1;
			if (shapes[s].mesh.material_ids[0] >= 0)
			{
				const auto& diffuseTex = materials[shapes[s].mesh.material_ids[0]].diffuse_texname;
				if (diffuseTex != "")
				{
					auto result = std::find(std::begin(p_model->m_textureNames), std::end(p_model->m_textureNames), diffuseTex);
					if (result != std::end(p_model->m_textureNames) && result->loaded)
					{
						material.Diffuse = result->id;
						material.Color = { materials[shapes[s].mesh.material_ids[0]].diffuse[0], materials[shapes[s].mesh.material_ids[0]].diffuse[1], materials[shapes[s].mesh.material_ids[0]].diffuse[2], 1.f };
					}
				}

				else
				{
					material.Diffuse = -1;
					material.Color = { materials[shapes[s].mesh.material_ids[0]].diffuse[0], materials[shapes[s].mesh.material_ids[0]].diffuse[1], materials[shapes[s].mesh.material_ids[0]].diffuse[2], 1.f };
				}
				material.Shininess = materials[shapes[s].mesh.material_ids[0]].shininess;
				material.SpecularHighlights = materials[shapes[s].mesh.material_ids[0]].specular[0];
			}

			else
			{
				material.Diffuse = -1;
			}


		//	WriteToCache("vertex_attrib", 13);
			WriteToCache(vertexAttribs.size());
			WriteToCache(vertexAttribs.data(), vertexAttribs.size());
		//	WriteToCache("indices", 7);
			WriteToCache(vertexAttribCount);
			WriteToCache(indices, vertexAttribCount);
		//	WriteToCache("boundind_box", 12);
			WriteToCache(&boundindBox, 1);
		//	WriteToCache("material", 8);
			WriteToCache(&material, 1);
			Mesh mesh(vertexAttribs, indices, vertexAttribCount, material, boundindBox);

			p_model->AddMesh(std::forward<Mesh>(mesh));
			vertexAttribs.clear();

			delete[] indices;
			delete[] tangent;
			delete[] bitangent;
		}

		return true;
	}

	default:
	{
		return false;
	}

	}
}

void ModelLoader::ComputeTangentBasis(VertexAttrib* attrib)
{
	Vector3 deltaPos1 = attrib[1].vertices - attrib[0].vertices;
	Vector3 deltaPos2 = attrib[2].vertices - attrib[0].vertices;

	Vector3 deltaUV1 = attrib[1].uv - attrib[0].uv;
	Vector3 deltaUV2 = attrib[2].uv - attrib[0].uv;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

	
	if (isinf(r))
		return;

	Vector3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
	Vector3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
	
	
	for (int i = 0; i < 3; i++)
	{
		attrib[i].tangent = tangent;
		attrib[i].bitangent = bitangent;
	}
}

bool ModelLoader::LoadAsStaticModel(ModelFormat modelFormat, const std::string& p_filePath, Model* p_model)
{
	switch (modelFormat)
	{
	case ModelFormat::OBJ:
	{
		tinyobj::ObjReaderConfig readerConfig;
		readerConfig.mtl_search_path = "./Assets/";
		readerConfig.triangulate = true;
		readerConfig.vertex_color = false;

		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(p_filePath.data(), readerConfig))
		{
			if (!reader.Error().empty())
				Console::Log(LogMode::ERROR, reader.Error());
			return false;
		}

		if (!reader.Warning().empty())
			Console::Log(LogMode::ERROR, reader.Warning());

		auto& attribs = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();

		uint32_t textureCount = 0;

		// TODO: not just diffuse textures;
		for (int i = 0; i < materials.size(); i++)
		{
			if (materials[i].diffuse_texname != "")
			{
				auto result = std::find(std::begin(p_model->m_textureNames), std::end(p_model->m_textureNames), materials[i].diffuse_texname);

				if (result != p_model->m_textureNames.end())
					continue;

				textureCount++;
				p_model->m_textureNames.push_back({ -1, false, materials[i].diffuse_texname });
			}
		}
	//	p_model->SetTextures(textureCount);

		for (int i = 0; i < materials.size(); i++)
		{
			if (materials[i].diffuse_texname != "")
			{
				auto result = std::find(std::begin(p_model->m_textureNames), std::end(p_model->m_textureNames), materials[i].diffuse_texname);

				if (result != p_model->m_textureNames.end() && !result->loaded)
				{
					//auto texptr = p_model->GetTexture().lock();
					//result->id = texptr->AddImage("./Assets/Textures/" + result->name);
					//result->loaded = true;
				}

			}
		}

		int totalVeticesCount = 0;

		for (size_t s = 0; s < shapes.size(); s++)
		{
			totalVeticesCount += (int)shapes[s].mesh.indices.size();
		}

		VertexAttrib* vertexAttribs = new VertexAttrib[totalVeticesCount];
		uint32_t* indices = new uint32_t[totalVeticesCount];

		AABB totalBoundingBox;
		int offset = 0;
		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t indexOffset = 0;
			int nextIndexOffset = shapes[s].mesh.indices.size();

			std::unordered_map<VertexAttrib, uint32_t> uniqueVertices;
			AABB boundindBox;


			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

				for (size_t v = 0; v < fv; v++)
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];
					VertexAttrib vAttrib;

					vAttrib.vertices = { attribs.vertices[3 * size_t(idx.vertex_index) + 0], attribs.vertices[3 * size_t(idx.vertex_index) + 1], attribs.vertices[3 * size_t(idx.vertex_index) + 2] };
		//			vAttrib.vertices *= 5.f;
					if (f == 0)
					{
						//totalBoundingBox.position = vAttrib.vertices;
						boundindBox.position = vAttrib.vertices;
					}

					boundindBox.ExpandTo(vAttrib.vertices);


					if (idx.normal_index >= 0)
					{
						vAttrib.normals = Vector3::Normalize({ attribs.normals[3 * size_t(idx.normal_index) + 0], attribs.normals[3 * size_t(idx.normal_index) + 1], attribs.normals[3 * size_t(idx.normal_index) + 2] });
					}

					if (idx.texcoord_index >= 0)
					{
						vAttrib.uv = { attribs.texcoords[2 * size_t(idx.texcoord_index) + 0], attribs.texcoords[2 * size_t(idx.texcoord_index) + 1] };
					}

					if (uniqueVertices.count(vAttrib) == 0)
					{
						uniqueVertices[vAttrib] = (uint32_t)(indexOffset + v + offset);
						vertexAttribs[indexOffset + v + offset] = vAttrib;
					}

					indices[indexOffset + v + offset] = uniqueVertices[vAttrib];
				}

				shapes[s].mesh.indices;
				indexOffset += fv;
			}

			offset += nextIndexOffset;

			if (shapes[s].mesh.material_ids[0] >= 0)
			{
				const auto& diffuseTex = materials[shapes[s].mesh.material_ids[0]].diffuse_texname;
				if (diffuseTex != "")
				{
					auto result = std::find(std::begin(p_model->m_textureNames), std::end(p_model->m_textureNames), diffuseTex);
					if (result != std::end(p_model->m_textureNames) && result->loaded)
					{
						//material->Diffuse = result->id;
						//material->Color = { materials[shapes[s].mesh.material_ids[0]].diffuse[0], materials[shapes[s].mesh.material_ids[0]].diffuse[1], materials[shapes[s].mesh.material_ids[0]].diffuse[2], 1.f };
					}
				}

				else
				{
					//	material->Diffuse = -1;
					//	material->Color = { materials[shapes[s].mesh.material_ids[0]].diffuse[0], materials[shapes[s].mesh.material_ids[0]].diffuse[1], materials[shapes[s].mesh.material_ids[0]].diffuse[2], 1.f };
					//}
					//material->Shininess = materials[shapes[s].mesh.material_ids[0]].shininess;
					//material->SpecularHighlights = materials[shapes[s].mesh.material_ids[0]].specular[0];
				}

				//else
				//{
				//	material->Diffuse = -1;
				//}
			}

			totalBoundingBox.MergeWith(boundindBox);
		}

		Material material;
		material.Diffuse = -1;
		Mesh mesh(vertexAttribs, indices, totalVeticesCount, material, totalBoundingBox);

		p_model->AddMesh(std::forward<Mesh>(mesh));
		delete[] vertexAttribs;
		delete[] indices;

		return true;
	}

	default:
	{
		return false;
	}
	}
}

void Model::AddMesh(Mesh&& p_mesh)
{

	m_meshes.insert(m_meshes.begin(), std::move(p_mesh));
	///	m_meshes.push_back(std::move(p_mesh));
}


void Model::CreateDiffuseTextures(uint32_t count)
{

	while (count)
	{
		auto tex = std::make_shared<Texture2D>();
		m_diffuseTextures.push_back(tex);
		--count;
	}
}


void Model::CreateSpecularTextures(uint32_t count)
{
	while (count)
	{
		auto tex = std::make_shared<Texture2D>();
		m_specularTextures.push_back(tex);
		--count;
	}
}

Model::Model()
{

}

Model::Model(std::string p_modelFilePath)
{

	ModelLoader loader;
	loader.LoadModel(ModelFormat::OBJ, p_modelFilePath, this);
}

Model::Model(std::string p_modelFilePath, ModelFormat p_modelFormat)
{
	ModelLoader loader;
	loader.LoadModel(p_modelFormat, p_modelFilePath, this);
}

StaticModel::StaticModel()
	: Model()
{

}

StaticModel::StaticModel(std::string p_modelFilePath)
	: Model()
{
	ModelLoader loader;
	loader.LoadAsStaticModel(ModelFormat::OBJ, p_modelFilePath, this);
}

StaticModel::StaticModel(std::string, ModelFormat p_modelFormat)
	: Model()
{

}