#pragma	once

#include <memory>
#include <unordered_map>
#include <array>
#include "Math/Transform.h"

class Actor
{
	uint32_t componentCount = 0;
	Transform m_transform;
	std::unordered_multimap<std::string, std::shared_ptr<Component>> components;

public:
	
	const Transform& GetTransform() const { return m_transform; }
	const Vector3& GetWorldPosition() const { return m_transform.GetWorldPosition(); }
	const Vector3& GetLocalPosition() const { return m_transform.GetLocalPosition(); }
	const Quaternion& GetWorldRotation() const { return m_transform.GetWorldRotation(); }
	const Quaternion& GetLocationRotation() const { return m_transform.GetLocalRotation(); }

	const Component& GetComponent(const std::string& componentName) const;
	void AddComponent(class Component* p_component);
	void AddComponent(const class Component& p_component);
	void RemoveComponent(const std::string& p_componentName);

};