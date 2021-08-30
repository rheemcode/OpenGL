#pragma once
#include "Mesh.h"
#include "Model.h"
#include "Component.h"

class GLIB_API MeshRendererComponent : public Component
{
protected:
	std::shared_ptr<Model> m_model;
	virtual ComponentCategory GetComponentCategoryImpl() const override { return ComponentCategory::Renderable; }
	virtual std::string GetComponentNameImpl() const override { return "Renderer Component"; }

public:
	void AddModel(const std::string& p_modelFilePath);

	std::shared_ptr<Model> GetModel() 
	{
		return m_model; 
	}
	//Model& GetModel() { return *m_model; }
	const std::vector<Mesh>& GetMeshes() const { return m_model->GetMeshes();}
	
	void UpdateTransform();
	
	MeshRendererComponent(std::shared_ptr<Actor> p_actor);
	MeshRendererComponent(std::shared_ptr<Actor> p_actor, const std::string& p_modelFilePath);
};