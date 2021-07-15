#include "Components/Component.h"
#include "Actor.h"

const Component& Actor::GetComponent(const std::string& componentName) const
{
	auto val = components.find(componentName);
	if (val != components.end())
		return *val->second;
}

void Actor::AddComponent(Component* p_component)
{
	components.insert({ p_component->GetComponentName(), std::make_shared<Component>(p_component)});
}

void Actor::AddComponent(const Component& p_component)
{
	components.insert({ p_component.GetComponentName(), std::make_shared<Component>(p_component)});
}

void Actor::RemoveComponent(const std::string& p_componentName)
{
	auto val = components.find(p_componentName);
	if (val != components.end())
		components.erase(p_componentName);
}
