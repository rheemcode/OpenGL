#include "MeshRendererComponent.h"

void MeshRendererComponent::AddModel(const std::string& p_modelFilePath)
{
	m_model = std::make_unique<Model>(p_modelFilePath, OBJ);
}

MeshRendererComponent::MeshRendererComponent(std::shared_ptr<Actor> p_actor)
	: Component(p_actor) {}

MeshRendererComponent::MeshRendererComponent(std::shared_ptr<Actor> p_actor, const std::string& p_modelFilePath)
	: Component(p_actor)
{
	AddModel(p_modelFilePath);
}