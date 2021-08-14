#pragma once
#include "MeshRendererComponent.h"
class StaticMeshRendererComponent : public MeshRendererComponent
{
protected:
	//ComponentCategory GetComponentCategoryImpl() const override { return ComponentCategory::Renderable;  };
	//std::string GetComponentNameImpl() const override { return "Static Mesh Render Component"; }
public:
	StaticMeshRendererComponent(std::shared_ptr<Actor> p_actor);
	StaticMeshRendererComponent(std::shared_ptr<Actor> p_actor, std::string p_filePath);

};

