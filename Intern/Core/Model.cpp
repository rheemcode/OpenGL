#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Model.h"
#include "Components/MeshRendererComponent.h"

#include <algorithm>
#include <unordered_map>

bool ModelLoader::LoadModel(MODEL_FORMAT modelFormat, std::string_view p_filePath, Model* p_model)
{
	switch (modelFormat)
	{
		case OBJ:
		{
			tinyobj::ObjReaderConfig readerConfig;
			readerConfig.mtl_search_path = "./";
			readerConfig.triangulate = true;
			readerConfig.vertex_color = false;

			tinyobj::ObjReader reader;

			if (!reader.ParseFromFile(p_filePath.data(), readerConfig))
			{
				if (!reader.Error().empty())
					Console::Log(reader.Error());
				return false;
			}

			if (!reader.Warning().empty())
				Console::Log(reader.Warning());

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
					p_model->m_textureNames.push_back({-1, false, materials[i].diffuse_texname });
				}
			}
			p_model->SetTextures(textureCount);

			for (int i = 0; i < materials.size(); i++)
			{
				if (materials[i].diffuse_texname != "")
				{
					auto result = std::find(std::begin(p_model->m_textureNames), std::end(p_model->m_textureNames), materials[i].diffuse_texname);

					if (result != p_model->m_textureNames.end() && !result->loaded)
					{
						auto texptr = p_model->GetTexture().lock();
						result->id = texptr->AddImage("C:\\Users\\rheen\\source\\repos\\OpenGL\\" + result->name);
						result->loaded = true;
					}

				}
			}

			for (size_t s = 0; s < shapes.size(); s++)
			{
				size_t indexOffset = 0;
				const int size = shapes[s].mesh.indices.size();
				VertexAttrib* vertexAttribs = new VertexAttrib[size];
				uint32_t* indices = new uint32_t[size];
				
				std::unordered_map<VertexAttrib, uint32_t> uniqueVertices;
				
				std::unique_ptr<AABB> boundindBox = std::make_unique<AABB>();
				for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
				{
					size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

					for (size_t v = 0; v < fv; v++)
					{
						tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];
						VertexAttrib vAttrib;
						
						vAttrib.vertices = { attribs.vertices[3 * size_t(idx.vertex_index) + 0], attribs.vertices[3 * size_t(idx.vertex_index) + 1], attribs.vertices[3 * size_t(idx.vertex_index) + 2] };
						if (f == 0)
							boundindBox->position = vAttrib.vertices;
						boundindBox->ExpandTo(vAttrib.vertices);


						if (idx.normal_index >= 0)
						{
							vAttrib.normals = { attribs.vertices[3 * size_t(idx.vertex_index) + 0], attribs.vertices[3 * size_t(idx.vertex_index) + 1], attribs.vertices[3 * size_t(idx.vertex_index) + 2] };
						}

						if (idx.texcoord_index >= 0)
						{
							vAttrib.uv = { attribs.texcoords[2 * size_t(idx.texcoord_index) + 0], attribs.texcoords[2 * size_t(idx.texcoord_index) + 1] };
						}

						if (uniqueVertices.count(vAttrib) == 0)
						{
							uniqueVertices[vAttrib] = indexOffset + v;
							vertexAttribs[indexOffset + v] = vAttrib;
						}

						indices[indexOffset + v] = uniqueVertices[vAttrib];
					}

					shapes[s].mesh.indices;
					indexOffset += fv;
				}


				std::unique_ptr<Material> material = std::make_unique<Material>();
				
				if (shapes[s].mesh.material_ids[0] >= 0)
				{
					const auto& diffuseTex = materials[shapes[s].mesh.material_ids[0]].diffuse_texname;
					if (diffuseTex != "")
					{
						auto result = std::find(std::begin(p_model->m_textureNames), std::end(p_model->m_textureNames), diffuseTex);
						if (result != std::end(p_model->m_textureNames) && result->loaded)
						{
							material->Diffuse = result->id;
							material->Color = { materials[shapes[s].mesh.material_ids[0]].diffuse[0], materials[shapes[s].mesh.material_ids[0]].diffuse[1], materials[shapes[s].mesh.material_ids[0]].diffuse[2], 1.f };
						}
					}

					else
					{
						material->Diffuse = -1;
						material->Color = { materials[shapes[s].mesh.material_ids[0]].diffuse[0], materials[shapes[s].mesh.material_ids[0]].diffuse[1], materials[shapes[s].mesh.material_ids[0]].diffuse[2], 1.f };
					}
					material->Shininess = materials[shapes[s].mesh.material_ids[0]].shininess;
					material->SpecularHighlights = materials[shapes[s].mesh.material_ids[0]].specular[0];
				}
		


				Mesh mesh(vertexAttribs, indices, size, material, boundindBox);
				p_model->AddMesh(std::forward<Mesh>(mesh));
				delete[] vertexAttribs;
				delete[] indices;
			}

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


void Model::SetTextures(uint32_t count)
{
	m_texture = std::make_shared<Texture>(count);
}

Model::Model()
{

}

Model::Model(std::string p_modelFilePath)
{

	ModelLoader loader;
	loader.LoadModel(OBJ, p_modelFilePath, this);
}

Model::Model(std::string p_modelFilePath, MODEL_FORMAT p_modelFormat) 
{
	ModelLoader loader;
	loader.LoadModel(p_modelFormat, p_modelFilePath, this);
}

