#pragma	once

#include <memory>
#include <unordered_map>
#include <array>
#include "Math/Transform.h"

class Component;

class Actor
{
	uint32_t componentCount = 0;
	Transform m_transform;
	std::unordered_multimap<std::string, std::shared_ptr<Component>> m_components;

public:
	
	const Transform& GetTransform() const { return m_transform; }
	const Vector3& GetWorldPosition() const { return m_transform.GetWorldPosition(); }
	const Vector3& GetLocalPosition() const { return m_transform.GetLocalPosition(); }
	void SetLocalPosition(const Vector3&  p_position)  
	{ 
		m_transform.SetLocalPosition(p_position); 
	}
	const Quaternion& GetWorldRotation() const { return m_transform.GetWorldRotation(); }
	const Quaternion& GetLocationRotation() const { return m_transform.GetLocalRotation(); }
	
	void OnUpdate();
	
	std::weak_ptr<Component> GetComponent(const std::string& componentName);
	void AddComponent(std::shared_ptr<Component> p_component);
	void RemoveComponent(const std::string& p_componentName);

};