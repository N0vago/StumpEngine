#ifndef ST_MATRIX3X4_H
#define ST_MATRIX3X4_H

#include <cstring>
#include "Matrix3x3.h"
#include "Vector3.h"
#include "Plane.h"

class Matrix3x4
{
public:
	Matrix3x3 mat3;
	Vector3 origin;

	Matrix3x4(num_fd xx, num_fd xy, num_fd xz, num_fd yx, num_fd yy, num_fd yz, num_fd zx, num_fd zy, num_fd zz, num_fd ox, num_fd oy, num_fd oz);
	Matrix3x4(const Matrix3x3& p_mat3, const Vector3& p_origin = Vector3());
	Matrix3x4();

	void Invert();
	Matrix3x4 Inverse() const;

	void AffineInvert();
	Matrix3x4 AffineInverse() const;


	void Rotate(const Vector3& p_axis, num_fd p_angle);
	Matrix3x4 Rotated(const Vector3& p_axis, num_fd p_angle) const;

	void Rotate(const Quaternion& p_quat);
	Matrix3x4 Rotated(const Quaternion& p_quat) const;

	void SetLookAt(const Vector3& p_eye, const Vector3& p_target, const Vector3& p_up);
	Matrix3x4 LookingAt(const Vector3& p_target, const Vector3& p_up) const;

	void Scale(const Vector3& p_scale);
	Matrix3x4 Scaled(const Vector3& p_scale) const;
	void ScaleMat3(const Vector3& p_scale);
	void Translate(num_fd p_tx, num_fd p_ty, num_fd p_tz);
	void Translate(const Vector3& p_translation);
	Matrix3x4 Translated(const Vector3& p_translation) const;

	const Matrix3x3& GetMat3() const { return mat3; }
	void SetMat3(const Matrix3x3& p_mat3) { mat3 = p_mat3; }

	const Vector3& SetOrigin() const { return origin; }
	void SetOrigin(const Vector3& p_origin) { origin = p_origin; }

	void Orthonormalize();
	Matrix3x4 Orthonormalized() const;
	bool IsEqualApprox(const Matrix3x4& p_transform) const;

	bool operator==(const Matrix3x4& p_transform) const;
	bool operator!=(const Matrix3x4& p_transform) const;

	Vector3 XForm(const Vector3& p_vector) const;

	// NOTE: These are UNSAFE with non-uniform scaling, and will produce incorrect results.
	// They use the transpose.
	// For safe inverse transforms, xform by the affine_inverse.
	Vector3 XFormInv(const Vector3& p_vector) const;

	Plane XForm(const Plane& p_plane) const;
	Plane XFormInv(const Plane& p_plane) const;

	Plane XFormFast(const Plane& p_plane, const Matrix3x3& p_mat3_inverse_transpose) const;
	static Plane XFormInvFast(const Plane& p_plane, const Matrix3x4& p_inverse, const Matrix3x3& p_mat3_transpose);

	void operator*=(const Matrix3x4& p_transform);
	Matrix3x4 operator*(const Matrix3x4& p_transform) const;


	Matrix3x4 InterpolateWith(const Matrix3x4& p_transform, num_fd p_c) const;

	Matrix3x4 InverseXform(const Matrix3x4& t) const {
		Vector3 v = t.origin - origin;
		return Matrix3x4(mat3.TransposeXform(t.mat3),
			mat3.XForm(v));
	}

	void set(num_fd xx, num_fd xy, num_fd xz, num_fd yx, num_fd yy, num_fd yz, num_fd zx, num_fd zy, num_fd zz, num_fd tx, num_fd ty, num_fd tz) {
		mat3.Set(xx, xy, xz, yx, yy, yz, zx, zy, zz);
		origin.x = tx;
		origin.y = ty;
		origin.z = tz;
	}

	Matrix3x4& operator=(const Matrix3x4& other)
	{
		if (this != &other)
		{
			std::memcpy(this, &other, sizeof(Matrix3x4));
		}
		return *this;
	}

	float* ToRenderMatrix() const
	{
		static float renderMat[4][4];
		renderMat[0][0] = mat3[0][0];
		renderMat[0][1] = mat3[0][1];
		renderMat[0][2] = mat3[0][2];
		renderMat[0][3] = origin.x;
		renderMat[1][0] = mat3[1][0];
		renderMat[1][1] = mat3[1][1];
		renderMat[1][2] = mat3[1][2];
		renderMat[1][3] = origin.y;
		renderMat[2][0] = mat3[2][0];
		renderMat[2][1] = mat3[2][1];
		renderMat[2][2] = mat3[2][2];
		renderMat[2][3] = origin.z;
		renderMat[3][0] = 0.0f;
		renderMat[3][1] = 0.0f;
		renderMat[3][2] = 0.0f;
		renderMat[3][3] = 1.0f;
		return &renderMat[0][0];
	}

	Matrix3x4(const Matrix3x4& other)
	{
		std::memcpy(this, &other, sizeof(Matrix3x4));
	}

	Matrix3x4(Matrix3x4&& other) noexcept
	{
		std::memcpy(this, &other, sizeof(Matrix3x4));
	}


	Matrix3x4& operator=(Matrix3x4&& other) noexcept
	{
		if (this != &other)
		{
			std::memcpy(this, &other, sizeof(Matrix3x4));
		}
		return *this;
	}
};


#endif // ST_TRANSFORMMAT_H

