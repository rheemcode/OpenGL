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

	bool LoadModel(MODEL_FORMAT modelFormat, const std::string& p_filePath, Model* p_model );
	bool LoadAsStaticModel(MODEL_FORMAT modelFormat, const std::string& p_filePath, Model* p_model);
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
	friend ModelLoader;
	friend class MeshRendererComponent;
	friend class StaticMeshRendererComponent;

	const Transform* m_transform = nullptr;

	std::vector<Mesh> m_meshes;
	std::shared_ptr<Texture> m_texture;
	std::vector<TextureNameMap> m_textureNames;

	std::weak_ptr<Texture> GetTexture() { return m_texture; }
	std::vector<Mesh>& GetMeshesRef() { return m_meshes; }
	void AddMesh(Mesh&& mesh);

public:
	const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
	const Transform& GetTransform() const { return *m_transform; }
	
	void ActiveTexture(uint32_t count)
	{
		m_texture->ActiveTexture(count);
	}

	void BindTexture(uint32_t id)
	{
		m_texture->Bind(id);
	}

	void BindTextures()
	{
		m_texture->BindAll();
	}
	void SetTransform(const Transform& p_transform) { m_transform = &p_transform; }
	void SetTextures(uint32_t count);

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