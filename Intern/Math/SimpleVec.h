#pragma once
#include "GLCore.h"

struct GLIB_API  SimpleVec3
{
	union
	{
		struct { float x, y, z; };
		struct { float r, g, b; };
		struct { float width, height, depth; };
		float component[3] = { 0 };
	};

	SimpleVec3& operator +=(const SimpleVec3& p_vec)
	{
		this->x += p_vec.x;
		this->y += p_vec.y;
		this->z += p_vec.z;


		return *this;
	}

	SimpleVec3 operator +(const SimpleVec3& p_vec) const
	{
		SimpleVec3 res = *this;
		res.x += p_vec.x;
		res.y += p_vec.y;
		res.z += p_vec.z;

		return res;
	}
	
	SimpleVec3& operator =(const struct SimpleVec4& p_vec);

	SimpleVec3 operator -(const SimpleVec3& p_vec) const
	{
		SimpleVec3 res = *this;
		res.x -=p_vec.x;
		res.y -=p_vec.y;
		res.z -=p_vec.z;

		return res;
	}

	SimpleVec3 operator *(const SimpleVec3& p_vec) const
	{
		SimpleVec3 res = *this;
		res.x*= p_vec.x;
		res.y*= p_vec.y;
		res.z*= p_vec.z;
		return res;
	}

	SimpleVec3 operator /(const SimpleVec3& p_vec) const
	{
		SimpleVec3 res = *this;
		res.x /= p_vec.x;
		res.y /= p_vec.y;
		res.z /= p_vec.z;

		return res;
	}

	SimpleVec3 operator *(const float& s) const
	{
		SimpleVec3 res = *this;
		res.x*= s;
		res.y*= s;
		res.z*= s;

		return res;
	}
	SimpleVec3 operator /(const float& s) const
	{
		SimpleVec3 res = *this;
		res.x /= s;
		res.y /= s;
		res.z /= s;

		return res;
	}

	SimpleVec3 operator +(const float& s) const
	{
		SimpleVec3 res = *this;
		res.x += s;
		res.y += s;
		res.z += s;
		return res;
	}

	SimpleVec3 operator -(const float& s) const
	{
		SimpleVec3 res = *this;
		res.x -= s;
		res.y -= s;
		res.z -= s;
		return res;
	}

	SimpleVec3& operator -=(const SimpleVec3& p_vec)
	{
		this->x -= p_vec.x;
		this->y -= p_vec.y;
		this->z -= p_vec.z;

		return *this;
	}

	SimpleVec3& operator *=(const SimpleVec3& p_vec)
	{
		this->x *= p_vec.x;
		this->y *= p_vec.y;
		this->z *= p_vec.z;


		return *this;
	}
	SimpleVec3& operator *=(const float& s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;
		return *this;
	}

	SimpleVec3& operator -=(const float& s)
	{
		this->x -= s;
		this->y -= s;
		this->z -= s;
		return *this;
	}

	SimpleVec3& operator +=(const float& s)
	{
		this->x += s;
		this->y += s;
		this->z += s;

		return *this;
	}

	SimpleVec3& operator /=(const float& s)
	{
		this->x /= s;
		this->y /= s;
		this->z /= s;


		return *this;
	}

	SimpleVec3& operator /=(const SimpleVec3& p_vec)
	{
		this->x /= p_vec.x;
		this->y /= p_vec.y;
		this->z /= p_vec.z;


		return *this;
	}
	
	bool operator!=(const SimpleVec3& p_vec) const
	{
		return (x != p_vec.x && p_vec.y != y);
	}

	const float& operator[](const int& p_index) const
	{
		return component[p_index];
	}

	float& operator[](const int& p_index)
	{
		return component[p_index];
	}

	bool operator ==(const SimpleVec3& p_vec) const
	{
		return (this->x == p_vec.x && this->y == p_vec.y && this->z == p_vec.z);
	}


	SimpleVec3();
	SimpleVec3(const float& p_x, const float& p_y, const float& p_z);
	SimpleVec3(const float& p_x, const float& p_y);
	SimpleVec3(const SimpleVec3& p_vec3);
	SimpleVec3(const struct Vector3& p_vec3);
	SimpleVec3(const SimpleVec4& p_vec3);
};


struct GLIB_API SimpleVec4
{
	union
	{
		__m128 vec_comp;
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };

		float component[4] = { 0 };
	};

	void Normalize()
	{
		auto len = _mm_rsqrt_ps(_mm_dp_ps(vec_comp, vec_comp, 0xff));
		vec_comp = _mm_mul_ps(vec_comp, len);
	}

	SimpleVec4& operator +=(const SimpleVec4& p_vec)
	{
		this->vec_comp = _mm_add_ps(this->vec_comp, p_vec.vec_comp);
		/*this->x += p_vec.x;
		this->y += p_vec.y;
		this->z += p_vec.z;
		this->w += p_vec.w;*/

		return *this;
	}

	SimpleVec4 operator +(const SimpleVec4& p_vec) const
	{
		SimpleVec4 res = *this;
		/*res.x += p_vec.x;
		res.y += p_vec.y;
		res.z += p_vec.z;
		res.w += p_vec.w;*/
		res.vec_comp = _mm_add_ps(res.vec_comp, p_vec.vec_comp);
		return res;
	}
	SimpleVec4 operator -(const SimpleVec4& p_vec) const
	{
		SimpleVec4 res = *this;
		res.vec_comp = _mm_sub_ps(res.vec_comp, p_vec.vec_comp);
		/*res.x -= p_vec.x;
		res.y -= p_vec.y;
		res.z -= p_vec.z;
		res.w -= p_vec.w;*/

		return res;
	}

	SimpleVec4 operator *(const SimpleVec4& p_vec) const
	{
		SimpleVec4 res = *this;
		/*res.x*= p_vec.x;
		res.y*= p_vec.y;
		res.z*= p_vec.z;
		res.w*= p_vec.w;*/
		res.vec_comp = _mm_mul_ps(res.vec_comp, p_vec.vec_comp);
		return res;
	}

	SimpleVec4 operator /(const SimpleVec4& p_vec) const
	{
		SimpleVec4 res = *this;
		/*res.x /= p_vec.x;
		res.y /= p_vec.y;
		res.z /= p_vec.z;
		res.w /= p_vec.w;*/
		res.vec_comp = _mm_div_ps(res.vec_comp, p_vec.vec_comp);
		return res;
	}

	SimpleVec4 operator *(const float& s) const
	{
		SimpleVec4 res = *this;
		res.x *= s;
		res.y *= s;
		res.z *= s;
		res.w *= s;
		//res.vec_comp = _mm_add_ps(res.vec_comp, p_vec.vec_comp);
		return res;
	}
	SimpleVec4 operator /(const float& s) const
	{
		SimpleVec4 res = *this;
		res.x /= s;
		res.y /= s;
		res.z /= s;
		res.w /= s;

		return res;
	}

	SimpleVec4 operator +(const float& s) const
	{
		SimpleVec4 res = *this;
		res.x += s;
		res.y += s;
		res.z += s;
		res.w += s;

		return res;
	}

	SimpleVec4 operator -(const float& s) const
	{
		SimpleVec4 res = *this;
		res.x -= s;
		res.y -= s;
		res.z -= s;
		res.w -= s;

		return res;
	}

	SimpleVec4& operator -=(const SimpleVec4& p_vec)
	{
		/*this->x -= p_vec.x;
		this->y -= p_vec.y;
		this->z -= p_vec.z;
		this->w -= p_vec.w;*/
		this->vec_comp = _mm_sub_ps(this->vec_comp, p_vec.vec_comp);
		return *this;
	}

	SimpleVec4& operator *=(const SimpleVec4& p_vec)
	{
		/*this->x *= p_vec.x;
		this->y *= p_vec.y;
		this->z *= p_vec.z;
		this->w *= p_vec.w;*/
		this->vec_comp = _mm_mul_ps(this->vec_comp, p_vec.vec_comp);
		return *this;
	}
	SimpleVec4& operator *=(const float& s)
	{
		this->x *= s;
		this->y *= s;
		this->z *= s;
		this->w *= s;

		return *this;
	}

	SimpleVec4& operator -=(const float& s)
	{
		this->x -= s;
		this->y -= s;
		this->z -= s;
		this->w -= s;
		return *this;
	}

	SimpleVec4& operator +=(const float& s)
	{
		this->x += s;
		this->y += s;
		this->z += s;
		this->w += s;
		return *this;
	}

	SimpleVec4& operator /=(const float& s)
	{
		this->x /= s;
		this->y /= s;
		this->z /= s;
		this->w /= s;

		return *this;
	}

	SimpleVec4& operator /=(const SimpleVec4& p_vec)
	{
	/*	this->x /= p_vec.x;
		this->y /= p_vec.y;
		this->z /= p_vec.z;
		this->w /= p_vec.w;*/
		this->vec_comp = _mm_div_ps(this->vec_comp, p_vec.vec_comp);
		return *this;
	}

	const float& operator[](const int& p_index) const
	{
		return component[p_index];
	}

	float& operator[](const int& p_index)
	{
		return component[p_index];
	}

	bool operator ==(const SimpleVec4& p_vec)
	{
		return (this->x == p_vec.x && this->y == p_vec.y && this->z == p_vec.z && this->w == p_vec.w);
	}


	SimpleVec4();
	SimpleVec4(const float& p_val);
	SimpleVec4(const float& p_x, const float& p_y, const float& p_z, const float& p_w);
	SimpleVec4(const struct SimpleVec4& p_vec4);
	SimpleVec4(const SimpleVec3& p_vec3, const float& p_w = 1);
	SimpleVec4(const struct Vector3& p_vec3, const float& p_w = 1);
};

typedef SimpleVec4 Vector4;