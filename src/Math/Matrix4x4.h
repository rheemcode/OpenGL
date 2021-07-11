#pragma once
#include "Math/SimpleVec.h"
#include "Vector3.h"
#include "Matrix3x3.h"
#include "Math/Matrix4x4.h"

class Matrix4x4
{
	typedef SimpleVec4 colType;
public:
	Matrix4x4();
	Matrix4x4(const Matrix4x4& p_mat);
	Matrix4x4(const class Matrix3x3& p_mat);
	Matrix4x4(const colType& col1, const colType& col2, const colType& col3, const colType& col4 );
	Matrix4x4(const float& s);
	Matrix4x4
	(
		const float& x0, const float& y0, const float& z0, const float& w0,
		const float& x1, const float& y1, const float& z1, const float& w1,
		const float& x2, const float& y2, const float& z2, const float& w2,
		const float& x3, const float& y3, const float& z3, const float& w3
	);


	const float* GetAsFloatPtr() const
	{
		return &m_data[0].x;
	}
	
	static const float* GetAsFloatPtr(const Matrix4x4& p_mat)
	{
		return &p_mat[0].x;
	}

	friend Vector4 operator*(const Matrix4x4& p_mat, const Vector4& p_vec);
	friend Matrix4x4 operator*(const Matrix4x4& p_left, const Matrix4x4& p_right);
	Matrix4x4& operator*(const float& p_s);
	Matrix4x4& operator*=(const Matrix4x4& p_matrix);
	const SimpleVec4& operator[](const int& p_index) const;
	SimpleVec4& operator[](const int& p_index);
	Matrix4x4& operator+=(const Matrix4x4& p_matrix);
	Matrix4x4& operator=(const Matrix4x4& p_matrix);
	bool operator==(const Matrix4x4& p_matrix);

	//Matrix4x4 AffineInverse(const Matrix4x4& p_matrix);
	static Matrix4x4 Inverse(const Matrix4x4& p_matrix);

	static Matrix4x4 CreateScale(const float& xScale, const float& yScale, const float& zScale);
	static Matrix4x4 CreateScale(const struct Vector3& scaleVector);
	static Matrix4x4 CreateScale(const float& scale);
	static Matrix4x4 Scale(const Matrix4x4& p_matrix, const struct Vector3& scaleVector);
	static Matrix4x4 CreateRotationX(const float& theta);
	static Matrix4x4 CreateRotationY(const float& theta);
	static Matrix4x4 CreateRotationZ(const float& theta);
	static Matrix4x4 Scale(const Matrix4x4& p_matrix, const float& xScale, const float& yScale, const float& zScale);
	static Matrix4x4 Scale(const Matrix4x4& p_matrix, const float& scale);
	static Matrix4x4 RotateX(const Matrix4x4& p_matrix, const float& theta);
	static Matrix4x4 RotateY(const Matrix4x4& p_matrix, const float& theta);
	static Matrix4x4 RotateZ(const Matrix4x4& p_matrix, const float& theta);
	static Matrix4x4 Rotate(const struct Quaternion& p_rotation);
	static Matrix4x4 Rotate(const Matrix4x4& p_matrix, const struct Quaternion& q);
	static Matrix4x4 Rotate(const Matrix4x4& p_matrix, const struct Vector3& p_axis, const float& p_theta);
	static Matrix4x4 CreateTranslation(const struct Vector3& trans);

	static Matrix4x4 Translate(const Matrix4x4& p_matrix, const struct Vector3& trans);

	static Matrix4x4 CreateLookAt(const struct Vector3& eye, const struct Vector3& target, const struct Vector3& up);
	static Matrix4x4 LookAt(const Matrix4x4& p_matrix, const struct Vector3& eye, const struct Vector3& target, const struct Vector3& up);
	
	inline Vector3 GetScale() const
	{
		return Vector3
		(
			Math::Sqrt(m_data[0].x * m_data[0].x + m_data[1].x * m_data[1].x + m_data[2].x * m_data[2].x),
			Math::Sqrt(m_data[0].x * m_data[0].y + m_data[1].y * m_data[1].y + m_data[2].y * m_data[2].y),
			Math::Sqrt(m_data[0].x * m_data[0].z + m_data[1].z * m_data[1].z + m_data[2].z * m_data[2].z)
		);
	}

	inline Matrix3x3 GetRotation() const 
	{
		Vector3 invScale
		(
			1.f / Math::Sqrt(m_data[0].x * m_data[0].x + m_data[1].x * m_data[1].x + m_data[2].x * m_data[2].x),
			1.f / Math::Sqrt(m_data[0].x * m_data[0].y + m_data[1].y * m_data[1].y + m_data[2].y * m_data[2].y),
			1.f / Math::Sqrt(m_data[0].x * m_data[0].z + m_data[1].z * m_data[1].z + m_data[2].z * m_data[2].z)
		);

		return Matrix3x3::Scale(Matrix3x3(*this), invScale);
	}

	static float Determinant(const Matrix4x4& p_matrix);
	static Matrix4x4 Transpose(const Matrix4x4& p_matrix);
	static Matrix4x4 CreateOrtho(const float& p_left, const float& p_right, const float& p_bottom, const float& p_top, const float& p_zNear, const float& p_Zfar);
	static Matrix4x4 CreatePerspectiveFOV(const float& fovY, const float& width, const float& height, const float& near, const float& far);
	static Matrix4x4 CreatePerspective(const float& p_fov, const float& p_aspectRatio, const float& p_zNear, const float& p_zFar);
	static Matrix4x4 CreateFrustum(const float& left, const float& right, const float& bottom, const float& top, const float& nearVal, const float& farVal);
	static const Matrix4x4 Identity;

private:
	colType m_data[4];
};