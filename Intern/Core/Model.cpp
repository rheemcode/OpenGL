#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Mesh.h"
#include "Model.h"

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

			for (size_t s = 0; s < shapes.size(); s++)
			{
				size_t indexOffset = 0;
				std::vector<VertexAttrib> vertexAttrib;
				std::vector<uint32_t> indices;

				for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
				{
					size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

					for (size_t v = 0; v < fv; v++)
					{
						tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];
						VertexAttrib vAttrib;
						vAttrib.vertices = { attribs.vertices[3 * size_t(idx.vertex_index) + 0], attribs.vertices[3 * size_t(idx.vertex_index) + 1], attribs.vertices[3 * size_t(idx.vertex_index) + 2] };

						if (idx.normal_index >= 0)
						{
							vAttrib.normals = { attribs.vertices[3 * size_t(idx.vertex_index) + 0], attribs.vertices[3 * size_t(idx.vertex_index) + 1], attribs.vertices[3 * size_t(idx.vertex_index) + 2] };
						}

						if (idx.texcoord_index >= 0)
						{
							vAttrib.uv = { attribs.texcoords[2 * size_t(idx.texcoord_index) + 0], attribs.texcoords[2 * size_t(idx.texcoord_index) + 1] };
						}
						vertexAttrib.push_back(vAttrib);
						indices.push_back(uint32_t(idx.vertex_index));
					}

					shapes[s].mesh.indices;
					indexOffset += fv;
				}

				Material material;
				p_model->AddMesh(Mesh(vertexAttrib, indices, material));
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
	m_meshes.push_back(std::move(p_mesh));
}

Model::Model()
{

}

Model::Model(std::string p_modelFilePath)
{
	ModelLoader loader;
	// Ahh Why
	loader.LoadModel(OBJ, p_modelFilePath, this);
}

Model::Model(std::string p_modelFilePath, MODEL_FORMAT p_modelFormat) 
{
	ModelLoader loader;
	loader.LoadModel(p_modelFormat, p_modelFilePath, this);
}
