#pragma once
#include "Mesh.h"
#include "Console.h"


enum MODEL_FORMAT
{
	FBX,
	OBJ,
	GLTF,
};


class ModelLoader
{
public:

	bool LoadModel(MODEL_FORMAT modelFormat, std::string_view p_filePath, Model* p_model );
};

class Model
{
	friend ModelLoader;
	std::vector<Mesh> m_meshes;
	void AddMesh(Mesh&& mesh);

public:
	const std::vector<Mesh>& GetMeshes() const { return m_meshes; }

	Model();
	Model(std::string p_modelFilePath);
	Model(std::string p_modelFilePath, MODEL_FORMAT p_modelFormat);
};

