#pragma	once
#include "Math/Transform.h"

class Component;
class Scene;
class Event;

class Actor
{
	friend Scene;
	std::string name;
	uint32_t componentCount = 0;
	Transform m_transform;
	std::unordered_multimap<std::string, std::shared_ptr<Component>> m_components;
	void UpdateComponents(float p_delta);
	void DispatchEventToComponents(const Event& p_event);

public:
	void SetName(const std::string& p_name) { name = p_name;  }
	const std::string& GetName() const { return name;  }
	
	const Transform& GetTransform() const { return m_transform; }
	const Vector3& GetWorldPosition() const { return m_transform.GetWorldPosition(); }
	const Vector3& GetLocalPosition() const { return m_transform.GetLocalPosition(); }
	const Quaternion& GetWorldRotation() const { return m_transform.GetWorldRotation(); }
	const Quaternion& GetLocationRotation() const { return m_transform.GetLocalRotation(); }
	void SetLocalPosition(const Vector3&  p_position)  { m_transform.SetLocalPosition(p_position); }
	

	std::weak_ptr<Component> GetComponent(const std::string& componentName);
	void AddComponent(std::shared_ptr<Component> p_component);
	void RemoveComponent(const std::string& p_componentName);
	
	virtual void OnUpdate(float p_delta) {};
	virtual void OnEvent(const Event& p_event) {};

	Actor() = default;
	~Actor() = default;
};