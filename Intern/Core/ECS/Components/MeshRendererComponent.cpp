#include <glpch.h>
#include "MeshRendererComponent.h"

void MeshRendererComponent::AddModel(const std::string& p_modelFilePath)
{
	m_model = std::make_shared<Model>(p_modelFilePath, ModelFormat::OBJ);
	m_model->SetTransform(m_actor->GetTransform());
	for (auto& mesh : m_model->GetMeshesRef())
	{
		mesh.m_modelInstance = m_model;
		mesh.GetTransformRef().SetParent(m_model->GetTransform());
	}
}

void MeshRendererComponent::UpdateTransform()
{
	for (auto& mesh : m_model->GetMeshesRef())
	{
		mesh.GetTransformRef().UpdateTransform();
	}
}

MeshRendererComponent::MeshRendererComponent(std::shared_ptr<Actor> p_actor)
	: Component(p_actor) {}

MeshRendererComponent::MeshRendererComponent(std::shared_ptr<Actor> p_actor, const std::string& p_modelFilePath)
	: Component(p_actor)
{
	AddModel(p_modelFilePath);
}