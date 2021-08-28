#include <glpch.h>
#include "StaticMeshRendererComponent.h"

StaticMeshRendererComponent::StaticMeshRendererComponent(std::shared_ptr<Actor> p_actor, std::string p_filePath)
	: MeshRendererComponent(p_actor)
{
	m_model = std::make_shared<StaticModel>(p_filePath);
	m_model->SetTransform(m_actor->GetTransform());
	for (auto& mesh : m_model->GetMeshesRef())
	{
		mesh.m_modelInstance = m_model;
		mesh.GetTransformRef().SetParent(m_model->GetTransform());
	}
}

StaticMeshRendererComponent::StaticMeshRendererComponent(std::shared_ptr<Actor> p_actor)
	: MeshRendererComponent(p_actor)
{

}
