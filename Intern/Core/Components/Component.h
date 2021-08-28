#pragma once
#include <string>
#include <memory>
#include <Actor.h>
#include "Events/Event.h"


enum ComponentCategory
{
	Renderable = 1,
	Transformable,
	Script
};
class Component
{
	bool enabled;
protected:
	std::shared_ptr<Actor> m_actor;
	virtual std::string GetComponentNameImpl() const = 0;
	virtual ComponentCategory GetComponentCategoryImpl() const = 0;

public:
	bool IsEnabled() const { return enabled; }
	void SetEnabled(bool p_enabled) { enabled = p_enabled; }

	std::string GetComponentName() const { return GetComponentNameImpl(); }
	ComponentCategory GetComponentCategory() const { return GetComponentCategoryImpl(); }

	virtual void OnUpdate(float p_delta) {}
	virtual void OnEvent(const Event& event) {}

	Component(std::shared_ptr<Actor> p_actor);
};
