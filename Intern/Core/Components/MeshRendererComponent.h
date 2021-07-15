#pragma once
#include "Mesh.h"
#include "Model.h"
#include "Component.h"

class MeshRendererComponent : public Component
{
	std::unique_ptr<Model> m_model;
protected:
	virtual ComponentCategory GetComponentCategoryImpl() const override { return ComponentCategory::Renderable; }
	virtual std::string GetComponentNameImpl() const override { return "Renderer Component"; }

public:
	void AddModel(const std::string& p_modelFilePath);
	const std::vector<Mesh>& GetMeshes() const { return m_model->GetMeshes();}
	MeshRendererComponent(std::shared_ptr<Actor> p_actor);
	MeshRendererComponent(std::shared_ptr<Actor> p_actor, const std::string& p_modelFilePath);
};