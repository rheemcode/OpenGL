#pragma once
#include "Mesh.h"
#include "Console.h"


enum MODEL_FORMAT
{
	FBX,
	OBJ,
	GLTF,
};

class Model;


class ModelLoader
{
public:

	bool LoadModel(MODEL_FORMAT modelFormat, const std::string& p_filePath, Model* p_model);
	bool LoadAsStaticModel(MODEL_FORMAT modelFormat, const std::string& p_filePath, Model* p_model);
	void ComputeTangentBasis(VertexAttrib* attrib);
};

struct TextureNameMap
{
	int id = -1;
	bool loaded = false;
	std::string name;

	bool operator==(const TextureNameMap& p_other) const
	{
		return name == name;
	}

	bool operator==(const std::string& p_name) const
	{
		return name == p_name;
	}


};

class Model
{

public:

	enum TextureType
	{
		TEX_SHADOW,
		TEX_DIFFUSE = 1,
		TEX_SPECULAR,
		TEX_AMBIENT,
		TEX_NORMAL,
		TEX_BUMP
	};

private:
	friend ModelLoader;
	friend class MeshRendererComponent;
	friend class StaticMeshRendererComponent;

	const Transform* m_transform = nullptr;

	std::vector<Mesh> m_meshes;
	std::vector<std::shared_ptr<Texture>> m_diffuseTextures;
	std::vector<std::shared_ptr<Texture>> m_specularTextures;
	std::vector<TextureNameMap> m_textureNames;

	std::weak_ptr<Texture> GetTexture(TextureType type, uint32_t id) 
	{
		switch (type)
		{
		case Model::TEX_DIFFUSE:
			return m_diffuseTextures[id];
		case Model::TEX_SPECULAR:
			return m_specularTextures[id];
		default:
			return std::weak_ptr<Texture>();
		}
	}


	std::vector<Mesh>& GetMeshesRef() { return m_meshes; }
	void AddMesh(Mesh&& mesh);

public:

	const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
	const Transform& GetTransform() const { return *m_transform; }

	void ActiveTexture(uint32_t id, TextureType type = TEX_DIFFUSE)
	{
		switch (type)
		{
		case Model::TEX_DIFFUSE:
			m_diffuseTextures[id]->ActiveTexture(TEX_DIFFUSE);
			break;
		case Model::TEX_SPECULAR:
			m_specularTextures[id]->ActiveTexture(TEX_SPECULAR);
			break;
		default:
			break;
		}
	}

	void BindTexture(uint32_t id, TextureType type = TEX_DIFFUSE)
	{
		switch (type)
		{
		case Model::TEX_DIFFUSE:
			m_diffuseTextures[id]->Bind(0);
			break;
		case Model::TEX_SPECULAR:
			m_specularTextures[id]->Bind(0);
			break;
		default:
			break;
		}
	}

	void BindTextures()
	{
		m_diffuseTextures[0]->ActiveTexture(TEX_DIFFUSE);
		m_diffuseTextures[0]->Bind(1);
		m_specularTextures[0]->ActiveTexture(TEX_SPECULAR);
		m_specularTextures[0]->Bind(1);
	}
	void SetTransform(const Transform& p_transform) { m_transform = &p_transform; }
	
	void CreateDiffuseTextures(uint32_t count);
	void CreateSpecularTextures(uint32_t count);

	Model();
	Model(std::string p_modelFilePath);
	Model(std::string p_modelFilePath, MODEL_FORMAT p_modelFormat);
};

class StaticModel : public Model
{
	friend class StaticMeshRendererComponent;
public:

	StaticModel();
	StaticModel(std::string p_modelFilePath);
	StaticModel(std::string, MODEL_FORMAT p_modelFormat);
};

class ProcMesh
{
public:
	void CreateFromArray(VertexAttrib* attribs, uint32_t count) {};
};