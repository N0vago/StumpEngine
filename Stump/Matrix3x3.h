#ifndef ST_MATRIX3X3_H
#define ST_MATRIX3X3_H

#include "Vector3.h"
#include "Quaternion.h"
#include "Typedefs.h"

class Matrix3x3
{
public:
	Vector3 elements[3] =
	{
		Vector3(1, 0, 0),
		Vector3(0, 1, 0),
		Vector3(0, 0, 1)
	};

	Matrix3x3(const Quaternion& p_quat);
	Matrix3x3(const Quaternion& p_quat, const Vector3& p_scale);

	Matrix3x3(const Vector3& p_euler);
	Matrix3x3(const Vector3& p_euler, const Vector3& p_scale);

	Matrix3x3(const Vector3& p_axis, num_fd p_angle);
	Matrix3x3(const Vector3& p_axis, num_fd p_angle, const Vector3& p_scale);

	Matrix3x3(const Vector3& row0, const Vector3& row1, const Vector3& row2);
	Matrix3x3();

	const Vector3& operator[](int index) const {
		return elements[index];
	}

	Vector3& operator[](int index) {
		return elements[index];
	}

	//Противоположная матрица
	void Invert();
	//Колонка становится строкой и наоборот
	void Transpose();

	Matrix3x3 Inverse() const;
	Matrix3x3 Transposed() const;

	//Определитель матрицы
	num_fd Determinant() const;

	//Установка матрицы из вектора Z (например, направление “вперёд” камеры или нормаль поверхности)
	void FromZ(const Vector3& p_z);

	Vector3 GetAxis(int p_axis) {
		return Vector3(
			elements[0][p_axis],
			elements[1][p_axis],
			elements[2][p_axis]
		);
	}

	void SetAxis(int p_axis, const Vector3& p_value) {
		elements[0][p_axis] = p_value.x;
		elements[1][p_axis] = p_value.y;
		elements[2][p_axis] = p_value.z;
	}

	void Rotate(const Vector3& p_axis, num_fd p_angle);
	Matrix3x3 Rotated(const Vector3& p_axis, num_fd p_angle) const;

	void RotateLocal(const Vector3& p_axis, num_fd p_angle);
	Matrix3x3 RotatedLocal(const Vector3& p_axis, num_fd p_angle) const;

	void Rotate(const Vector3& p_euler);
	Matrix3x3 Rotated(const Vector3& p_euler) const;

	void Rotate(const Quaternion& p_quat);
	Matrix3x3 Rotated(const Quaternion& p_quat) const;

	Vector3 GetRotationEuler() const;
	void GetRotationAxisAngle(Vector3& p_axis, num_fd& p_angle) const;
	void GetRotationAxisAngleLocal(Vector3& p_axis, num_fd& p_angle) const;
	Quaternion GetRotationQuat() const;
	Vector3 GetRotation() const { return GetRotationEuler(); };

	Vector3 RotrefPosscaleDecomposition(Matrix3x3& rotref) const;

	Vector3 GetEulerXYZ() const;
	void SetEulerXYZ(const Vector3& p_euler);

	Vector3 GetEulerXZY() const;
	void SetEulerXZY(const Vector3& p_euler);

	Vector3 GetEulerYZX() const;
	void SetEulerYZX(const Vector3& p_euler);

	Vector3 GetEulerYXZ() const;
	void SetEulerYXZ(const Vector3& p_euler);

	Vector3 GetEulerZXY() const;
	void SetEulerZXY(const Vector3& p_euler);

	Vector3 GetEulerZYX() const;
	void SetEulerZYX(const Vector3& p_euler);

	Quaternion GetQuat() const;
	void SetQuat(const Quaternion& p_quat);

	Vector3 GetEuler() const { return GetEulerYXZ(); }
	void SetEuler(const Vector3& p_euler) { SetEulerYXZ(p_euler); }

	void GetAxisAngle(Vector3& r_axis, num_fd& r_angle) const;
	void SetAxisAngle(const Vector3& p_axis, num_fd p_angle);

	void Scale(const Vector3& p_scale);
	Matrix3x3 Scaled(const Vector3& p_scale) const;

	void ScaleLocal(const Vector3& p_scale);
	Matrix3x3 ScaledLocal(const Vector3& p_scale) const;

	Vector3 GetScale() const;
	Vector3 GetScaleAbs() const;
	Vector3 GetScaleLocal() const;

	void SetAxisAngleScale(const Vector3& p_axis, num_fd p_angle, const Vector3& p_scale);
	void SetEulerScale(const Vector3& p_euler, const Vector3& p_scale);
	void SetQuatScale(const Quaternion& p_quat, const Vector3& p_scale);

	num_fd Tdotx(const Vector3& v) const {
		return elements[0][0] * v[0] + elements[1][0] * v[1] + elements[2][0] * v[2];
	}
	num_fd Tdoty(const Vector3& v) const {
		return elements[0][1] * v[0] + elements[1][1] * v[1] + elements[2][1] * v[2];
	}
	num_fd Tdotz(const Vector3& v) const {
		return elements[0][2] * v[0] + elements[1][2] * v[1] + elements[2][2] * v[2];
	}

	bool IsEqualApprox(const Matrix3x3& p_basis) const;
	// For complicated reasons, the second argument is always discarded. See #45062.
	bool IsEqualApprox(const Matrix3x3& a, const Matrix3x3& b) const { return IsEqualApprox(a); }
	bool IsEqualApproxRatio(const Matrix3x3& a, const Matrix3x3& b, num_fd p_epsilon = Math::UNIT_EPSILON) const;

	bool operator==(const Matrix3x3& p_matrix) const;
	bool operator!=(const Matrix3x3& p_matrix) const;

	Vector3 XForm(const Vector3& p_vector) const;
	Vector3 XFormInv(const Vector3& p_vector) const;
	void operator*=(const Matrix3x3& p_matrix);
	Matrix3x3 operator*(const Matrix3x3& p_matrix) const;
	void operator+=(const Matrix3x3& p_matrix);
	Matrix3x3 operator+(const Matrix3x3& p_matrix) const;
	void operator-=(const Matrix3x3& p_matrix);
	Matrix3x3 operator-(const Matrix3x3& p_matrix) const;
	void operator*=(num_fd p_val);
	Matrix3x3 operator*(num_fd p_val) const;

	int GetOrthogonalIndex() const;
	void SetOrthogonalIndex(int p_index);

	void SetDiagonal(const Vector3& p_diag);

	bool IsOrthogonal() const;
	bool IsDiagonal() const;
	bool IsRotation() const;

	Matrix3x3 Slerp(const Matrix3x3& p_to, const num_fd& p_weight) const;
	Matrix3x3 Lerp(const Matrix3x3& p_to, const num_fd& p_weight) const;


	void Set(num_fd xx, num_fd xy, num_fd xz, num_fd yx, num_fd yy, num_fd yz, num_fd zx, num_fd zy, num_fd zz) {
		elements[0][0] = xx;
		elements[0][1] = xy;
		elements[0][2] = xz;
		elements[1][0] = yx;
		elements[1][1] = yy;
		elements[1][2] = yz;
		elements[2][0] = zx;
		elements[2][1] = zy;
		elements[2][2] = zz;
	}
	void Set(const Vector3& p_x, const Vector3& p_y, const Vector3& p_z) {
		SetAxis(0, p_x);
		SetAxis(1, p_y);
		SetAxis(2, p_z);
	}
	Vector3 GetColumn(int i) const {
		return Vector3(elements[0][i], elements[1][i], elements[2][i]);
	}

	Vector3 GetRow(int i) const {
		return Vector3(elements[i][0], elements[i][1], elements[i][2]);
	}
	Vector3 GetMainDiagonal() const {
		return Vector3(elements[0][0], elements[1][1], elements[2][2]);
	}

	void SetRow(int i, const Vector3& p_row) {
		elements[i][0] = p_row.x;
		elements[i][1] = p_row.y;
		elements[i][2] = p_row.z;
	}

	void SetZero() {
		elements[0].Zero();
		elements[1].Zero();
		elements[2].Zero();
	}

	Matrix3x3 TransposeXform(const Matrix3x3& m) const {
		return Matrix3x3(
			elements[0].x * m[0].x + elements[1].x * m[1].x + elements[2].x * m[2].x,
			elements[0].x * m[0].y + elements[1].x * m[1].y + elements[2].x * m[2].y,
			elements[0].x * m[0].z + elements[1].x * m[1].z + elements[2].x * m[2].z,
			elements[0].y * m[0].x + elements[1].y * m[1].x + elements[2].y * m[2].x,
			elements[0].y * m[0].y + elements[1].y * m[1].y + elements[2].y * m[2].y,
			elements[0].y * m[0].z + elements[1].y * m[1].z + elements[2].y * m[2].z,
			elements[0].z * m[0].x + elements[1].z * m[1].x + elements[2].z * m[2].x,
			elements[0].z * m[0].y + elements[1].z * m[1].y + elements[2].z * m[2].y,
			elements[0].z * m[0].z + elements[1].z * m[1].z + elements[2].z * m[2].z);
	}
	Matrix3x3(num_fd xx, num_fd xy, num_fd xz, num_fd yx, num_fd yy, num_fd yz, num_fd zx, num_fd zy, num_fd zz) {
		Set(xx, xy, xz, yx, yy, yz, zx, zy, zz);
	}

	void Orthonormalize();
	Matrix3x3 Orthonormalized() const;

	bool IsSymmetric() const;
	Matrix3x3 Diagonalize();

	// The following normal xform functions are correct for non-uniform scales.
	// Use these two functions in combination to xform a series of normals.
	// First use get_normal_xform_basis() to precalculate the inverse transpose.
	// Then apply xform_normal_fast() multiple times using the inverse transpose basis.
	Matrix3x3 GetNormalXFormMatrix3x3() const { return Inverse().Transposed(); }

	// N.B. This only does a normal transform if the basis used is the inverse transpose!
	// Otherwise use xform_normal().
	Vector3 XFormNormalFast(const Vector3& p_vector) const { return XForm(p_vector).Normalized(); }

	// This function does the above but for a single normal vector. It is considerably slower, so should usually
	// only be used in cases of single normals, or when the basis changes each time.
	Vector3 XFormNormal(const Vector3& p_vector) const { return GetNormalXFormMatrix3x3().XFormNormalFast(p_vector); }

	operator Quaternion() const { return GetQuat(); }

};

#endif // !ST_MATRIX3X3_H

