#pragma once
#include "Math/SimpleVec.h"


class Matrix4x4
{
	typedef SimpleVec4 colType;
public:
	Matrix4x4();
	Matrix4x4(const Matrix4x4& p_mat);
	Matrix4x4(const Matrix3x3& p_mat);
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
		return reinterpret_cast<const float*>(&m_data[0]);
	}


	friend Matrix4x4 operator*(const Matrix4x4& p_left, const Matrix4x4& p_right);
	Matrix4x4& operator*=(const Matrix4x4& p_matrix);
	const SimpleVec4& operator[](const int& p_index) const;
	SimpleVec4& operator[](const int& p_index);
	Matrix4x4& operator+=(const Matrix4x4& p_matrix);
	Matrix4x4& operator=(const Matrix4x4& p_matrix);
	bool operator==(const Matrix4x4& p_matrix);

	//Matrix4x4 AffineInverse(const Matrix4x4& p_matrix);
	Matrix4x4 Inverse(const Matrix4x4& p_matrix);

	static Matrix4x4 CreateScale(const float& xScale, const float& yScale, const float& zScale);
	static Matrix4x4 CreateScale(const Vector3& scaleVector);
	static Matrix4x4 CreateScale(const float& scale);
	static Matrix4x4 Scale(const Matrix4x4& p_matrix, const Vector3& scaleVector);
	static Matrix4x4 CreateRotationX(const float& theta);
	static Matrix4x4 CreateRotationY(const float& theta);
	static Matrix4x4 CreateRotationZ(const float& theta);
	static Matrix4x4 Scale(const Matrix4x4& p_matrix, const float& xScale, const float& yScale, const float& zScale);
	static Matrix4x4 Scale(const Matrix4x4& p_matrix, const float& scale);
	static Matrix4x4 RotateX(const Matrix4x4& p_matrix, const float& theta);
	static Matrix4x4 RotateY(const Matrix4x4& p_matrix, const float& theta);
	static Matrix4x4 RotateZ(const Matrix4x4& p_matrix, const float& theta);
	static Matrix4x4 Rotate(const Quaternion& p_rotation);
	static Matrix4x4 Rotate(const Matrix4x4& p_matrix, const Quaternion& q);
	static Matrix4x4 Rotate(const Matrix4x4& p_matrix, const Vector3& p_axis, const float& p_theta);
	static Matrix4x4 CreateTranslation(const Vector3& trans);

	static Matrix4x4 Translate(const Matrix4x4& p_matrix, const Vector3& trans);

	static Matrix4x4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
	static Matrix4x4 LookAt(const Matrix4x4& p_matrix, const Vector3& eye, const Vector3& target, const Vector3& up);

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