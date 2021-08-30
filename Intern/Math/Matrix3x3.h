#pragma once
#include "GLCore.h"
#include "SimpleVec.h"

class Matrix4x4;
class GLIB_API Matrix3x3
{
	typedef SimpleVec3 colType;
public:
	Matrix3x3();
	Matrix3x3(const Matrix3x3& p_mat);
	Matrix3x3(const Matrix4x4& p_mat);
	Matrix3x3(const colType& col1, const colType& col2, const colType& col3);
	Matrix3x3(const float& s);
	Matrix3x3
	(
		const float& x0, const float& y0, const float& z0,
		const float& x1, const float& y1, const float& z1,
		const float& x2, const float& y2, const float& z2
	);


	const float* GetAsFloatPtr() const
	{
		return reinterpret_cast<const float*>(&m_data[0]);
	}


	friend Matrix3x3 operator*(const Matrix3x3& p_left, const Matrix3x3& p_right);
	friend colType operator*(const Matrix3x3& p_left, const colType& p_vec3);
	Matrix3x3& operator*=(const Matrix3x3& p_matrix);
	colType operator[](const int& p_index) const;
	colType& operator[](const int& p_index);
	Matrix3x3& operator+=(const Matrix3x3& p_matrix);
	Matrix3x3& operator=(const Matrix3x3& p_matrix);
	bool operator==(const Matrix3x3& p_matrix);

	static Matrix3x3 Inverse(const Matrix3x3& p_matrix);
	static Matrix3x3 AffineInverse(const Matrix3x3& p_matrix);

	static Matrix3x3 CreateScale(const float& xScale, const float& yScale, const float& zScale);
	static Matrix3x3 CreateScale(const struct Vector3& scaleVector);
	static Matrix3x3 CreateScale(const float& scale);
	static Matrix3x3 Scale(const Matrix3x3& p_matrix, const struct Vector3& scaleVector);
	static Matrix3x3 Scale(const Matrix3x3& p_matrix, const float& xScale, const float& yScale, const float& zScale);
	static Matrix3x3 Scale(const Matrix3x3& p_matrix, const float& scale);
	static Matrix3x3 Rotate(const Matrix3x3& p_matrix, const float& p_theta);
	static Matrix3x3 CreateTranslation(const struct Vector3& trans);
	static Matrix3x3 Translate(const Matrix3x3& p_matrix, const struct Vector3& trans);
	static float Determinant(const Matrix3x3& p_matrix);
	static Matrix3x3 Transpose(const Matrix3x3& p_matrix);
	

	static const Matrix3x3 Identity;

private:
	colType m_data[3];
};

