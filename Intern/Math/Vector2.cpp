#include <glpch.h>
#include "Vector3.h"
#include "Math/Vector2.h"


Vector2::Vector2(const Vector3& p_vec)
{
	x = p_vec.x; y = p_vec.y;
}


Vector2 Vector2::Abs(Vector2& p_vec)
{
	return Vector2(Math::Abs(p_vec.x), Math::Abs(p_vec.y));
}

Vector2 Vector2::operator=(const Vector3& p_vec)
{
	this->x = p_vec.x;
	this->y = p_vec.y;
	return *this;
}
