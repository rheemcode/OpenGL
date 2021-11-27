#pragma once
#include "Mesh.h"
#include "Console.h"


enum class ModelFormat
{
	FBX,
	OBJ,
	GLTF,
};

class Model;


class ModelLoader
{
	std::ofstream outputCacheStream;
	std::ifstream inputCacheStream;
	std::ifstream inputMaterialStream;
	std::ofstream outputMaterialStream;

	std::string modelName;

public:
	template <typename T>
	void WriteToCache(T* data, uint64_t count)
	{
		if (!outputCacheStream.is_open())
			outputCacheStream = std::ofstream(modelName + ".bin", std::ios::binary);

		outputCacheStream.write(reinterpret_cast<char*>((void*)data), sizeof(T) * count);
	}
	
	template <>
	void WriteToCache(Material* data, uint64_t count)
	{
		if (!outputMaterialStream.is_open())
		{
			outputMaterialStream = std::ofstream(modelName + ".mat", std::ios::binary);
		}
		
		for (int i = 0; i < count; ++i)
		{
			outputMaterialStream << data[i];
		}
	}

	void WriteToCache(uint64_t data, uint64_t count = 1)
	{
		if (!outputCacheStream.is_open())
			outputCacheStream = std::ofstream(modelName + ".bin", std::ios::binary);

		outputCacheStream.write(reinterpret_cast<char*>(&data), sizeof(uint64_t) * count);
	}

	template <typename T>
	void ReadFromCache(T* data, uint64_t count)
	{
		if (!inputCacheStream.is_open())
			inputCacheStream = std::ifstream(modelName + ".bin", std::ios::binary);

		
		inputCacheStream.read(reinterpret_cast<char*>(data), sizeof(T) * count);
	}
	
	template<>
	void ReadFromCache(Material* mat, uint64_t count)
	{
		if (!outputMaterialStream.is_open())
		{
			inputMaterialStream = std::ifstream(modelName + ".mat", std::ios::binary);
		}

		for (int i = 0; i < count; i++)
		{
			inputMaterialStream >> mat[i];
		}
	}
	
	void ReadFromCache(uint64_t* data, uint64_t count)
	{
		if (!inputCacheStream.is_open())
			inputCacheStream = std::ifstream(modelName + ".bin", std::ios::binary);

		
		inputCacheStream.read(reinterpret_cast<char*>(data), sizeof(uint64_t) * count);
	}
	
	void ReadFromCache(uint8_t* data, uint64_t count)
	{
		if (!inputCacheStream.is_open())
			inputCacheStream = std::ifstream(modelName + ".bin", std::ios::binary);

		
		inputCacheStream.read(reinterpret_cast<char*>(data), sizeof(uint64_t) * count);
	}
	
	void ReadFromCache(uint16_t* data, uint64_t count)
	{
		if (!inputCacheStream.is_open())
			inputCacheStream = std::ifstream(modelName + ".bin", std::ios::binary);

		
		inputCacheStream.read(reinterpret_cast<char*>(data), sizeof(uint64_t) * count);
	}

	bool LoadModelFromCache(Model* model);
	bool LoadModel(ModelFormat modelFormat, const std::string& p_filePath, Model* p_model);
	bool LoadAsStaticModel(ModelFormat modelFormat, const std::string& p_filePath, Model* p_model);
	void ComputeTangentBasis(VertexAttrib* attrib);
};

struct TextureNameMap
{
	int id = -1;
	bool loaded = false;
	std::string name;

	bool operator==(const TextureNameMap& p_other) const
	{
		return name  == name;
	}

	bool operator==(const std::string& p_name) const
	{
		return name  == p_name;
	}


};

class Model
{

public:

	enum class TextureType
	{
		TEX_DIFFUSE = 1,
		TEX_SPECULAR,
		TEX_NORMAL,
		TEX_BUMP,
		TEX_AMBIENT
	};

private:
	friend ModelLoader;
	friend class MeshRendererComponent;
	friend class StaticMeshRendererComponent;

	const Transform* m_transform = nullptr;

	std::vector<Mesh> m_meshes;
	std::vector<std::shared_ptr<Texture>> m_albedoTextures;
	std::vector<std::shared_ptr<Texture>> m_specularTextures;
	std::vector<std::shared_ptr<Texture>> m_normalMapTextures;
	std::vector<TextureNameMap> m_textureNames;
	std::vector<TextureNameMap> m_specularTextureNames;
	std::vector<TextureNameMap> m_normalTextureNames;

	std::weak_ptr<Texture> GetTexture(TextureType type, uint32_t id) 
	{
		switch (type)
		{
		case TextureType::TEX_DIFFUSE:
			return m_albedoTextures[id];
		case TextureType::TEX_SPECULAR:
			return m_specularTextures[id];
		case TextureType::TEX_NORMAL:
			return m_normalMapTextures[id];
		default:
			return std::weak_ptr<Texture>();
		}
	}


	std::vector<Mesh>& GetMeshesRef() { return m_meshes; }
	void AddMesh(Mesh&& mesh);

public:

	const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
	const Transform& GetTransform() const { return *m_transform; }

	/*void ActiveTexture(uint32_t id, TextureType type = TEX_DIFFUSE)
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
	}*/

	void BindTexture(uint32_t id, TextureType type = TextureType::TEX_DIFFUSE)
	{
		switch (type)
		{
		case TextureType::TEX_DIFFUSE:
			m_albedoTextures[id]->Bind();
			break;
		case TextureType::TEX_SPECULAR:
			m_specularTextures[id]->Bind();
			break;
		case TextureType::TEX_NORMAL:
			m_normalMapTextures[id]->Bind();
			break;
		default:
			BIND_DEFAULT_TEXTURE();
			break;
		}
	}

	void SetTransform(const Transform& p_transform) { m_transform = &p_transform; }
	
	void CreateAlbedoTextures(uint32_t count);
	void CreateSpecularTextures(uint32_t count);
	void CreateNormalTextures(uint32_t count);

	Model();
	Model(std::string p_modelFilePath);
	Model(std::string p_modelFilePath, ModelFormat p_modelFormat);
};

class StaticModel : public Model
{
	friend class StaticMeshRendererComponent;
public:

	StaticModel();
	StaticModel(std::string p_modelFilePath);
	StaticModel(std::string, ModelFormat p_modelFormat);
};

class ProcMesh
{
public:
	void CreateFromArray(VertexAttrib* attribs, uint32_t count) {};
};