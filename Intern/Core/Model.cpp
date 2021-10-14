#include <glpch.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Model.h"
#include "Components/MeshRendererComponent.h"
#include <filesystem>
#include <Profiler.h>


std::ostream& operator<<(std::ostream& os, Material& mat)
{
	std::ostream::sentry s(os);
	if (s)
	{
		int t = 0;
		os.write("Albedo", 7);
		os.write((char*)&mat.Albedo, sizeof(Vector3));
		os.write("Diffuse", 8);
		os.write((char*)&mat.Diffuse, sizeof(Vector3));
		os.write("Specular", 9);
		os.write((char*)&mat.Specular, sizeof(Vector3));
		os.write("Albedo_Texture", 15).write((char*)&mat.AlbedoTexture, 4);
		os.write("Specular_Texture",17 ).write((char*)& mat.SpecularTexture, 4);
		os.write("Normal_Texture" , 15).write((char*)&mat.NormalTexture, 4);
		os.write("NormalMap_Enabled", 18).write((char*)&mat.NormalMapEnabled, 1);
		os.write("Shininess", 10).write((char*)& mat.Shininess, 4);
		os.write("Specular_Highlight", 19).write((char*)&mat.SpecularHighlight, 4);
		os.write("Use_SSAO", 9).write( (char*)&mat.useSSAO, 1);

		t = mat.AlbedoTexName.size() + 1;
		std::string st = mat.AlbedoTexName + '\0';
		os .write( "AlbedoTexture_Name", 19).write((char*)&t, 4).write((char*)st.c_str(), mat.AlbedoTexName.size() + 1);
		t = mat.SpecularTexName.size() + 1;
		st = mat.SpecularTexName + '\0';
		os .write( "SpecularTexture_Name", 21).write((char*)&t, 4).write((char*)st.c_str(), mat.SpecularTexName.size() + 1);
		st = mat.NormalTexName + '\0';
		t = mat.NormalTexName.size() + 1;
		os .write( "NormalTexture_Name", 19).write((char*)&t, 4).write((char*)st.c_str(), mat.NormalTexName.size() + 1);
	}

	return os;
}

std::istream& operator>>(std::istream& is, Material& mat)
{
	std::istream::sentry s(is);
	if (s)
	{
		char* t = new char[25];
		is.read(t, 7);
		is.read((char*)&mat.Albedo, sizeof(Vector3));
		is.read(t, 8);
		is.read((char*)&mat.Diffuse, sizeof(Vector3));
		is.read(t, 9);
		is.read((char*)&mat.Specular, sizeof(Vector3));
		is.read(t, 15).read((char*)&mat.AlbedoTexture, 4);
		is.read(t, 17).read((char*)&mat.SpecularTexture, 4);
		is.read(t, 15).read((char*)&mat.NormalTexture, 4);
		is.read(t, 18).read((char*)&mat.NormalMapEnabled, 1);
		is.read(t, 10).read((char*)&mat.Shininess, 4);
		is.read(t, 19).read((char*)&mat.SpecularHighlight, 4);
		is.read(t, 9).read((char*)&mat.useSSAO, 1);

		int st = 0;
		is.read(t, 19).read((char*)&(st), 4);
		char* str = new char[st];
		is.read(str, st);
		mat.AlbedoTexName = str;
		delete[] str;

		is.read(t, 21).read((char*)&(st), 4);
		str = new char[st];
		is.read(str, st);
		mat.SpecularTexName = str;
		delete[] str;

		is.read(t, 19).read((char*)&(st), 4);
		str = new char[st];
		is.read(str, st);
		mat.NormalTexName = str;
		delete[] str;
	}

	return is;
}


bool ModelLoader::LoadModelFromCache(Model* p_model)
{

	// TODO: Reading just Vertex Attribs is probably better and then creating AABB and materials
	// programatically

		uint64_t meshCount = 0;

		ReadFromCache(&meshCount, 1);

		uint64_t albedoTexCount = 0;
		uint64_t specularTexCount = 0;
		uint64_t normalTexCount = 0;
		
		ReadFromCache(&albedoTexCount, 1);
		ReadFromCache(&specularTexCount, 1);
		ReadFromCache(&normalTexCount, 1);

		p_model->CreateAlbedoTextures(albedoTexCount);
		p_model->CreateSpecularTextures(specularTexCount);
		p_model->CreateNormalTextures(normalTexCount);


		TextureParameters texParam;
		texParam.dataFormat = TextureFormat::RGB;
		texParam.internalFormat = TextureFormat::RGB8;
		texParam.magFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
		texParam.minFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
		texParam.wrap = TextureWrap::REPEAT;

		std::string texDir = "./Assets/Textures/";
		for (int i = 1; i < albedoTexCount + 1; i++)
		{
			uint64_t texSize = 0;
			ReadFromCache(&texSize, 1);
			char* texPath = new char[texSize];
			ReadFromCache(texPath, texSize);

			auto tex = p_model->GetTexture(Model::TextureType::TEX_DIFFUSE, i).lock();
			tex->CreateFromFile(texPath, texParam);
			p_model->m_textureNames.push_back({ i, true, texPath });
			delete[] texPath;
		}
		
		for (int i = 1; i < specularTexCount + 1; i++)
		{
			uint64_t texSize = 0;
			ReadFromCache(&texSize, 1);
			char* texPath = new char[texSize];
			ReadFromCache(texPath, texSize);

			auto tex = p_model->GetTexture(Model::TextureType::TEX_SPECULAR, i).lock();
			tex->CreateFromFile(texPath, texParam);
			p_model->m_specularTextureNames.push_back({ i, true, texPath });
			delete[] texPath;
		}
		
		for (int i = 1; i < normalTexCount + 1; i++)
		{
			uint64_t texSize = 0;
			ReadFromCache(&texSize, 1);
			char* texPath = new char[texSize];
			ReadFromCache(texPath, texSize);

			auto tex = p_model->GetTexture(Model::TextureType::TEX_NORMAL, i).lock();
			tex->CreateFromFile(texPath, texParam);
			p_model->m_normalTextureNames.push_back({ i, true, texPath });
			delete[] texPath;
		}

		Material* material = new Material[meshCount];
		ReadFromCache(material, meshCount);
		for (uint64_t i = 0; i < meshCount; i++)
		{
			// Read number of vertex Attributes
			uint64_t vertexAttribCount = 0;
			ReadFromCache(&vertexAttribCount, 1);
			VertexAttrib* vertexAttrib = new VertexAttrib[vertexAttribCount];
			ReadFromCache(vertexAttrib, vertexAttribCount);

			uint64_t indicesCount = 0;
			ReadFromCache(&indicesCount, 1);
			uint32_t* indices = new uint32_t[indicesCount];
			ReadFromCache(indices, indicesCount);

			
			AABB* aabb = new AABB;
			ReadFromCache(aabb, 1);

			if (material[i].AlbedoTexName != "")
			{
				const auto& result = std::find(p_model->m_textureNames.begin(), p_model->m_textureNames.end(),  texDir + material[i].AlbedoTexName);
				if (result != std::end(p_model->m_textureNames))
				{
					material[i].AlbedoTexture = result->id;
				}
			}

			if (material[i].SpecularTexName != "")
			{
				const auto& result = std::find(p_model->m_specularTextureNames.begin(), p_model->m_specularTextureNames.end(), texDir + material[i].SpecularTexName);
				if (result != std::end(p_model->m_specularTextureNames))
				{
					material[i].SpecularTexture = result->id;
				}
			}

			if (material[i].NormalTexName != "")
			{
				const auto& result = std::find(p_model->m_normalTextureNames.begin(), p_model->m_normalTextureNames.end(), texDir + material[i].NormalTexName);
				if (result != std::end(p_model->m_normalTextureNames))
				{
					material[i].NormalTexture = result->id;
				}
			}

			Mesh mesh(vertexAttrib, indices, vertexAttribCount, material[i], *aabb);
			p_model->AddMesh(std::forward<Mesh>(mesh));

			delete[] vertexAttrib;
			delete[] indices;
			delete aabb;
		}
		delete[] material;

		return true;
}

bool ModelLoader::LoadModel(ModelFormat modelFormat, const std::string& p_filePath, Model* p_model)
{
	PROFILE_FUNCTION
	uint64_t start_pos;
	if ((start_pos = p_filePath.find_last_of("/\\")) != p_filePath.npos)
	{
		modelName = p_filePath.substr(start_pos + 1, p_filePath.size());
	}
	else
	{
		modelName = p_filePath;
	}

	{
		if (std::filesystem::exists(modelName + ".bin"))
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

		uint16_t albedoTextureCount = 0;
		uint16_t specularTextureCount = 0;
		uint16_t normalTextureCount = 0;

		// Load Textures
		for (int i = 0; i < materials.size(); i++)
		{
			if (materials[i].diffuse_texname != "")
			{
				auto result = std::find(std::begin(p_model->m_textureNames), std::end(p_model->m_textureNames), materials[i].diffuse_texname);

				if (result != p_model->m_textureNames.end())
					continue;

				albedoTextureCount++;
				p_model->m_textureNames.push_back({ -1, false, materials[i].diffuse_texname });
			}

			if (materials[i].specular_texname != "")
			{
				auto result = std::find(std::begin(p_model->m_specularTextureNames), std::end(p_model->m_specularTextureNames), materials[i].specular_texname);
				if (result != p_model->m_specularTextureNames.end())
					continue;

				specularTextureCount++;
				p_model->m_specularTextureNames.push_back({ -1, false, materials[i].specular_texname });
			}
			
			if (materials[i].bump_texname != "")
			{
				auto result = std::find(std::begin(p_model->m_normalTextureNames), std::end(p_model->m_normalTextureNames), materials[i].bump_texname);
				if (result != p_model->m_normalTextureNames.end())
					continue;

				normalTextureCount++;
				p_model->m_normalTextureNames.push_back({ -1, false, materials[i].bump_texname });
			}
		}

		WriteToCache(albedoTextureCount, 1);
		p_model->CreateAlbedoTextures(albedoTextureCount);
		WriteToCache(specularTextureCount, 1);
		p_model->CreateSpecularTextures(specularTextureCount);
		WriteToCache(normalTextureCount);
		p_model->CreateNormalTextures(normalTextureCount);
		

		TextureParameters texParam;
		texParam.dataFormat = TextureFormat::RGB;
		texParam.internalFormat = TextureFormat::RGB8;
		texParam.magFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
		texParam.minFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
		texParam.wrap = TextureWrap::REPEAT;

		
		for (int i = 0; i < int(albedoTextureCount); i++)
		{
			auto tex = p_model->GetTexture(Model::TextureType::TEX_DIFFUSE, i).lock();
			std::string texPath = "./Assets/Textures/" + p_model->m_textureNames[i].name;
			WriteToCache(uint16_t(texPath.size() + 1), 1);
			WriteToCache((texPath + "\0").c_str(), texPath.size() + 1);
			tex->CreateFromFile(texPath, texParam);
			p_model->m_textureNames[i].loaded = true;
			p_model->m_textureNames[i].id = i;
		}
		

		for (int i = 0; i < int(specularTextureCount); i++)
		{
			int index = i;
			auto tex = p_model->GetTexture(Model::TextureType::TEX_SPECULAR, i).lock();
			std::string texPath = "./Assets/Textures/" + p_model->m_specularTextureNames[index].name;
			WriteToCache(uint16_t(texPath.size() + 1), 1);
			WriteToCache((texPath + "\0").c_str(), texPath.size() + 1);
			tex->CreateFromFile(texPath, texParam);
			p_model->m_specularTextureNames[index].loaded = true;
			p_model->m_specularTextureNames[index].id = i;
		}

		
		for (int i = 0; i < int(normalTextureCount); i++)
		{
			int index = i;
			auto tex = p_model->GetTexture(Model::TextureType::TEX_NORMAL, i).lock();
			std::string texPath = "./Assets/Textures/" + p_model->m_normalTextureNames[index].name;
			WriteToCache(uint16_t(texPath.size() + 1), 1);
			WriteToCache((texPath + "\0").c_str(), texPath.size() + 1);
			tex->CreateFromFile(texPath, texParam);
			p_model->m_normalTextureNames[index].loaded = true;
			p_model->m_normalTextureNames[index].id = i;
		}

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
						vertexAttribs.push_back(vAttrib[0]);
						vertexAttribs.push_back(vAttrib[1]);
						vertexAttribs.push_back(vAttrib[2]);
					}


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
				const auto& albedoTex = materials[shapes[s].mesh.material_ids[0]].diffuse_texname;
				const auto& specularTex = materials[shapes[s].mesh.material_ids[0]].specular_texname;
				const auto& normalTex = materials[shapes[s].mesh.material_ids[0]].bump_texname;
				
				if (albedoTex != "")
				{
					auto result = std::find(std::begin(p_model->m_textureNames), std::end(p_model->m_textureNames), albedoTex);
					if (result != std::end(p_model->m_textureNames) && result->loaded)
					{
						material.AlbedoTexture = result->id;
						material.AlbedoTexName = albedoTex;
					}
				}
				
				if (specularTex != "")
				{
					auto result = std::find(std::begin(p_model->m_specularTextureNames), std::end(p_model->m_specularTextureNames), specularTex);
					if (result != std::end(p_model->m_specularTextureNames) && result->loaded)
					{
						material.SpecularTexture = result->id;
						material.SpecularTexName = specularTex;
					}
				}
				
				if (normalTex != "")
				{
					auto result = std::find(std::begin(p_model->m_normalTextureNames), std::end(p_model->m_normalTextureNames), normalTex);
					if (result != std::end(p_model->m_normalTextureNames) && result->loaded)
					{
						material.NormalTexture = result->id;
						material.NormalTexName = normalTex;
						material.NormalMapEnabled = true;
					}
				}

				material.Diffuse = { materials[shapes[s].mesh.material_ids[0]].diffuse[0], materials[shapes[s].mesh.material_ids[0]].diffuse[1], materials[shapes[s].mesh.material_ids[0]].diffuse[2], 1.f };
				material.Albedo = { materials[shapes[s].mesh.material_ids[0]].ambient[0], materials[shapes[s].mesh.material_ids[0]].ambient[1], materials[shapes[s].mesh.material_ids[0]].ambient[2], 1.f };
				material.Shininess = materials[shapes[s].mesh.material_ids[0]].shininess;
				material.SpecularHighlight = materials[shapes[s].mesh.material_ids[0]].specular[0];
			}

			WriteToCache(vertexAttribs.size());
			WriteToCache(vertexAttribs.data(), vertexAttribs.size());
			WriteToCache(vertexAttribCount);
			WriteToCache(indices, vertexAttribCount);
			WriteToCache(&boundindBox, 1);
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

	Vector2 deltaUV1 = attrib[1].uv - attrib[0].uv;
	Vector2 deltaUV2 = attrib[2].uv - attrib[0].uv;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

	
	if (isinf(r))
	{
		Vector3 tan = Vector3(-1.f, 0, 0);
		Vector3 bitan = Vector3(1.f, 0, 0);
		for (int i = 0; i < 3; i++)
		{
			attrib[i].tangent = tan;
			attrib[i].bitangent = bitan;
		}
		return;
	}

	Vector3 tangent = Vector3::Normalize((deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r);
	Vector3 bitangent = Vector3::Normalize((deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r);
	
	
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

		// TODO: not just albedo textures;
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
				const auto& albedoTex = materials[shapes[s].mesh.material_ids[0]].diffuse_texname;
				if (albedoTex != "")
				{
					auto result = std::find(std::begin(p_model->m_textureNames), std::end(p_model->m_textureNames), albedoTex);
					if (result != std::end(p_model->m_textureNames) && result->loaded)
					{
						//material->Diffuse = result->id;
						//material->Color = { materials[shapes[s].mesh.material_ids[0]].albedo[0], materials[shapes[s].mesh.material_ids[0]].albedo[1], materials[shapes[s].mesh.material_ids[0]].albedo[2], 1.f };
					}
				}

				else
				{
					//	material->Diffuse = -1;
					//	material->Color = { materials[shapes[s].mesh.material_ids[0]].albedo[0], materials[shapes[s].mesh.material_ids[0]].albedo[1], materials[shapes[s].mesh.material_ids[0]].albedo[2], 1.f };
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


void Model::CreateAlbedoTextures(uint32_t count)
{
	m_albedoTextures.push_back(Texture::s_defaultTexture);
	while (count)
	{
		auto tex = std::make_shared<Texture2D>();
		m_albedoTextures.push_back(tex);
		--count;
	}
}


void Model::CreateSpecularTextures(uint32_t count)
{
	m_specularTextures.push_back(Texture::s_defaultSpecularTexture);
	while (count)
	{
		auto tex = std::make_shared<Texture2D>();
		m_specularTextures.push_back(tex);
		--count;
	}
}

void Model::CreateNormalTextures(uint32_t count)
{
	m_normalMapTextures.push_back(Texture::s_defaultNormalTexture);
	while (count)
	{
		auto tex = std::make_shared<Texture2D>();
		m_normalMapTextures.push_back(tex);
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