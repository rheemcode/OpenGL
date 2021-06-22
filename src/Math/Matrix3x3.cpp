
#include "Math/Matrix4x4.h"
#include "Math/Matrix3x3.h"


const Matrix3x3 Matrix3x3::Identity = Matrix3x3(
	1.f, 0.f, 0.f,
	0.f, 1.f, 0.f,
	0.f, 0.f, 1.f);

Matrix3x3::Matrix3x3(const Matrix4x4& p_mat)
{
	this->m_data[0] = p_mat[0];
	this->m_data[1] = p_mat[1];
	this->m_data[2] = colType(0.f, 0.f, 1.f);
}

Matrix3x3::Matrix3x3()
{
	*this = Matrix3x3
	(
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f
	);
}


Matrix3x3::Matrix3x3(const Matrix3x3& p_mat)
{
	m_data[0] = p_mat.m_data[0];
	m_data[1] = p_mat.m_data[1];
	m_data[2] = p_mat.m_data[2];
	m_data[3] = p_mat.m_data[3];
}

Matrix3x3::Matrix3x3
(
	const float& x0, const float& y0, const float& z0,
	const float& x1, const float& y1, const float& z1,
	const float& x2, const float& y2, const float& z2

)
{
	m_data[0] = colType(x0, y0, z0);
	m_data[1] = colType(x1, y1, z1);
	m_data[2] = colType(x2, y2, z2);
}

Matrix3x3::Matrix3x3(const float& s)
{
	m_data[0] = colType(s, 0, 0);
	m_data[1] = colType(0, s, 0);
	m_data[2] = colType(0, 0, s);

}

SimpleVec3 Matrix3x3::operator[](const int& p_index) const
{
	return m_data[p_index];
}

SimpleVec3& Matrix3x3::operator[](const int& p_index)
{
	return m_data[p_index];
}

Matrix3x3 operator*(const Matrix3x3& p_mat, const Matrix3x3& p_mat2)
{
	const float SrcA00 = p_mat[0][0];
	const float SrcA01 = p_mat[0][1];
	const float SrcA02 = p_mat[0][2];
	const float SrcA10 = p_mat[1][0];
	const float SrcA11 = p_mat[1][1];
	const float SrcA12 = p_mat[1][2];
	const float SrcA20 = p_mat[2][0];
	const float SrcA21 = p_mat[2][1];
	const float SrcA22 = p_mat[2][2];

	const float SrcB00 = p_mat2[0][0];
	const float SrcB01 = p_mat2[0][1];
	const float SrcB02 = p_mat2[0][2];
	const float SrcB10 = p_mat2[1][0];
	const float SrcB11 = p_mat2[1][1];
	const float SrcB12 = p_mat2[1][2];
	const float SrcB20 = p_mat2[2][0];
	const float SrcB21 = p_mat2[2][1];
	const float SrcB22 = p_mat2[2][2];

	Matrix3x3 Result(0);
	Result[0][0] = SrcA00 * SrcB00 + SrcA10 * SrcB01 + SrcA20 * SrcB02;
	Result[0][1] = SrcA01 * SrcB00 + SrcA11 * SrcB01 + SrcA21 * SrcB02;
	Result[0][2] = SrcA02 * SrcB00 + SrcA12 * SrcB01 + SrcA22 * SrcB02;
	Result[1][0] = SrcA00 * SrcB10 + SrcA10 * SrcB11 + SrcA20 * SrcB12;
	Result[1][1] = SrcA01 * SrcB10 + SrcA11 * SrcB11 + SrcA21 * SrcB12;
	Result[1][2] = SrcA02 * SrcB10 + SrcA12 * SrcB11 + SrcA22 * SrcB12;
	Result[2][0] = SrcA00 * SrcB20 + SrcA10 * SrcB21 + SrcA20 * SrcB22;
	Result[2][1] = SrcA01 * SrcB20 + SrcA11 * SrcB21 + SrcA21 * SrcB22;
	Result[2][2] = SrcA02 * SrcB20 + SrcA12 * SrcB21 + SrcA22 * SrcB22;
	return Result;
}

SimpleVec3 operator*(const Matrix3x3& m, const SimpleVec3& v)
{
	return SimpleVec3(
		m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
		m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
		m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z);
}


Matrix3x3& Matrix3x3::operator=(const Matrix3x3& p_mat)
{
	m_data[0] = p_mat.m_data[0];
	m_data[1] = p_mat.m_data[1];
	m_data[2] = p_mat.m_data[2];
	m_data[3] = p_mat.m_data[3];
	return *this;
}

Matrix3x3::Matrix3x3(const colType& col1, const colType& col2, const colType& col3)
{
	m_data[0] = col1;
	m_data[1] = col2;
	m_data[2] = col3;
}
Matrix3x3& Matrix3x3::operator+=(const Matrix3x3& p_mat)
{
	m_data[0] += p_mat.m_data[0];
	m_data[1] += p_mat.m_data[1];
	m_data[2] += p_mat.m_data[2];
	return *this;
}

bool Matrix3x3::operator==(const Matrix3x3& p_mat)
{
	return (m_data[0] == p_mat.m_data[0]) &&
		(m_data[1] == p_mat.m_data[1]) &&
		(m_data[2] == p_mat.m_data[2]);
}

Matrix3x3 Matrix3x3::Inverse(const Matrix3x3& m)
{
	float OneOverDeterminant = 1.f / (
		+m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
		- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
		+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

	Matrix3x3 Inverse(0);
	Inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * OneOverDeterminant;
	Inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * OneOverDeterminant;
	Inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * OneOverDeterminant;
	Inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * OneOverDeterminant;
	Inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * OneOverDeterminant;
	Inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * OneOverDeterminant;
	Inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * OneOverDeterminant;
	Inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * OneOverDeterminant;
	Inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * OneOverDeterminant;

	return Inverse;
}

Matrix3x3& Matrix3x3::operator*=(const Matrix3x3& p_matrix)
{
	*this = *this * p_matrix;
	return *this;
}


float Matrix3x3::Determinant(const Matrix3x3& m)
{
	return
		+m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
		- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
		+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
}

Matrix3x3 Matrix3x3::Transpose(const Matrix3x3& m)
{
	Matrix3x3 result(0);
	result[0][0] = m[0][0];
	result[0][1] = m[1][0];
	result[0][2] = m[2][0];

	result[1][0] = m[0][1];
	result[1][1] = m[1][1];
	result[1][2] = m[2][1];

	result[2][0] = m[0][2];
	result[2][1] = m[1][2];
	result[2][2] = m[2][2];
	return result;
}

Matrix3x3 Matrix3x3::CreateTranslation(const Vector3& p_trans)
{
	return Matrix3x3(
		1, 0, 0,
		0, 1, 0,
		p_trans.x, p_trans.y, p_trans.z
	);
}

Matrix3x3 Matrix3x3::Translate(const Matrix3x3& p_matrix, const Vector3& p_trans)
{
	return p_matrix * CreateTranslation(p_trans);
}


Matrix3x3 Matrix3x3::CreateScale(const float& xScale, const float& yScale, const float& zScale)
{
	return Matrix3x3(xScale, 0.f, 0.f,
					0.f, yScale, 0.f,
					0.f, 0.f, zScale
					);
}

Matrix3x3 Matrix3x3::CreateScale(const Vector3& p_vec)
{
	return Matrix3x3(p_vec.x, 0, 0,
		0, p_vec.y, 0,
		0, 0, p_vec.z
	);
}

Matrix3x3 Matrix3x3::CreateScale(const float& scale)
{
	return Matrix3x3(scale, 0.f, 0.f,
		0.f, scale, 0.f,
		0.f, 0.f, scale
	);
}

Matrix3x3 Matrix3x3::Scale(const Matrix3x3& p_matrix, const Vector3& p_vec)
{
	return p_matrix * CreateScale(p_vec);
}

Matrix3x3 Matrix3x3::Scale(const Matrix3x3& p_matrix, const float& scale)
{
	return p_matrix * CreateScale(scale);
}

Matrix3x3 Matrix3x3::Scale(const Matrix3x3& p_matrix, const float& xScale, const float& yScale, const float& zScale)
{
	return p_matrix * CreateScale(xScale, yScale, zScale);
}


Matrix3x3 Matrix3x3::Rotate(const Matrix3x3& p_matrix, const float& p_rotation)
{
	return Matrix3x3(Math::Cos(p_rotation), -Math::Sin(p_rotation), 0,
		Math::Sin(p_rotation), Math::Cos(p_rotation), 0,
		0, 0, 1);
}


Matrix3x3 Matrix3x3::AffineInverse(const Matrix3x3& m)
{
	Matrix3x3 result(m);
	result[2] = colType(0, 0, 1);
	result = Transpose(result);
	colType translation = result * colType(-m[2][0], -m[2][1], m[2][2]);
	result[2] = translation;
	return result;
}

