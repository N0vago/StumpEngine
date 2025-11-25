#include "Matrix3x4.h"
#include "Matrix4x4.h"
#include "Math.h"

Matrix3x4::Matrix3x4(const Matrix3x3& p_mat3, const Vector3& p_origin) :
	mat3(p_mat3),
	origin(p_origin) {
}

Matrix3x4::Matrix3x4(num_fd xx, num_fd xy, num_fd xz, num_fd yx, num_fd yy, num_fd yz, num_fd zx, num_fd zy, num_fd zz, num_fd ox, num_fd oy, num_fd oz) {
	mat3 = Matrix3x3(xx, xy, xz, yx, yy, yz, zx, zy, zz);
	origin = Vector3(ox, oy, oz);
}
Matrix3x4::Matrix3x4() : mat3(), origin() {
}
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

void Matrix3x4::AffineInvert() {
	mat3.Invert();
	origin = mat3.XForm(-origin);
}

Matrix3x4 Matrix3x4::AffineInverse() const {
	Matrix3x4 ret = *this;
	ret.AffineInvert();
	return ret;
}

void Matrix3x4::Invert() {
	mat3.Transpose();
	origin = mat3.XForm(-origin);
}

Matrix3x4 Matrix3x4::Inverse() const {
	Matrix3x4 ret = *this;
	ret.Invert();
	return ret;
}

void Matrix3x4::Rotate(const Vector3& p_axis, num_fd p_angle) {
	*this = Rotated(p_axis, p_angle);
}

Matrix3x4 Matrix3x4::Rotated(const Vector3& p_axis, num_fd p_angle) const {
	return Matrix3x4(Matrix3x3(p_axis, p_angle), Vector3()) * (*this);
}

void Matrix3x4::Rotate(const Quaternion& p_quat) {
	*this = Rotated(p_quat);
}

Matrix3x4 Matrix3x4::Rotated(const Quaternion& p_quat) const {
	return Matrix3x4(Matrix3x3(p_quat), Vector3()) * (*this);
}

Matrix3x4 Matrix3x4::LookingAt(const Vector3& p_target, const Vector3& p_up) const {
	Matrix3x4 t = *this;
	t.SetLookAt(origin, p_target, p_up);
	return t;
}

void Matrix3x4::SetLookAt(const Vector3& p_eye, const Vector3& p_target, const Vector3& p_up) {
	Vector3 v_x, v_y, v_z;
	v_z = (p_eye - p_target).Normalized();
	v_x = p_up.Cross(v_z).Normalized();
	v_y = v_z.Cross(v_x);
	
	mat3.Set(v_x, v_y, v_z);
	
	origin = p_eye;
}

Matrix3x4 Matrix3x4::InterpolateWith(const Matrix3x4& p_transform, num_fd p_c) const {

	Vector3 src_scale = mat3.GetScale();
	Quaternion src_rot = mat3.GetRotationQuat();
	Vector3 src_loc = origin;

	Vector3 dst_scale = p_transform.mat3.GetScale();
	Quaternion dst_rot = p_transform.mat3.GetRotationQuat();
	Vector3 dst_loc = p_transform.origin;

	Matrix3x4 interp;
	interp.mat3.SetQuatScale(src_rot.Slerp(dst_rot, p_c).Normalized(), src_scale.LinearInterpolate(dst_scale, p_c));
	interp.origin = src_loc.LinearInterpolate(dst_loc, p_c);

	return interp;
}

void Matrix3x4::Scale(const Vector3& p_scale) {
	mat3.Scale(p_scale);
	origin *= p_scale;
}

Matrix3x4 Matrix3x4::Scaled(const Vector3& p_scale) const {
	Matrix3x4 t = *this;
	t.Scale(p_scale);
	return t;
}

void Matrix3x4::ScaleMat3(const Vector3& p_scale) {
	mat3.Scale(p_scale);
}

void Matrix3x4::Translate(num_fd p_tx, num_fd p_ty, num_fd p_tz) {
	Translate(Vector3(p_tx, p_ty, p_tz));
}
void Matrix3x4::Translate(const Vector3& p_translation) {
	for (int i = 0; i < 3; i++) {
		origin[i] += mat3[i].Dot(p_translation);
	}
}

Matrix3x4 Matrix3x4::Translated(const Vector3& p_translation) const {
	Matrix3x4 t = *this;
	t.Translate(p_translation);
	return t;
}

void Matrix3x4::Orthonormalize() {
	mat3.Orthonormalize();
}

Matrix3x4 Matrix3x4::Orthonormalized() const {
	Matrix3x4 _copy = *this;
	_copy.Orthonormalize();
	return _copy;
}

bool Matrix3x4::IsEqualApprox(const Matrix3x4& p_transform) const {
	return mat3.IsEqualApprox(p_transform.mat3) && origin.IsEqualApprox(p_transform.origin);
}

bool Matrix3x4::operator==(const Matrix3x4& p_transform) const {
	return (mat3 == p_transform.mat3 && origin == p_transform.origin);
}
bool Matrix3x4::operator!=(const Matrix3x4& p_transform) const {
	return (mat3 != p_transform.mat3 || origin != p_transform.origin);
}

void Matrix3x4::operator*=(const Matrix3x4& p_transform) {
	origin = XForm(p_transform.origin);
	mat3 *= p_transform.mat3;
}

Matrix3x4 Matrix3x4::operator*(const Matrix3x4& p_transform) const {
	Matrix3x4 t = *this;
	t *= p_transform;
	return t;
}

