#include "Component.h"

Component::Component(std::shared_ptr<Actor> p_actor)
{
	m_actor = p_actor;
}
