#pragma once
#include <string>
#include <memory>
#include <Actor.h>


enum ComponentCategory
{
	Renderable = 1,
	Transformable,
	Script
};
class Component
{
	bool enabled;
	std::shared_ptr<Actor> m_actor;
protected:
	virtual std::string GetComponentNameImpl() const = 0;
	virtual ComponentCategory GetComponentCategoryImpl() const = 0;

public:
	bool IsEnabled() const { return enabled; }
	void SetEnabled(bool p_enabled) { enabled = p_enabled; }

	std::string GetComponentName() const { return GetComponentNameImpl(); }
	ComponentCategory GetComponentCategory() const { return GetComponentCategoryImpl(); }

	virtual void OnUpdate() {}
	virtual void OnEvent() {}

	Component(std::shared_ptr<Actor> p_actor);
};
