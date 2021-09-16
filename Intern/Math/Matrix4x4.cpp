#include <glpch.h>
#include "Math/Math.h"
#include "Math/Vector3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include <cassert>
#include <iostream>

//#define USE_INTRINSIC

const Matrix4x4 Matrix4x4::Identity = Matrix4x4(1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f);


Matrix4x4::Matrix4x4(const Matrix3x3& p_mat)
{
	this->m_data[0] = SimpleVec4(p_mat[0], 0.f);
	this->m_data[1] = SimpleVec4(p_mat[1], 0.f);
	this->m_data[2] = SimpleVec4(p_mat[2], 0.f);
	this->m_data[3] = SimpleVec4(0.f, 0.f, 0.f, 1.f);
}
Matrix4x4::Matrix4x4()
{
	*this = Matrix4x4(1.f, 0.f, 0.f, 0.f,
					 0.f, 1.f, 0.f, 0.f,
					 0.f, 0.f, 1.f, 0.f,
					 0.f, 0.f, 0.f, 1.f);
}


Matrix4x4::Matrix4x4(const Matrix4x4& p_mat)
{
	m_data[0] = p_mat.m_data[0];
	m_data[1] = p_mat.m_data[1];
	m_data[2] = p_mat.m_data[2];
	m_data[3] = p_mat.m_data[3];
}

Matrix4x4::Matrix4x4 
(
	const float& x0, const float& y0, const float& z0, const float& w0,
	const float& x1, const float& y1, const float& z1, const float& w1,
	const float& x2, const float& y2, const float& z2, const float& w2,
	const float& x3, const float& y3, const float& z3, const float& w3
)
{
	m_data[0] = colType(x0, y0, z0, w0);
	m_data[1] = colType(x1, y1, z1, w1);
	m_data[2] = colType(x2, y2, z2, w2);
	m_data[3] = colType(x3, y3, z3, w3);

}

Matrix4x4::Matrix4x4(const float& s)
{
	m_data[0] = colType(s, 0, 0, 0);
	m_data[1] = colType(0, s, 0, 0);
	m_data[2] = colType(0, 0, s, 0);
	m_data[3] = colType(0, 0, 0, s);

}

const SimpleVec4& Matrix4x4::operator[](const int& p_index) const 
{
	return m_data[p_index];
}

SimpleVec4& Matrix4x4::operator[](const int& p_index)
{
	return m_data[p_index];
}

Vector4 operator*(const Matrix4x4& p_mat, const Vector4& p_vec)
{
#ifndef USE_INTRINSIC
	Vector4 const Mov0(p_vec[0]);
	Vector4 const Mov1(p_vec[1]);
	Vector4 const Mul0 = p_mat[0] * Mov0;
	Vector4 const Mul1 = p_mat[1] * Mov1;
	Vector4 const Add0 = Mul0 + Mul1;
	Vector4 const Mov2(p_vec[2]);
	Vector4 const Mov3(p_vec[3]);
	Vector4 const Mul2 = p_mat[2] * Mov2;
	Vector4 const Mul3 = p_mat[3] * Mov3;
	Vector4 const Add1 = Mul2 + Mul3;
	Vector4 const Add2 = Add0 + Add1;
	return Add2;
#else
	Vector4 res;
	__m128 v0 = _mm_shuffle_ps(p_vec.vec_comp, p_vec.vec_comp, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 v1 = _mm_shuffle_ps(p_vec.vec_comp, p_vec.vec_comp, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 v2 = _mm_shuffle_ps(p_vec.vec_comp, p_vec.vec_comp, _MM_SHUFFLE(2, 2, 2, 2));
	__m128 v3 = _mm_shuffle_ps(p_vec.vec_comp, p_vec.vec_comp, _MM_SHUFFLE(3, 3, 3, 3));

	__m128 m0 = _mm_mul_ps(p_mat[0].vec_comp, v0);
	__m128 m1 = _mm_mul_ps(p_mat[1].vec_comp, v1);
	__m128 m2 = _mm_mul_ps(p_mat[2].vec_comp, v2);
	__m128 m3 = _mm_mul_ps(p_mat[3].vec_comp, v3);

	__m128 a0 = _mm_add_ps(m0, m1);
	__m128 a1 = _mm_add_ps(m2, m3);
	__m128 a2 = _mm_add_ps(a0, a1);
	
	res.vec_comp = a2;
	return res;
#endif
}

Matrix4x4 operator*(const Matrix4x4& p_mat, const Matrix4x4& p_mat2)
{
	Matrix4x4 res(0);

#ifndef USE_INTRINSIC

	const SimpleVec4 SrcA0 = p_mat[0];
	const SimpleVec4 SrcA1 = p_mat[1];
	const SimpleVec4 SrcA2 = p_mat[2];
	const SimpleVec4 SrcA3 = p_mat[3];

	const SimpleVec4 SrcB0 = p_mat2[0];
	const SimpleVec4 SrcB1 = p_mat2[1];
	const SimpleVec4 SrcB2 = p_mat2[2];
	const SimpleVec4 SrcB3 = p_mat2[3];

	auto ss = SrcB0[0];
	auto s = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
	res[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] + SrcA3 * SrcB0[3];
	res[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] + SrcA3 * SrcB1[3];
	res[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] + SrcA3 * SrcB2[3];
	res[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] + SrcA3 * SrcB3[3];


#else

	{
		__m128 e0 = _mm_shuffle_ps(p_mat2[0].vec_comp, p_mat2[0].vec_comp, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 e1 = _mm_shuffle_ps(p_mat2[0].vec_comp, p_mat2[0].vec_comp, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 e2 = _mm_shuffle_ps(p_mat2[0].vec_comp, p_mat2[0].vec_comp, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 e3 = _mm_shuffle_ps(p_mat2[0].vec_comp, p_mat2[0].vec_comp, _MM_SHUFFLE(3, 3, 3, 3));

		__m128 m0 = _mm_mul_ps(p_mat[0].vec_comp, e0);
		__m128 m1 = _mm_mul_ps(p_mat[1].vec_comp, e1);
		__m128 m2 = _mm_mul_ps(p_mat[2].vec_comp, e2);
		__m128 m3 = _mm_mul_ps(p_mat[3].vec_comp, e3);

		__m128 a0 = _mm_add_ps(m0, m1);
		__m128 a1 = _mm_add_ps(m2, m3);
		__m128 a2 = _mm_add_ps(a0, a1);

		res[0].vec_comp = a2;
	}

	{
		__m128 e0 = _mm_shuffle_ps(p_mat2[1].vec_comp, p_mat2[1].vec_comp, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 e1 = _mm_shuffle_ps(p_mat2[1].vec_comp, p_mat2[1].vec_comp, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 e2 = _mm_shuffle_ps(p_mat2[1].vec_comp, p_mat2[1].vec_comp, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 e3 = _mm_shuffle_ps(p_mat2[1].vec_comp, p_mat2[1].vec_comp, _MM_SHUFFLE(3, 3, 3, 3));

		__m128 m0 = _mm_mul_ps(p_mat[0].vec_comp, e0);
		__m128 m1 = _mm_mul_ps(p_mat[1].vec_comp, e1);
		__m128 m2 = _mm_mul_ps(p_mat[2].vec_comp, e2);
		__m128 m3 = _mm_mul_ps(p_mat[3].vec_comp, e3);

		__m128 a0 = _mm_add_ps(m0, m1);
		__m128 a1 = _mm_add_ps(m2, m3);
		__m128 a2 = _mm_add_ps(a0, a1);

		res[1].vec_comp = a2;
	}

	{
		__m128 e0 = _mm_shuffle_ps(p_mat2[2].vec_comp, p_mat2[2].vec_comp, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 e1 = _mm_shuffle_ps(p_mat2[2].vec_comp, p_mat2[2].vec_comp, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 e2 = _mm_shuffle_ps(p_mat2[2].vec_comp, p_mat2[2].vec_comp, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 e3 = _mm_shuffle_ps(p_mat2[2].vec_comp, p_mat2[2].vec_comp, _MM_SHUFFLE(3, 3, 3, 3));

		__m128 m0 = _mm_mul_ps(p_mat[0].vec_comp, e0);
		__m128 m1 = _mm_mul_ps(p_mat[1].vec_comp, e1);
		__m128 m2 = _mm_mul_ps(p_mat[2].vec_comp, e2);
		__m128 m3 = _mm_mul_ps(p_mat[3].vec_comp, e3);

		__m128 a0 = _mm_add_ps(m0, m1);
		__m128 a1 = _mm_add_ps(m2, m3);
		__m128 a2 = _mm_add_ps(a0, a1);

		res[2].vec_comp = a2;
	}

	{
		//(__m128&)_mm_shuffle_epi32(__m128i&)p_mat2[0].vec_comp, _MM_SHUFFLE(3, 3, 3, 3))
		__m128 e0 = _mm_shuffle_ps(p_mat2[3].vec_comp, p_mat2[3].vec_comp, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 e1 = _mm_shuffle_ps(p_mat2[3].vec_comp, p_mat2[3].vec_comp, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 e2 = _mm_shuffle_ps(p_mat2[3].vec_comp, p_mat2[3].vec_comp, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 e3 = _mm_shuffle_ps(p_mat2[3].vec_comp, p_mat2[3].vec_comp, _MM_SHUFFLE(3, 3, 3, 3));

		__m128 m0 = _mm_mul_ps(p_mat[0].vec_comp, e0);
		__m128 m1 = _mm_mul_ps(p_mat[1].vec_comp, e1);
		__m128 m2 = _mm_mul_ps(p_mat[2].vec_comp, e2);
		__m128 m3 = _mm_mul_ps(p_mat[3].vec_comp, e3);

		__m128 a0 = _mm_add_ps(m0, m1);
		__m128 a1 = _mm_add_ps(m2, m3);
		__m128 a2 = _mm_add_ps(a0, a1);

		res[3].vec_comp = a2;
	}
#endif
	return res;
}


Matrix4x4& Matrix4x4::operator=(const Matrix4x4& p_mat)
{
	m_data[0] = p_mat.m_data[0];
	m_data[1] = p_mat.m_data[1];
	m_data[2] = p_mat.m_data[2];
	m_data[3] = p_mat.m_data[3];
	return *this;
}

Matrix4x4::Matrix4x4(const colType& col1, const colType& col2, const colType& col3, const colType& col4)
{
	m_data[0] = col1;
	m_data[1] = col2;
	m_data[2] = col3;
	m_data[3] = col4;
}
Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& p_mat)
{
	m_data[0] += p_mat.m_data[0];
	m_data[1] += p_mat.m_data[1];
	m_data[2] += p_mat.m_data[2];
	m_data[3] += p_mat.m_data[3];
	return *this;
}

bool Matrix4x4::operator==(const Matrix4x4& p_mat)
{
	return (m_data[0] == p_mat.m_data[0]) &&
		(m_data[1] == p_mat.m_data[1]) &&
		(m_data[2] == p_mat.m_data[2] )&&
		(m_data[3] == p_mat.m_data[3]);
}

Matrix4x4 Matrix4x4::Inverse(const Matrix4x4& p_mat)
{
#ifndef USE_INTRINSIC
	float Coef00 = p_mat[2][2] * p_mat[3][3] - p_mat[3][2] * p_mat[2][3];
	float Coef02 = p_mat[1][2] * p_mat[3][3] - p_mat[3][2] * p_mat[1][3];
	float Coef03 = p_mat[1][2] * p_mat[2][3] - p_mat[2][2] * p_mat[1][3];

	float Coef04 = p_mat[2][1] * p_mat[3][3] - p_mat[3][1] * p_mat[2][3];
	float Coef06 = p_mat[1][1] * p_mat[3][3] - p_mat[3][1] * p_mat[1][3];
	float Coef07 = p_mat[1][1] * p_mat[2][3] - p_mat[2][1] * p_mat[1][3];

	float Coef08 = p_mat[2][1] * p_mat[3][2] - p_mat[3][1] * p_mat[2][2];
	float Coef10 = p_mat[1][1] * p_mat[3][2] - p_mat[3][1] * p_mat[1][2];
	float Coef11 = p_mat[1][1] * p_mat[2][2] - p_mat[2][1] * p_mat[1][2];

	float Coef12 = p_mat[2][0] * p_mat[3][3] - p_mat[3][0] * p_mat[2][3];
	float Coef14 = p_mat[1][0] * p_mat[3][3] - p_mat[3][0] * p_mat[1][3];
	float Coef15 = p_mat[1][0] * p_mat[2][3] - p_mat[2][0] * p_mat[1][3];

	float Coef16 = p_mat[2][0] * p_mat[3][2] - p_mat[3][0] * p_mat[2][2];
	float Coef18 = p_mat[1][0] * p_mat[3][2] - p_mat[3][0] * p_mat[1][2];
	float Coef19 = p_mat[1][0] * p_mat[2][2] - p_mat[2][0] * p_mat[1][2];

	float Coef20 = p_mat[2][0] * p_mat[3][1] - p_mat[3][0] * p_mat[2][1];
	float Coef22 = p_mat[1][0] * p_mat[3][1] - p_mat[3][0] * p_mat[1][1];
	float Coef23 = p_mat[1][0] * p_mat[2][1] - p_mat[2][0] * p_mat[1][1];

	SimpleVec4 Fac0(Coef00, Coef00, Coef02, Coef03);
	SimpleVec4 Fac1(Coef04, Coef04, Coef06, Coef07);
	SimpleVec4 Fac2(Coef08, Coef08, Coef10, Coef11);
	SimpleVec4 Fac3(Coef12, Coef12, Coef14, Coef15);
	SimpleVec4 Fac4(Coef16, Coef16, Coef18, Coef19);
	SimpleVec4 Fac5(Coef20, Coef20, Coef22, Coef23);

	SimpleVec4 Vec0(p_mat[1][0], p_mat[0][0], p_mat[0][0], p_mat[0][0]);
	SimpleVec4 Vec1(p_mat[1][1], p_mat[0][1], p_mat[0][1], p_mat[0][1]);
	SimpleVec4 Vec2(p_mat[1][2], p_mat[0][2], p_mat[0][2], p_mat[0][2]);
	SimpleVec4 Vec3(p_mat[1][3], p_mat[0][3], p_mat[0][3], p_mat[0][3]);

	SimpleVec4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
	SimpleVec4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
	SimpleVec4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
	SimpleVec4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

	SimpleVec4 SignA(+1, -1, +1, -1);
	SimpleVec4 SignB(-1, +1, -1, +1);
	Matrix4x4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

	SimpleVec4 Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

	SimpleVec4 Dot0(p_mat[0] * Row0);
	float Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

	float OneOverDeterminant = 1.f / Dot1;
	return Inverse * OneOverDeterminant;
#else
	__m128 Fac0;
	{
		//	valType SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		//	valType SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		//	valType SubFactor06 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		//	valType SubFactor13 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		__m128 Swp0a = _mm_shuffle_ps(p_mat[3].vec_comp, p_mat[2].vec_comp, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 Swp0b = _mm_shuffle_ps(p_mat[3].vec_comp, p_mat[2].vec_comp, _MM_SHUFFLE(2, 2, 2, 2));

		__m128 Swp00 = _mm_shuffle_ps(p_mat[2].vec_comp, p_mat[1].vec_comp, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
		__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
		__m128 Swp03 = _mm_shuffle_ps(p_mat[2].vec_comp, p_mat[1].vec_comp, _MM_SHUFFLE(3, 3, 3, 3));

		__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
		__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
		Fac0 = _mm_sub_ps(Mul00, Mul01);
	}

	__m128 Fac1;
	{
		//	valType SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		//	valType SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		//	valType SubFactor07 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		//	valType SubFactor14 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		__m128 Swp0a = _mm_shuffle_ps(p_mat[3].vec_comp, p_mat[2].vec_comp, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 Swp0b = _mm_shuffle_ps(p_mat[3].vec_comp, p_mat[2].vec_comp, _MM_SHUFFLE(1, 1, 1, 1));

		__m128 Swp00 = _mm_shuffle_ps(p_mat[2].vec_comp, p_mat[1].vec_comp, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
		__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
		__m128 Swp03 = _mm_shuffle_ps(p_mat[2].vec_comp, p_mat[1].vec_comp, _MM_SHUFFLE(3, 3, 3, 3));

		__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
		__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
		Fac1 = _mm_sub_ps(Mul00, Mul01);
	}


	__m128 Fac2;
	{
		//	valType SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		//	valType SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		//	valType SubFactor08 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		//	valType SubFactor15 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		__m128 Swp0a = _mm_shuffle_ps(p_mat[3].vec_comp, p_mat[2].vec_comp, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 Swp0b = _mm_shuffle_ps(p_mat[3].vec_comp, p_mat[2].vec_comp, _MM_SHUFFLE(1, 1, 1, 1));

		__m128 Swp00 = _mm_shuffle_ps(p_mat[2].vec_comp, p_mat[1].vec_comp, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
		__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
		__m128 Swp03 = _mm_shuffle_ps(p_mat[2].vec_comp, p_mat[1].vec_comp, _MM_SHUFFLE(2, 2, 2, 2));

		__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
		__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
		Fac2 = _mm_sub_ps(Mul00, Mul01);
	}

	__m128 Fac3;
	{
		//	valType SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		//	valType SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		//	valType SubFactor09 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		//	valType SubFactor16 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		__m128 Swp0a = _mm_shuffle_ps(p_mat[3].vec_comp, p_mat[2].vec_comp, _MM_SHUFFLE(3, 3, 3, 3));
		__m128 Swp0b = _mm_shuffle_ps(p_mat[3].vec_comp, p_mat[2].vec_comp, _MM_SHUFFLE(0, 0, 0, 0));

		__m128 Swp00 = _mm_shuffle_ps(p_mat[2].vec_comp, p_mat[1].vec_comp, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
		__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
		__m128 Swp03 = _mm_shuffle_ps(p_mat[2].vec_comp, p_mat[1].vec_comp, _MM_SHUFFLE(3, 3, 3, 3));

		__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
		__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
		Fac3 = _mm_sub_ps(Mul00, Mul01);
	}

	__m128 Fac4;
	{
		//	valType SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		//	valType SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		//	valType SubFactor10 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		//	valType SubFactor17 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		__m128 Swp0a = _mm_shuffle_ps(p_mat[3].vec_comp, p_mat[2].vec_comp, _MM_SHUFFLE(2, 2, 2, 2));
		__m128 Swp0b = _mm_shuffle_ps(p_mat[3].vec_comp, p_mat[2].vec_comp, _MM_SHUFFLE(0, 0, 0, 0));

		__m128 Swp00 = _mm_shuffle_ps(p_mat[2].vec_comp, p_mat[1].vec_comp, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
		__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
		__m128 Swp03 = _mm_shuffle_ps(p_mat[2].vec_comp, p_mat[1].vec_comp, _MM_SHUFFLE(2, 2, 2, 2));

		__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
		__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
		Fac4 = _mm_sub_ps(Mul00, Mul01);
	}

	__m128 Fac5;
	{
		//	valType SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		//	valType SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		//	valType SubFactor12 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		//	valType SubFactor18 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		__m128 Swp0a = _mm_shuffle_ps(p_mat[3].vec_comp, p_mat[2].vec_comp, _MM_SHUFFLE(1, 1, 1, 1));
		__m128 Swp0b = _mm_shuffle_ps(p_mat[3].vec_comp, p_mat[2].vec_comp, _MM_SHUFFLE(0, 0, 0, 0));

		__m128 Swp00 = _mm_shuffle_ps(p_mat[2].vec_comp, p_mat[1].vec_comp, _MM_SHUFFLE(0, 0, 0, 0));
		__m128 Swp01 = _mm_shuffle_ps(Swp0a, Swp0a, _MM_SHUFFLE(2, 0, 0, 0));
		__m128 Swp02 = _mm_shuffle_ps(Swp0b, Swp0b, _MM_SHUFFLE(2, 0, 0, 0));
		__m128 Swp03 = _mm_shuffle_ps(p_mat[2].vec_comp, p_mat[1].vec_comp, _MM_SHUFFLE(1, 1, 1, 1));

		__m128 Mul00 = _mm_mul_ps(Swp00, Swp01);
		__m128 Mul01 = _mm_mul_ps(Swp02, Swp03);
		Fac5 = _mm_sub_ps(Mul00, Mul01);
	}

	__m128 SignA = _mm_set_ps(1.0f, -1.0f, 1.0f, -1.0f);
	__m128 SignB = _mm_set_ps(-1.0f, 1.0f, -1.0f, 1.0f);

	// m[1][0]
	// m[0][0]
	// m[0][0]
	// m[0][0]
	__m128 Temp0 = _mm_shuffle_ps(p_mat[1].vec_comp, p_mat[0].vec_comp, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 Vec0 = _mm_shuffle_ps(Temp0, Temp0, _MM_SHUFFLE(2, 2, 2, 0));

	// m[1][1]
	// m[0][1]
	// m[0][1]
	// m[0][1]
	__m128 Temp1 = _mm_shuffle_ps(p_mat[1].vec_comp, p_mat[0].vec_comp, _MM_SHUFFLE(1, 1, 1, 1));
	__m128 Vec1 = _mm_shuffle_ps(Temp1, Temp1, _MM_SHUFFLE(2, 2, 2, 0));

	// m[1][2]
	// m[0][2]
	// m[0][2]
	// m[0][2]
	__m128 Temp2 = _mm_shuffle_ps(p_mat[1].vec_comp, p_mat[0].vec_comp, _MM_SHUFFLE(2, 2, 2, 2));
	__m128 Vec2 = _mm_shuffle_ps(Temp2, Temp2, _MM_SHUFFLE(2, 2, 2, 0));

	// m[1][3]
	// m[0][3]
	// m[0][3]
	// m[0][3]
	__m128 Temp3 = _mm_shuffle_ps(p_mat[1].vec_comp, p_mat[0].vec_comp, _MM_SHUFFLE(3, 3, 3, 3));
	__m128 Vec3 = _mm_shuffle_ps(Temp3, Temp3, _MM_SHUFFLE(2, 2, 2, 0));

	// col0
	// + (Vec1[0] * Fac0[0] - Vec2[0] * Fac1[0] + Vec3[0] * Fac2[0]),
	// - (Vec1[1] * Fac0[1] - Vec2[1] * Fac1[1] + Vec3[1] * Fac2[1]),
	// + (Vec1[2] * Fac0[2] - Vec2[2] * Fac1[2] + Vec3[2] * Fac2[2]),
	// - (Vec1[3] * Fac0[3] - Vec2[3] * Fac1[3] + Vec3[3] * Fac2[3]),
	__m128 Mul00 = _mm_mul_ps(Vec1, Fac0);
	__m128 Mul01 = _mm_mul_ps(Vec2, Fac1);
	__m128 Mul02 = _mm_mul_ps(Vec3, Fac2);
	__m128 Sub00 = _mm_sub_ps(Mul00, Mul01);
	__m128 Add00 = _mm_add_ps(Sub00, Mul02);
	__m128 Inv0 = _mm_mul_ps(SignB, Add00);

	// col1
	// - (Vec0[0] * Fac0[0] - Vec2[0] * Fac3[0] + Vec3[0] * Fac4[0]),
	// + (Vec0[0] * Fac0[1] - Vec2[1] * Fac3[1] + Vec3[1] * Fac4[1]),
	// - (Vec0[0] * Fac0[2] - Vec2[2] * Fac3[2] + Vec3[2] * Fac4[2]),
	// + (Vec0[0] * Fac0[3] - Vec2[3] * Fac3[3] + Vec3[3] * Fac4[3]),
	__m128 Mul03 = _mm_mul_ps(Vec0, Fac0);
	__m128 Mul04 = _mm_mul_ps(Vec2, Fac3);
	__m128 Mul05 = _mm_mul_ps(Vec3, Fac4);
	__m128 Sub01 = _mm_sub_ps(Mul03, Mul04);
	__m128 Add01 = _mm_add_ps(Sub01, Mul05);
	__m128 Inv1 = _mm_mul_ps(SignA, Add01);

	// col2
	// + (Vec0[0] * Fac1[0] - Vec1[0] * Fac3[0] + Vec3[0] * Fac5[0]),
	// - (Vec0[0] * Fac1[1] - Vec1[1] * Fac3[1] + Vec3[1] * Fac5[1]),
	// + (Vec0[0] * Fac1[2] - Vec1[2] * Fac3[2] + Vec3[2] * Fac5[2]),
	// - (Vec0[0] * Fac1[3] - Vec1[3] * Fac3[3] + Vec3[3] * Fac5[3]),
	__m128 Mul06 = _mm_mul_ps(Vec0, Fac1);
	__m128 Mul07 = _mm_mul_ps(Vec1, Fac3);
	__m128 Mul08 = _mm_mul_ps(Vec3, Fac5);
	__m128 Sub02 = _mm_sub_ps(Mul06, Mul07);
	__m128 Add02 = _mm_add_ps(Sub02, Mul08);
	__m128 Inv2 = _mm_mul_ps(SignB, Add02);

	// col3
	// - (Vec1[0] * Fac2[0] - Vec1[0] * Fac4[0] + Vec2[0] * Fac5[0]),
	// + (Vec1[0] * Fac2[1] - Vec1[1] * Fac4[1] + Vec2[1] * Fac5[1]),
	// - (Vec1[0] * Fac2[2] - Vec1[2] * Fac4[2] + Vec2[2] * Fac5[2]),
	// + (Vec1[0] * Fac2[3] - Vec1[3] * Fac4[3] + Vec2[3] * Fac5[3]));
	__m128 Mul09 = _mm_mul_ps(Vec0, Fac2);
	__m128 Mul10 = _mm_mul_ps(Vec1, Fac4);
	__m128 Mul11 = _mm_mul_ps(Vec2, Fac5);
	__m128 Sub03 = _mm_sub_ps(Mul09, Mul10);
	__m128 Add03 = _mm_add_ps(Sub03, Mul11);
	__m128 Inv3 = _mm_mul_ps(SignA, Add03);

	__m128 Row0 = _mm_shuffle_ps(Inv0, Inv1, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 Row1 = _mm_shuffle_ps(Inv2, Inv3, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 Row2 = _mm_shuffle_ps(Row0, Row1, _MM_SHUFFLE(2, 0, 2, 0));

	//	valType Determinant = m[0][0] * Inverse[0][0] 
	//						+ m[0][1] * Inverse[1][0] 
	//						+ m[0][2] * Inverse[2][0] 
	//						+ m[0][3] * Inverse[3][0];

	
	__m128 Det0 = _mm_dp_ps(p_mat[0].vec_comp, Row2, 0xff);
	__m128 Rcp0 = _mm_rcp_ps(Det0);
	//__m128 Rcp0 = _mm_div_ps(one, Det0);
	//	Inverse /= Determinant;
	Matrix4x4 out;
	out[0].vec_comp = _mm_mul_ps(Inv0, Rcp0);
	out[1].vec_comp = _mm_mul_ps(Inv1, Rcp0);
	out[2].vec_comp = _mm_mul_ps(Inv2, Rcp0);
	out[3].vec_comp = _mm_mul_ps(Inv3, Rcp0);
	return out;
#endif

}

Matrix4x4& Matrix4x4::operator*(const float& p_s)
{
	this->m_data[0] *= p_s;
	this->m_data[1] *= p_s;
	this->m_data[2] *= p_s;
	this->m_data[3] *= p_s;
	return *this;
}

Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& p_matrix)
{
	*this = *this * p_matrix;
	return *this;
}


Matrix4x4 Matrix4x4::CreateOrtho(const float& p_left, const float& p_right, const float& p_bottom, const float& p_top, const float& p_zNear, const float& p_zFar)
{
	Matrix4x4 Result(1);
	Result[0][0] = 2.f / (p_right - p_left);
	Result[1][1] = 2.f / (p_top - p_bottom);
	Result[2][2] = -2.f / (p_zFar - p_zNear);
	Result[3][0] = -(p_right + p_left) / (p_right - p_left);
	Result[3][1] = -(p_top + p_bottom) / (p_top - p_bottom);
	Result[3][2] = -(p_zFar + p_zNear) / (p_zFar - p_zNear);
	return Result;
}

Matrix4x4 Matrix4x4::CreatePerspectiveFOV(const float& fov, const float& width, const float& height, const float& zNear, const float& zFar)
{
	assert(width > 0);
	assert(height > 0);
	assert(fov > 0);

	const float rad = fov;
	const float h = Math::Cos(0.5f * rad) / Math::Sin(0.5f * rad);
	const float w = h * height / width;

	Matrix4x4 Result(0.f);
	Result[0][0] = w;
	Result[1][1] = h;
	Result[2][2] = -(zFar + zNear) / (zFar - zNear);
	Result[2][3] = -1.f;
	Result[3][2] = -(2.f * zFar * zNear) / (zFar - zNear);
	return Result;
}

Matrix4x4 Matrix4x4::CreateFrustum(const float& left, const float& right, const float& bottom, const float& top, const float& nearVal, const float& farVal)
{
	Matrix4x4 Result(0);
	Result[0][0] = (2.f * nearVal) / (right - left);
	Result[1][1] = (2.f * nearVal) / (top - bottom);
	Result[2][0] = (right + left) / (right - left);
	Result[2][1] = (top + bottom) / (top - bottom);
	Result[2][2] = -(farVal + nearVal) / (farVal - nearVal);
	Result[2][3] = -1.f;
	Result[3][2] = -(2.f * farVal * nearVal) / (farVal - nearVal);
	return Result;
}

Matrix4x4 Matrix4x4::CreatePerspective(const float& p_fovy, const float& p_aspectRatio, const float& p_zNear, const float& p_zFar)
{
	
   assert(Math::Abs(p_aspectRatio - static_cast<float>(EPSILON)) > 0.f);

	const float tanHalfFovy = tan(p_fovy / 2.f);

	Matrix4x4 Result(0.f);
	Result[0][0] = 1.f / (p_aspectRatio * tanHalfFovy);
	Result[1][1] = 1.f / (tanHalfFovy);
	Result[2][2] = -(p_zFar + p_zNear) / (p_zFar - p_zNear);
	Result[2][3] = -1.f;
	Result[3][2] = -(2.f* p_zFar * p_zNear) / (p_zFar - p_zNear);
	return Result;
}

float Matrix4x4::Determinant(const Matrix4x4& m)
{
	float SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	float SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	float SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	float SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	float SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	float SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

	SimpleVec4 DetCof(
		+(m[1][1] * SubFactor00 - m[1][2] * SubFactor01 + m[1][3] * SubFactor02),
		-(m[1][0] * SubFactor00 - m[1][2] * SubFactor03 + m[1][3] * SubFactor04),
		+(m[1][0] * SubFactor01 - m[1][1] * SubFactor03 + m[1][3] * SubFactor05),
		-(m[1][0] * SubFactor02 - m[1][1] * SubFactor04 + m[1][2] * SubFactor05));

	return
		m[0][0] * DetCof[0] + m[0][1] * DetCof[1] +
		m[0][2] * DetCof[2] + m[0][3] * DetCof[3];
}

Matrix4x4 Matrix4x4::Transpose(const Matrix4x4& m)
{
	Matrix4x4 result(0);
#ifndef USE_INTRNSIC
	result[0][0] = m[0][0];
	result[0][1] = m[1][0];
	result[0][2] = m[2][0];
	result[0][3] = m[3][0];

	result[1][0] = m[0][1];
	result[1][1] = m[1][1];
	result[1][2] = m[2][1];
	result[1][3] = m[3][1];

	result[2][0] = m[0][2];
	result[2][1] = m[1][2];
	result[2][2] = m[2][2];
	result[2][3] = m[3][2];

	result[3][0] = m[0][3];
	result[3][1] = m[1][3];
	result[3][2] = m[2][3];
	result[3][3] = m[3][3];
#else
	__m128 tmp0 = _mm_shuffle_ps(m[0].vec_comp, m[1].vec_comp, 0x44);
	__m128 tmp2 = _mm_shuffle_ps(m[0].vec_comp, m[1].vec_comp, 0xEE);
	__m128 tmp1 = _mm_shuffle_ps(m[2].vec_comp, m[3].vec_comp, 0x44);
	__m128 tmp3 = _mm_shuffle_ps(m[2].vec_comp, m[3].vec_comp, 0xEE);

	result[0].vec_comp = _mm_shuffle_ps(tmp0, tmp1, 0x88);
	result[1].vec_comp = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
	result[2].vec_comp = _mm_shuffle_ps(tmp2, tmp3, 0x88);
	result[3].vec_comp = _mm_shuffle_ps(tmp2, tmp3, 0xDD);
#endif
	return result;
}

Matrix4x4 Matrix4x4::CreateTranslation(const Vector3& p_trans)
{
	return Matrix4x4(
		1, 0, 0, 0,
		0, 1, 0, 0, 
		0, 0, 1, 0,
		p_trans.x, p_trans.y, p_trans.z, 1
	);
}

Matrix4x4 Matrix4x4::Translate(const Matrix4x4& p_matrix, const Vector3& p_trans)
{
	 return p_matrix * CreateTranslation(p_trans);
}

Matrix4x4 Matrix4x4::CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Vector3 const f(Vector3::Normalize(target - eye));
	Vector3 const s(Vector3::Normalize(Vector3::Cross(f, up)));
	Vector3 const u(Vector3::Cross(s, f));

	Matrix4x4 Result(1);
	Result[0][0] = s.x;
	Result[1][0] = s.y;
	Result[2][0] = s.z;
	Result[0][1] = u.x;
	Result[1][1] = u.y;
	Result[2][1] = u.z;
	Result[0][2] = -f.x;
	Result[1][2] = -f.y;
	Result[2][2] = -f.z;
	Result[3][0] = -Vector3::Dot(s, eye);
	Result[3][1] = -Vector3::Dot(u, eye);
	Result[3][2] = Vector3::Dot(f, eye);
	return Result;
}

Matrix4x4 Matrix4x4::LookAt(const Matrix4x4& p_matrix, const Vector3& eye, const Vector3& target, const Vector3& up)
{
	return p_matrix * CreateLookAt(eye, target, up);
}

Matrix4x4 Matrix4x4::CreateScale(const float& xScale, const float& yScale, const float& zScale)
{
	return Matrix4x4(xScale, 0 , 0, 0,
					 0, yScale, 0, 0,
					 0, 0, zScale, 0,
					 0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::CreateScale(const Vector3& p_vec)
{
	return Matrix4x4(p_vec.x, 0, 0, 0,
		0, p_vec.y, 0, 0,
		0, 0, p_vec.z, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::CreateScale(const float& scale)
{
	return Matrix4x4(scale, 0, 0, 0,
		0, scale, 0, 0,
		0, 0, scale, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::Scale(const Matrix4x4& p_matrix, const Vector3& p_vec)
{
	return p_matrix * CreateScale(p_vec);
}

Matrix4x4 Matrix4x4::CreateRotationX(const float& p_theta)
{
	return Matrix4x4(1, 0, 0, 0,
		0, Math::Cos(p_theta), Math::Sin(p_theta), 0,
		0, -Math::Sin(p_theta), Math::Cos(p_theta), 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::Scale(const Matrix4x4& p_matrix, const float& scale)
{
	return p_matrix * CreateScale(scale);
}

Matrix4x4 Matrix4x4::RotateX(const Matrix4x4& p_matrix, const float& p_theta)
{
	return p_matrix * CreateRotationX(p_theta);
}

Matrix4x4 Matrix4x4::CreateRotationY(const float& p_theta)
{
	return Matrix4x4(Math::Cos(p_theta), 0, -Math::Sin(p_theta), 0,
					 0, 1, 0, 0,
					 Math::Sin(p_theta), 0, Math::Cos(p_theta), 0,
					 0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::RotateY(const Matrix4x4& p_matrix, const float& p_theta)
{
	return p_matrix * CreateRotationY(p_theta);
}

Matrix4x4 Matrix4x4::CreateRotationZ(const float& p_theta)
{
	return Matrix4x4(Math::Cos(p_theta), -Math::Sin(p_theta), 0, 0,
		0, 1, 0, 0,
		Math::Sin(p_theta), Math::Cos(p_theta), 0, 0,
		0, 0, 0, 1);
}

Matrix4x4 Matrix4x4::Scale(const Matrix4x4& p_matrix, const float& xScale, const float& yScale, const float& zScale)
{
	return p_matrix * CreateScale(xScale,yScale,zScale);
}

Matrix4x4 Matrix4x4::RotateZ(const Matrix4x4& p_matrix, const float& p_theta)
{
	return p_matrix * CreateRotationZ(p_theta);
}

Matrix4x4 Matrix4x4::Rotate(const Matrix4x4& p_matrix, const Vector3& p_axis, const float& p_theta)
{
	const float c = Math::Cos(p_theta);
	const float s = Math::Sin(p_theta);

	auto axis = Vector3::Normalize(p_axis);
	Matrix4x4 rotation(0);
	Vector3 temp((1.f - c) * axis);

	rotation[0][0] = c + temp[0] * axis[0];
	rotation[0][1] = 0 + temp[0] * axis[1] + s * axis[2];
	rotation[0][2] = 0 + temp[0] * axis[2] - s * axis[1];

	rotation[1][0] = 0 + temp[1] * axis[0] - s * axis[2];
	rotation[1][1] = c + temp[1] * axis[1];
	rotation[1][2] = 0 + temp[1] * axis[2] + s * axis[0];

	rotation[2][0] = 0 + temp[2] * axis[0] + s * axis[1];
	rotation[2][1] = 0 + temp[2] * axis[1] - s * axis[0];
	rotation[2][2] = c + temp[2] * axis[2];

	Matrix4x4 result(0);
	result[0] = p_matrix[0] * rotation[0][0] + p_matrix[1] * rotation[0][1] + p_matrix[2] * rotation[0][2];
	result[1] = p_matrix[0] * rotation[1][0] + p_matrix[1] * rotation[1][1] + p_matrix[2] * rotation[1][2];
	result[2] = p_matrix[0] * rotation[2][0] + p_matrix[1] * rotation[2][1] + p_matrix[2] * rotation[2][2];
	result[3] = p_matrix[3];
	return result;

}

Matrix4x4 Matrix4x4::Rotate(const Quaternion& p_rotation)
{
	return Quaternion::ToMatrix4x4(p_rotation);
}

Matrix4x4 Matrix4x4::Rotate(const Matrix4x4& p_matrix, const Quaternion& p_rotation)
{
	return p_matrix * Rotate(p_rotation);
}