
#include "SimpleVec.h"
#include "Vector3.h"
SimpleVec3& SimpleVec3::operator =(const SimpleVec4& p_vec)
{
	this->x = p_vec.x;
	this->y = p_vec.y;
	this->z = p_vec.z;
	return *this;
}

SimpleVec3::SimpleVec3()
	: x(0), y(0), z(0) {}
SimpleVec3::SimpleVec3(const float& p_x, const float& p_y, const float& p_z)
	: x(p_x), y(p_y), z(p_z) {}
SimpleVec3::SimpleVec3(const float& p_x, const float& p_y)
	: x(p_x), y(p_y), z(0) {}
SimpleVec3::SimpleVec3(const SimpleVec3& p_vec3)
	: x(p_vec3.x), y(p_vec3.y), z(p_vec3.z) {}
SimpleVec3::SimpleVec3(const Vector3& p_vec3)
	: x(p_vec3.x), y(p_vec3.y), z(p_vec3.z) {}
SimpleVec3::SimpleVec3(const SimpleVec4& p_vec3)
	: x(p_vec3.x), y(p_vec3.y), z(p_vec3.z) {}

SimpleVec4::SimpleVec4()
	: x(0), y(0), z(0), w(0) {}
SimpleVec4::SimpleVec4(const float& p_val)
	: x(p_val), y(p_val), z(p_val), w(p_val)
{
}
SimpleVec4::SimpleVec4(const float& p_x, const float& p_y, const float& p_z, const float& p_w)
	: x(p_x), y(p_y), z(p_z), w(p_w) {}
SimpleVec4::SimpleVec4(const SimpleVec4& p_vec4)
	: x(p_vec4.x), y(p_vec4.y), z(p_vec4.z), w(p_vec4.w) {}
SimpleVec4::SimpleVec4(const SimpleVec3& p_vec3, const float& p_w)
	: x(p_vec3.x), y(p_vec3.y), z(p_vec3.z), w(p_w) {}
SimpleVec4::SimpleVec4(const Vector3& p_vec3, const float& p_w)
	: x(p_vec3.x), y(p_vec3.y), z(p_vec3.z), w(p_w) {}