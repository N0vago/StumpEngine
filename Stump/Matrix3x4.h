#ifndef ST_MATRIX3X4_H
#define ST_MATRIX3X4_H

#include "Matrix3x3.h"
#include "Vector3.h"
#include "Plane.h"
class Matrix3x4
{
public:
	Matrix3x3 mat3;
	Vector3 origin;

	void Invert();
	Matrix3x4 Inverse() const;

	void AffineInvert();
	Matrix3x4 AffineInverse() const;

	Matrix3x4 Rotated(const Vector3& p_axis, num_fd p_angle) const;

	void Rotate(const Vector3& p_axis, num_fd p_angle);
	void RotateMat3(const Vector3& p_axis, num_fd p_angle);

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

	Matrix3x4(num_fd xx, num_fd xy, num_fd xz, num_fd yx, num_fd yy, num_fd yz, num_fd zx, num_fd zy, num_fd zz, num_fd ox, num_fd oy, num_fd oz);
	Matrix3x4(const Matrix3x3& p_mat3, const Vector3& p_origin = Vector3());
	Matrix3x4() {}


};

Vector3 Matrix3x4::XForm(const Vector3& p_vector) const {
	return Vector3(
		mat3[0].Dot(p_vector) + origin.x,
		mat3[1].Dot(p_vector) + origin.y,
		mat3[2].Dot(p_vector) + origin.z);
}
Plane Matrix3x4::XForm(const Plane& p_plane) const {
	Matrix3x3 b = mat3.Inverse();
	b.Transpose();
	return XFormFast(p_plane, b);
}

Plane Matrix3x4::XFormInv(const Plane& p_plane) const {
	Matrix3x4 inv = AffineInverse();
	Matrix3x3 basis_transpose = mat3.Transposed();
	return XFormInvFast(p_plane, inv, basis_transpose);
}


Vector3 Matrix3x4::XFormInv(const Vector3& p_vector) const {
	Vector3 v = p_vector - origin;

	return Vector3(
		(mat3.elements[0][0] * v.x) + (mat3.elements[1][0] * v.y) + (mat3.elements[2][0] * v.z),
		(mat3.elements[0][1] * v.x) + (mat3.elements[1][1] * v.y) + (mat3.elements[2][1] * v.z),
		(mat3.elements[0][2] * v.x) + (mat3.elements[1][2] * v.y) + (mat3.elements[2][2] * v.z));
}

Plane Matrix3x4::XFormFast(const Plane& p_plane, const Matrix3x3& p_mat3_inverse_transpose) const {
	// Matrix3x4 a single point on the plane.
	Vector3 point = p_plane.normal * p_plane.d;
	point = XForm(point);

	// Use inverse transpose for correct normals with non-uniform scaling.
	Vector3 normal = p_mat3_inverse_transpose.XForm(p_plane.normal);
	normal.Normalize();

	num_fd d = normal.Dot(point);
	return Plane(normal, d);
}

Plane Matrix3x4::XFormInvFast(const Plane& p_plane, const Matrix3x4& p_inverse, const Matrix3x3& p_mat3_transpose) {
	// Matrix3x4 a single point on the plane.
	Vector3 point = p_plane.normal * p_plane.d;
	point = p_inverse.XForm(point);

	// Note that instead of precalculating the transpose, an alternative
	// would be to use the transpose for the basis transform.
	// However that would be less SIMD friendly (requiring a swizzle).
	// So the cost is one extra precalced value in the calling code.
	// This is probably worth it, as this could be used in bottleneck areas. And
	// where it is not a bottleneck, the non-fast method is fine.

	// Use transpose for correct normals with non-uniform scaling.
	Vector3 normal = p_mat3_transpose.XForm(p_plane.normal);
	normal.Normalize();

	num_fd d = normal.Dot(point);
	return Plane(normal, d);
}

#endif // ST_TRANSFORMMAT_H

