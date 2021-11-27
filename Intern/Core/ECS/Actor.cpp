#include <glpch.h>
#include "Components/Component.h"
#include "Actor.h"
#include "Events/Event.h"

void Actor::UpdateComponents(float p_delta)
{
	for (auto& component : m_components)
	{
		component.second->OnUpdate(p_delta);
	}
}


void Actor::DispatchEventToComponents(const Event& p_event)
{
	for (auto& component : m_components)
	{
		component.second->OnEvent(p_event);
	}
}

//std::weak_ptr<Component> Actor::GetComponent(const std::string& componentName)
//{
//	auto val = m_components.find(componentName);
//	if (val != m_components.end())
//		return val->second;
//	return std::weak_ptr<Component>();
//}

Component* Actor::GetComponent(const std::string& componentName)
{
	auto val = m_components.find(componentName);
	if (val != m_components.end())
		return val->second.get();
	return nullptr;
}

void Actor::AddComponent(std::shared_ptr<Component> p_component)
{
	m_components.insert({ p_component->GetComponentName(), p_component });
}

void Actor::RemoveComponent(const std::string& p_componentName)
{
	auto val = m_components.find(p_componentName);
	if (val != m_components.end())
		m_components.erase(p_componentName);
}
