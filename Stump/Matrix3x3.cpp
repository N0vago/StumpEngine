#include "Matrix3x3.h"


Matrix3x3::Matrix3x3(const Quaternion& p_quat) { SetQuat(p_quat); }

Matrix3x3::Matrix3x3(const Quaternion& p_quat, const Vector3& p_scale) { SetQuatScale(p_quat, p_scale); }

Matrix3x3::Matrix3x3(const Vector3& p_euler) { SetEuler(p_euler); }

Matrix3x3::Matrix3x3(const Vector3& p_euler, const Vector3& p_scale) { SetEulerScale(p_euler, p_scale); }

Matrix3x3::Matrix3x3(const Vector3& p_axis, num_fd p_angle) { SetAxisAngle(p_axis, p_angle); }

Matrix3x3::Matrix3x3(const Vector3& p_axis, num_fd p_angle, const Vector3& p_scale) { SetAxisAngleScale(p_axis, p_angle, p_scale); }

Matrix3x3::Matrix3x3(const Vector3& row0, const Vector3& row1, const Vector3& row2) {
	elements[0] = row0;
	elements[1] = row1;
	elements[2] = row2;
}

Matrix3x3::Matrix3x3() {}

#define cofac(row1, col1, row2, col2) \
	(elements[row1][col1] * (elements[row2][col2] - elements[row1][col2] * elements[row2][col1]))

void Matrix3x3::FromZ(const Vector3& p_z) {
	if (Math::Abs(p_z.z) > (num_fd)Math::SQRT12) {
		num_fd a = p_z[1] * p_z[1] + p_z[2] * p_z[2];
		num_fd k = 1 / Math::Sqrt(a);
		elements[0] = Vector3(0, -p_z[2] * k, p_z[1] * k);
		elements[1] = Vector3(a * k, -p_z[0] * elements[0][2], p_z[0] * elements[0][1]);
	}
	else {
		num_fd a = p_z.x * p_z.x + p_z.y * p_z.y;
		num_fd k = 1 / Math::Sqrt(a);
		elements[0] = Vector3(-p_z.y * k, p_z.x * k, 0);
		elements[1] = Vector3(-p_z.z * elements[0].y, p_z.z * elements[0].x, a * k);
	}
	elements[2] = p_z;

}

//Нахождение обратной матрицы, подробности по методу ниже: https://en.wikipedia.org/wiki/Invertible_matrix#Analytic_solution
void Matrix3x3::Invert() {
	num_fd co[3] = {
		cofac(1, 1, 2, 2), cofac(1, 2, 2, 0), cofac(1, 0, 2, 1)
	};
	num_fd det = elements[0][0] * co[0] +
		elements[0][1] * co[1] +
		elements[0][2] * co[2];
	//Posible det 0

	num_fd s = 1 / det;

	Set(co[0] * s, cofac(0, 2, 2, 1) * s, cofac(0, 1, 1, 2) * s,
		co[1] * s, cofac(0, 0, 2, 2) * s, cofac(0, 2, 1, 0) * s,
		co[2] * s, cofac(0, 1, 2, 0) * s, cofac(0, 0, 1, 1) * s);
}

Matrix3x3 Matrix3x3::Inverse() const {
	Matrix3x3 result = *this;
	result.Invert();
	return result;
}

//Делает вектора матрицы ортонормированными, то есть взаимно перпендикулярными и нормализованными
void Matrix3x3::Orthonormalize() {
	Vector3 x = GetAxis(0);
	Vector3 y = GetAxis(1);
	Vector3 z = GetAxis(2);

	x.Normalize();
	y = (y - x * (x.Dot(y)));
	y.Normalize();
	z = (z - x * (x.Dot(z)) - y * (y.Dot(z)));
	z.Normalize();

	SetAxis(0, x);
	SetAxis(1, y);
	SetAxis(2, z);
}

Matrix3x3 Matrix3x3::Orthonormalized() const {
	Matrix3x3 result = *this;
	result.Orthonormalize();
	return result;
}

bool Matrix3x3::IsOrthogonal() const {
	Matrix3x3 identity;
	Matrix3x3 m = (*this) * Transposed();

	return m.IsEqualApprox(identity);
}

bool Matrix3x3::IsDiagonal() const {
	return (
		Math::IsZeroApprox(elements[0][1]) && Math::IsZeroApprox(elements[0][2]) &&
		Math::IsZeroApprox(elements[1][0]) && Math::IsZeroApprox(elements[1][2]) &&
		Math::IsZeroApprox(elements[2][0]) && Math::IsZeroApprox(elements[2][1]));
}

bool Matrix3x3::IsRotation() const {
	return Math::IsEqualApprox(Determinant(), 1, (num_fd)Math::UNIT_EPSILON) && IsOrthogonal();
}

bool Matrix3x3::IsSymmetric() const {
	if (!Math::IsEqualApproxRatio(elements[0][1], elements[1][0], (num_fd)Math::UNIT_EPSILON)) {
		return false;
	}
	if (!Math::IsEqualApproxRatio(elements[0][2], elements[2][0], (num_fd)Math::UNIT_EPSILON)) {
		return false;
	}
	if (!Math::IsEqualApproxRatio(elements[1][2], elements[2][1], (num_fd)Math::UNIT_EPSILON)) {
		return false;
	}

	return true;
}

Matrix3x3 Matrix3x3::Diagonalize() {
	//Для более детальной информации https://en.wikipedia.org/wiki/Jacobi_eigenvalue_algorithm
	const int ite_max = 1024;

	num_fd off_matrix_norm_2 = elements[0][1] * elements[0][1] + elements[0][2] * elements[0][2] + elements[1][2] * elements[1][2];

	int ite = 0;
	Matrix3x3 acc_rot;
	while (off_matrix_norm_2 > (num_fd)Math::EPSILON && ite++ < ite_max) {
		num_fd el01_2 = elements[0][1] * elements[0][1];
		num_fd el02_2 = elements[0][2] * elements[0][2];
		num_fd el12_2 = elements[1][2] * elements[1][2];
		// Find the pivot element
		int i, j;
		if (el01_2 > el02_2) {
			if (el12_2 > el01_2) {
				i = 1;
				j = 2;
			}
			else {
				i = 0;
				j = 1;
			}
		}
		else {
			if (el12_2 > el02_2) {
				i = 1;
				j = 2;
			}
			else {
				i = 0;
				j = 2;
			}
		}

		// Compute the rotation angle
		num_fd angle;
		if (Math::IsEqualApprox(elements[j][j], elements[i][i])) {
			angle = Math::PI / 4;
		}
		else {
			angle = 0.5f * Math::Atan(2 * elements[i][j] / (elements[j][j] - elements[i][i]));
		}

		// Compute the rotation matrix
		Matrix3x3 rot;
		rot.elements[i][i] = rot.elements[j][j] = Math::Cos(angle);
		rot.elements[i][j] = -(rot.elements[j][i] = Math::Sin(angle));

		// Update the off matrix norm
		off_matrix_norm_2 -= elements[i][j] * elements[i][j];

		// Apply the rotation
		*this = rot * *this * rot.Transposed();
		acc_rot = rot * acc_rot;
	}

	return acc_rot;
}

void Matrix3x3::Transpose(){
	SWAP(elements[0][1], elements[1][0]);
	SWAP(elements[0][2], elements[2][0]);
	SWAP(elements[1][2], elements[2][1]);
}

Matrix3x3 Matrix3x3::Transposed() const {
	Matrix3x3 tr = *this;
	tr.Transpose();
	return tr;
}
 
void Matrix3x3::Scale(const Vector3& p_scale) {
	elements[0][0] *= p_scale.x;
	elements[0][1] *= p_scale.x;
	elements[0][2] *= p_scale.x;
	elements[1][0] *= p_scale.y;
	elements[1][1] *= p_scale.y;
	elements[1][2] *= p_scale.y;
	elements[2][0] *= p_scale.z;
	elements[2][1] *= p_scale.z;
	elements[2][2] *= p_scale.z;
}

Matrix3x3 Matrix3x3::Scaled(const Vector3& p_scale) const {
	Matrix3x3 result = *this;
	result.Scale(p_scale);
	return result;
}

void Matrix3x3::ScaleLocal(const Vector3& p_scale) {
	*this = ScaledLocal(p_scale);
}

Matrix3x3 Matrix3x3::ScaledLocal(const Vector3& p_scale) const {
	Matrix3x3 mt;
	mt.SetDiagonal(p_scale);
	return (*this) * mt;
}

Vector3 Matrix3x3::GetScaleAbs() const {
	return Vector3(
		Vector3(elements[0][0], elements[1][0], elements[2][0]).Length(),
		Vector3(elements[0][1], elements[1][1], elements[2][1]).Length(),
		Vector3(elements[0][2], elements[1][2], elements[2][2]).Length());
}

Vector3 Matrix3x3::GetScaleLocal() const {
	num_fd det_sign = SGN(Determinant());

	return Vector3(elements[0].Length(), elements[1].Length(), elements[2].Length()) * det_sign;
}

Vector3 Matrix3x3::GetScale() const {
	// FIXME: We are assuming M = R.S (R is rotation and S is scaling), and use polar decomposition to extract R and S.
	// A polar decomposition is M = O.P, where O is an orthogonal matrix (meaning rotation and reflection) and
	// P is a positive semi-definite matrix (meaning it contains absolute values of scaling along its diagonal).
	//
	// Despite being different from what we want to achieve, we can nevertheless make use of polar decomposition
	// here as follows. We can split O into a rotation and a reflection as O = R.Q, and obtain M = R.S where
	// we defined S = Q.P. Now, R is a proper rotation matrix and S is a (signed) scaling matrix,
	// which can involve negative scalings. However, there is a catch: unlike the polar decomposition of M = O.P,
	// the decomposition of O into a rotation and reflection matrix as O = R.Q is not unique.
	// Therefore, we are going to do this decomposition by sticking to a particular convention.
	// This may lead to confusion for some users though.
	//
	// The convention we use here is to absorb the sign flip into the scaling matrix.
	// The same convention is also used in other similar functions such as get_rotation_axis_angle, get_rotation, ...
	//
	// A proper way to get rid of this issue would be to store the scaling values (or at least their signs)
	// as a part of Matrix3x3. However, if we go that path, we need to disable direct (write) access to the
	// matrix elements.
	//
	// The rotation part of this decomposition is returned by get_rotation* functions.
	num_fd det_sign = SGN(Determinant());

	return GetScaleAbs() * det_sign;
}


//Декомпозиция матрицы на матрицу поворота и масштабирования
//Матрица поворота возвращается через аргумент rotref
//А масштабирование возвращается в виде вектора
Vector3 Matrix3x3::RotrefPosscaleDecomposition(Matrix3x3& rotref) const {
	Vector3 scale = GetScale();
	Matrix3x3 inv_scale = Matrix3x3().Scaled(scale.Inverse());
	rotref = (*this) * inv_scale;

	return scale.Abs();
}

Matrix3x3 Matrix3x3::Rotated(const Vector3& p_axis, num_fd p_angle) const {
	return Matrix3x3(p_axis, p_angle) * (*this);
}

void Matrix3x3::Rotate(const Vector3& p_axis, num_fd p_angle) {
	*this = Rotated(p_axis, p_angle);
}

Matrix3x3 Matrix3x3::RotatedLocal(const Vector3& p_axis, num_fd p_angle) const {
	return (*this) * Matrix3x3(p_axis, p_angle);
}

void Matrix3x3::RotateLocal(const Vector3& p_axis, num_fd p_angle) {
	*this = RotatedLocal(p_axis, p_angle);
}

void Matrix3x3::Rotate(const Vector3& p_euler) {
	*this = Rotated(p_euler);
}

Matrix3x3 Matrix3x3::Rotated(const Vector3& p_euler) const {
	return Matrix3x3(p_euler) * (*this);
}

void Matrix3x3::Rotate(const Quaternion& p_quat) {
	*this = Rotated(p_quat);
}
Matrix3x3 Matrix3x3::Rotated(const Quaternion& p_quat) const {
	return Matrix3x3(p_quat) * (*this);
}

//Делает матрицу ортонормированной, что имеет смысл так как нам нужна только ротация без масштабирования
//Если детерминант отрицательный, то инвертируем все оси чтобы получить правую систему координат
//Возвращает эйлеровы углы ротации
Vector3 Matrix3x3::GetRotationEuler() const {
	Matrix3x3 m = Orthonormalized();
	num_fd det = m.Determinant();
	if (det < 0) {
		m.Scale(Vector3(-1, -1, -1));
	}

	return m.GetEuler();
}


//То же самое что и в случае с углами эйлера, но возвращает кватернион
Quaternion Matrix3x3::GetRotationQuat() const {
	Matrix3x3 m = Orthonormalized();
	num_fd det = m.Determinant();
	if (det < 0) {
		m.Scale(Vector3(-1, -1, -1));
	}
	return m.GetQuat();
}

void Matrix3x3::GetRotationAxisAngle(Vector3& p_axis, num_fd& p_angle) const {
	Matrix3x3 m = Orthonormalized();
	num_fd det = m.Determinant();
	if (det < 0) {
		m.Scale(Vector3(-1, -1, -1));
	}
	m.GetAxisAngle(p_axis, p_angle);
}

void Matrix3x3::GetRotationAxisAngleLocal(Vector3& p_axis, num_fd& p_angle) const {
	Matrix3x3 m = Transposed();
	m.Orthonormalize();
	num_fd det = m.Determinant();
	if (det < 0) {
		m.Scale(Vector3(-1, -1, -1));
	}
	m.GetAxisAngle(p_axis, p_angle);
	p_angle = -p_angle;
}

Vector3 Matrix3x3::GetEulerXYZ() const {
	// Углы эйлера для XYZ ординат
	// Подробнее https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
	//
	// rot =  cy*cz          -cy*sz           sy
	//        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
	//       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

	Vector3 euler;
	num_fd sy = elements[0][2];
	if (sy < (1 - (num_fd)Math::EPSILON)) {
		if (sy > -(1 - (num_fd)Math::EPSILON)) {
			
			if (elements[1][0] == 0 && elements[0][1] == 0 && elements[1][2] == 0 && elements[2][1] == 0 && elements[1][1] == 1) {
				
				euler.x = 0;
				euler.y = Math::Atan2(elements[0][2], elements[0][0]);
				euler.z = 0;
			}
			else {
				euler.x = Math::Atan2(-elements[1][2], elements[2][2]);
				euler.y = Math::Asin(sy);
				euler.z = Math::Atan2(-elements[0][1], elements[0][0]);
			}
		}
		else {
			euler.x = Math::Atan2(elements[2][1], elements[1][1]);
			euler.y = -Math::PI / 2.0;
			euler.z = 0.0;
		}
	}
	else {
		euler.x = Math::Atan2(elements[2][1], elements[1][1]);
		euler.y = Math::PI / 2.0;
		euler.z = 0.0;
	}
	return euler;
}

//Установка матрицы поворота для текущей матрицы
//Беруться матрицы поворота для ординат XYZ https://en.wikipedia.org/wiki/Rotation_matrix#Basic_3D_rotations
//И перемножаются в порядке Z->Y->X
void Matrix3x3::SetEulerXYZ(const Vector3& p_euler) {
	num_fd c, s;

	c = Math::Cos(p_euler.x);
	s = Math::Sin(p_euler.x);
	Matrix3x3 xmat(1, 0, 0, 0, c, -s, 0, s, c);

	c = Math::Cos(p_euler.y);
	s = Math::Sin(p_euler.y);
	Matrix3x3 ymat(c, 0, s, 0, 1, 0, -s, 0, c);

	c = Math::Cos(p_euler.z);
	s = Math::Sin(p_euler.z);
	Matrix3x3 zmat(c, -s, 0, s, c, 0, 0, 0, 1);

	*this = xmat * (ymat * zmat);
}

Vector3 Matrix3x3::GetEulerXZY() const {
	// Углы эйлера для XZY ординат
	// Подробнее https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
	//
	// rot =  cz*cy             -sz             cz*sy
	//        sx*sy+cx*cy*sz    cx*cz           cx*sz*sy-cy*sx
	//        cy*sx*sz          cz*sx           cx*cy+sx*sz*sy

	Vector3 euler;
	num_fd sz = elements[0][1];
	if (sz < (1 - (num_fd)Math::EPSILON)) {
		if (sz > -(1 - (num_fd)Math::EPSILON)) {
			euler.x = Math::Atan2(elements[2][1], elements[1][1]);
			euler.y = Math::Atan2(elements[0][2], elements[0][0]);
			euler.z = Math::Asin(-sz);
		}
		else {
			euler.x = -Math::Atan2(elements[1][2], elements[2][2]);
			euler.y = 0.0;
			euler.z = Math::PI / 2.0;
		}
	}
	else {
		euler.x = -Math::Atan2(elements[1][2], elements[2][2]);
		euler.y = 0.0;
		euler.z = -Math::PI / 2.0;
	}
	return euler;
}

void Matrix3x3::SetEulerXZY(const Vector3& p_euler) {
	num_fd c, s;

	c = Math::Cos(p_euler.x);
	s = Math::Sin(p_euler.x);
	Matrix3x3 xmat(1, 0, 0, 0, c, -s, 0, s, c);

	c = Math::Cos(p_euler.y);
	s = Math::Sin(p_euler.y);
	Matrix3x3 ymat(c, 0, s, 0, 1, 0, -s, 0, c);

	c = Math::Cos(p_euler.z);
	s = Math::Sin(p_euler.z);
	Matrix3x3 zmat(c, -s, 0, s, c, 0, 0, 0, 1);

	*this = xmat * zmat * ymat;
}

Vector3 Matrix3x3::GetEulerYZX() const {
	// Углы эйлера для YZX ординат
	// Подробнее https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
	//
	// rot =  cy*cz             sy*sx-cy*cx*sz     cx*sy+cy*sz*sx
	//        sz                cz*cx              -cz*sx
	//        -cz*sy            cy*sx+cx*sy*sz     cy*cx-sy*sz*sx

	Vector3 euler;
	num_fd sz = elements[1][0];
	if (sz < (1 - (num_fd)Math::EPSILON)) {
		if (sz > -(1 - (num_fd)Math::EPSILON)) {
			euler.x = Math::Atan2(-elements[1][2], elements[1][1]);
			euler.y = Math::Atan2(-elements[2][0], elements[0][0]);
			euler.z = Math::Asin(sz);
		}
		else {
			euler.x = Math::Atan2(elements[2][1], elements[2][2]);
			euler.y = 0.0;
			euler.z = -Math::PI / 2.0;
		}
	}
	else {
		euler.x = Math::Atan2(elements[2][1], elements[2][2]);
		euler.y = 0.0;
		euler.z = Math::PI / 2.0;
	}
	return euler;
}

void Matrix3x3::SetEulerYZX(const Vector3& p_euler) {
	num_fd c, s;

	c = Math::Cos(p_euler.x);
	s = Math::Sin(p_euler.x);
	Matrix3x3 xmat(1, 0, 0, 0, c, -s, 0, s, c);

	c = Math::Cos(p_euler.y);
	s = Math::Sin(p_euler.y);
	Matrix3x3 ymat(c, 0, s, 0, 1, 0, -s, 0, c);

	c = Math::Cos(p_euler.z);
	s = Math::Sin(p_euler.z);
	Matrix3x3 zmat(c, -s, 0, s, c, 0, 0, 0, 1);

	*this = ymat * zmat * xmat;
}

Vector3 Matrix3x3::GetEulerYXZ() const {
	// Углы эйлера для YXZ ординат
	// Подробнее https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
	//
	// rot =  cy*cz+sy*sx*sz    cz*sy*sx-cy*sz        cx*sy
	//        cx*sz             cx*cz                 -sx
	//        cy*sx*sz-cz*sy    cy*cz*sx+sy*sz        cy*cx

	Vector3 euler;

	num_fd m12 = elements[1][2];

	if (m12 < (1 - (num_fd)Math::EPSILON)) {
		if (m12 > -(1 - (num_fd)Math::EPSILON)) {
			
			if (elements[1][0] == 0 && elements[0][1] == 0 && elements[0][2] == 0 && elements[2][0] == 0 && elements[0][0] == 1) {
				
				euler.x = Math::Atan2(-m12, elements[1][1]);
				euler.y = 0;
				euler.z = 0;
			}
			else {
				euler.x = Math::Asin(-m12);
				euler.y = Math::Atan2(elements[0][2], elements[2][2]);
				euler.z = Math::Atan2(elements[1][0], elements[1][1]);
			}
		}
		else {
			euler.x = Math::PI * 0.5;
			euler.y = Math::Atan2(elements[0][1], elements[0][0]);
			euler.z = 0;
		}
	}
	else {
		euler.x = -Math::PI * 0.5;
		euler.y = -Math::Atan2(elements[0][1], elements[0][0]);
		euler.z = 0;
	}

	return euler;
}

void Matrix3x3::SetEulerYXZ(const Vector3& p_euler) {
	num_fd c, s;

	c = Math::Cos(p_euler.x);
	s = Math::Sin(p_euler.x);
	Matrix3x3 xmat(1, 0, 0, 0, c, -s, 0, s, c);

	c = Math::Cos(p_euler.y);
	s = Math::Sin(p_euler.y);
	Matrix3x3 ymat(c, 0, s, 0, 1, 0, -s, 0, c);

	c = Math::Cos(p_euler.z);
	s = Math::Sin(p_euler.z);
	Matrix3x3 zmat(c, -s, 0, s, c, 0, 0, 0, 1);

	*this = ymat * xmat * zmat;
}

Vector3 Matrix3x3::GetEulerZXY() const {
	// Углы эйлера для ZXY ординат
	// Подробнее https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
	//
	// rot =  cz*cy-sz*sx*sy    -cx*sz                cz*sy+cy*sz*sx
	//        cy*sz+cz*sx*sy    cz*cx                 sz*sy-cz*cy*sx
	//        -cx*sy            sx                    cx*cy
	Vector3 euler;
	num_fd sx = elements[2][1];
	if (sx < (1 - (num_fd)Math::EPSILON)) {
		if (sx > -(1 - (num_fd)Math::EPSILON)) {
			euler.x = Math::Asin(sx);
			euler.y = Math::Atan2(-elements[2][0], elements[2][2]);
			euler.z = Math::Atan2(-elements[0][1], elements[1][1]);
		}
		else {
			euler.x = -Math::PI / 2.0;
			euler.y = Math::Atan2(elements[0][2], elements[0][0]);
			euler.z = 0;
		}
	}
	else {
		euler.x = Math::PI / 2.0;
		euler.y = Math::Atan2(elements[0][2], elements[0][0]);
		euler.z = 0;
	}
	return euler;
}

void Matrix3x3::SetEulerZXY(const Vector3& p_euler) {
	num_fd c, s;

	c = Math::Cos(p_euler.x);
	s = Math::Sin(p_euler.x);
	Matrix3x3 xmat(1, 0, 0, 0, c, -s, 0, s, c);

	c = Math::Cos(p_euler.y);
	s = Math::Sin(p_euler.y);
	Matrix3x3 ymat(c, 0, s, 0, 1, 0, -s, 0, c);

	c = Math::Cos(p_euler.z);
	s = Math::Sin(p_euler.z);
	Matrix3x3 zmat(c, -s, 0, s, c, 0, 0, 0, 1);

	*this = zmat * xmat * ymat;
}

Vector3 Matrix3x3::GetEulerZYX() const {
	// Углы эйлера для ZYX ординат
	// Подробнее https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
	//
	// rot =  cz*cy             cz*sy*sx-cx*sz        sz*sx+cz*cx*cy
	//        cy*sz             cz*cx+sz*sy*sx        cx*sz*sy-cz*sx
	//        -sy               cy*sx                 cy*cx
	Vector3 euler;
	num_fd sy = elements[2][0];
	if (sy < (1 - (num_fd)Math::EPSILON)) {
		if (sy > -(1 - (num_fd)Math::EPSILON)) {
			euler.x = Math::Atan2(elements[2][1], elements[2][2]);
			euler.y = Math::Asin(-sy);
			euler.z = Math::Atan2(elements[1][0], elements[0][0]);
		}
		else {
			euler.x = 0;
			euler.y = Math::PI / 2.0;
			euler.z = -Math::Atan2(elements[0][1], elements[1][1]);
		}
	}
	else {
		euler.x = 0;
		euler.y = -Math::PI / 2.0;
		euler.z = -Math::Atan2(elements[0][1], elements[1][1]);
	}
	return euler;
}

void Matrix3x3::SetEulerZYX(const Vector3& p_euler) {
	num_fd c, s;

	c = Math::Cos(p_euler.x);
	s = Math::Sin(p_euler.x);
	Matrix3x3 xmat(1, 0, 0, 0, c, -s, 0, s, c);

	c = Math::Cos(p_euler.y);
	s = Math::Sin(p_euler.y);
	Matrix3x3 ymat(c, 0, s, 0, 1, 0, -s, 0, c);

	c = Math::Cos(p_euler.z);
	s = Math::Sin(p_euler.z);
	Matrix3x3 zmat(c, -s, 0, s, c, 0, 0, 0, 1);

	*this = zmat * ymat * xmat;
}

bool Matrix3x3::IsEqualApprox(const Matrix3x3& p_matrix) const {
	return elements[0].IsEqualApprox(p_matrix.elements[0]) && elements[1].IsEqualApprox(p_matrix.elements[1]) && elements[2].IsEqualApprox(p_matrix.elements[2]);
}

bool Matrix3x3::IsEqualApproxRatio(const Matrix3x3& p_a, const Matrix3x3& p_b, num_fd p_epsilon) const {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (!Math::IsEqualApproxRatio(p_a.elements[i][j], p_b.elements[i][j], p_epsilon)) {
				return false;
			}
		}
	}
}

bool Matrix3x3::operator==(const Matrix3x3& p_matrix) const {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (elements[i][j] != p_matrix.elements[i][j]) {
				return false;
			}
		}
	}

	return true;
}

bool Matrix3x3::operator!=(const Matrix3x3& p_matrix) const {
	return (!(*this == p_matrix));
}
Quaternion Matrix3x3::GetQuat() const {
	Matrix3x3 m = *this;
	num_fd trace = m.elements[0][0] + m.elements[1][1] + m.elements[2][2];
	num_fd temp[4];

	if (trace > 0) {
		num_fd s = Math::Sqrt(trace + 1);
		temp[3] = (s * 0.5f);
		s = 0.5f / s;

		temp[0] = ((m.elements[2][1] - m.elements[1][2]) * s);
		temp[1] = ((m.elements[0][2] - m.elements[2][0]) * s);
		temp[2] = ((m.elements[1][0] - m.elements[0][1]) * s);
	}
	else {
		int i = m.elements[0][0] < m.elements[1][1]
			? (m.elements[1][1] < m.elements[2][2] ? 2 : 1)
			: (m.elements[0][0] < m.elements[2][2] ? 2 : 0);
		int j = (i + 1) % 3;
		int k = (i + 2) % 3;

		num_fd s = Math::Sqrt(m.elements[i][i] - m.elements[j][j] - m.elements[k][k] + 1);
		temp[i] = s * 0.5f;
		s = 0.5f / s;

		temp[3] = (m.elements[k][j] - m.elements[j][k]) * s;
		temp[j] = (m.elements[j][i] + m.elements[i][j]) * s;
		temp[k] = (m.elements[k][i] + m.elements[i][k]) * s;
	}

	return Quaternion(temp[0], temp[1], temp[2], temp[3]);
}

static const Matrix3x3 _ortho_bases[24] = {
	Matrix3x3(1, 0, 0, 0, 1, 0, 0, 0, 1),
	Matrix3x3(0, -1, 0, 1, 0, 0, 0, 0, 1),
	Matrix3x3(-1, 0, 0, 0, -1, 0, 0, 0, 1),
	Matrix3x3(0, 1, 0, -1, 0, 0, 0, 0, 1),
	Matrix3x3(1, 0, 0, 0, 0, -1, 0, 1, 0),
	Matrix3x3(0, 0, 1, 1, 0, 0, 0, 1, 0),
	Matrix3x3(-1, 0, 0, 0, 0, 1, 0, 1, 0),
	Matrix3x3(0, 0, -1, -1, 0, 0, 0, 1, 0),
	Matrix3x3(1, 0, 0, 0, -1, 0, 0, 0, -1),
	Matrix3x3(0, 1, 0, 1, 0, 0, 0, 0, -1),
	Matrix3x3(-1, 0, 0, 0, 1, 0, 0, 0, -1),
	Matrix3x3(0, -1, 0, -1, 0, 0, 0, 0, -1),
	Matrix3x3(1, 0, 0, 0, 0, 1, 0, -1, 0),
	Matrix3x3(0, 0, -1, 1, 0, 0, 0, -1, 0),
	Matrix3x3(-1, 0, 0, 0, 0, -1, 0, -1, 0),
	Matrix3x3(0, 0, 1, -1, 0, 0, 0, -1, 0),
	Matrix3x3(0, 0, 1, 0, 1, 0, -1, 0, 0),
	Matrix3x3(0, -1, 0, 0, 0, 1, -1, 0, 0),
	Matrix3x3(0, 0, -1, 0, -1, 0, -1, 0, 0),
	Matrix3x3(0, 1, 0, 0, 0, -1, -1, 0, 0),
	Matrix3x3(0, 0, 1, 0, -1, 0, 1, 0, 0),
	Matrix3x3(0, 1, 0, 0, 0, 1, 1, 0, 0),
	Matrix3x3(0, 0, -1, 0, 1, 0, 1, 0, 0),
	Matrix3x3(0, -1, 0, 0, 0, -1, 1, 0, 0)
};

int Matrix3x3::GetOrthogonalIndex() const {
	Matrix3x3 orth = *this;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			num_fd v = orth[i][j];
			if (v > 0.5f) {
				v = 1;
			}
			else if (v < -0.5f) {
				v = -1;
			}
			else {
				v = 0;
			}

			orth[i][j] = v;
		}
	}

	for (int i = 0; i < 24; i++) {
		if (_ortho_bases[i] == orth) {
			return i;
		}
	}

	return 0;
}
void Matrix3x3::GetAxisAngle(Vector3& r_axis, num_fd& r_angle) const {

	// https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToAngle/index.htm
	num_fd x, y, z;
	if (Math::IsZeroApprox(elements[0][1] - elements[1][0]) && Math::IsZeroApprox(elements[0][2] - elements[2][0]) && Math::IsZeroApprox(elements[1][2] - elements[2][1])) {
		// Singularity found.
		// First check for identity matrix which must have +1 for all terms in leading diagonal and zero in other terms.
		if (IsDiagonal() && (Math::Abs(elements[0][0] + elements[1][1] + elements[2][2] - 3) < 3 * Math::EPSILON)) {
			// This singularity is identity matrix so angle = 0.
			r_axis = Vector3(0, 1, 0);
			r_angle = 0;
			return;
		}
		// Otherwise this singularity is angle = 180.
		num_fd xx = (elements[0][0] + 1) / 2;
		num_fd yy = (elements[1][1] + 1) / 2;
		num_fd zz = (elements[2][2] + 1) / 2;
		num_fd xy = (elements[0][1] + elements[1][0]) / 4;
		num_fd xz = (elements[0][2] + elements[2][0]) / 4;
		num_fd yz = (elements[1][2] + elements[2][1]) / 4;

		if ((xx > yy) && (xx > zz)) { // elements[0][0] is the largest diagonal term.
			if (xx < Math::EPSILON) {
				x = 0;
				y = Math::SQRT12;
				z = Math::SQRT12;
			}
			else {
				x = Math::Sqrt(xx);
				y = xy / x;
				z = xz / x;
			}
		}
		else if (yy > zz) { // elements[1][1] is the largest diagonal term.
			if (yy < Math::EPSILON) {
				x = Math::SQRT12;
				y = 0;
				z = Math::SQRT12;
			}
			else {
				y = Math::Sqrt(yy);
				x = xy / y;
				z = yz / y;
			}
		}
		else { // elements[2][2] is the largest diagonal term so base result on this.
			if (zz < Math::EPSILON) {
				x = Math::SQRT12;
				y = Math::SQRT12;
				z = 0;
			}
			else {
				z = Math::Sqrt(zz);
				x = xz / z;
				y = yz / z;
			}
		}
		r_axis = Vector3(x, y, z);
		r_angle = Math::PI;
		return;
	}
	// As we have reached here there are no singularities so we can handle normally.
	double s = Math::Sqrt((elements[2][1] - elements[1][2]) * (elements[2][1] - elements[1][2]) + (elements[0][2] - elements[2][0]) * (elements[0][2] - elements[2][0]) + (elements[1][0] - elements[0][1]) * (elements[1][0] - elements[0][1])); // Used to normalise.

	if (Math::Abs(s) < Math::EPSILON) {
		// Prevent divide by zero, should not happen if matrix is orthogonal and should be caught by singularity test above.
		s = 1;
	}

	x = (elements[2][1] - elements[1][2]) / s;
	y = (elements[0][2] - elements[2][0]) / s;
	z = (elements[1][0] - elements[0][1]) / s;

	r_axis = Vector3(x, y, z);
	// acos does clamping.
	r_angle = Math::Acos((elements[0][0] + elements[1][1] + elements[2][2] - 1) / 2);
}

void Matrix3x3::SetQuat(const Quaternion& p_quat) {
	num_fd d = p_quat.LengthSquared();
	num_fd s = 2 / d;
	num_fd xs = p_quat.x * s, ys = p_quat.y * s, zs = p_quat.z * s;
	num_fd wx = p_quat.w * xs, wy = p_quat.w * ys, wz = p_quat.w * zs;
	num_fd xx = p_quat.x * xs, xy = p_quat.x * ys, xz = p_quat.x * zs;
	num_fd yy = p_quat.y * ys, yz = p_quat.y * zs, zz = p_quat.z * zs;
	Set(1 - (yy + zz), xy - wz, xz + wy,
		xy + wz, 1 - (xx + zz), yz - wx,
		xz - wy, yz + wx, 1 - (xx + yy));
}

void Matrix3x3::SetAxisAngle(const Vector3& p_axis, num_fd p_angle) {
	// Rotation matrix from axis and angle, see https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_angle
	Vector3 axis_sq(p_axis.x * p_axis.x, p_axis.y * p_axis.y, p_axis.z * p_axis.z);
	num_fd cosine = Math::Cos(p_angle);
	num_fd sine = Math::Sin(p_angle);
	num_fd t = 1 - cosine;


	elements[0][0] = axis_sq.x * t + cosine;
	elements[1][1] = axis_sq.y * t + cosine;
	elements[2][2] = axis_sq.z * t + cosine;


	num_fd xyzt = p_axis.x * p_axis.y * t;
	num_fd zyxs = p_axis.z * sine;
	elements[0][1] = xyzt - zyxs;
	elements[1][0] = xyzt + zyxs;

	xyzt = p_axis.x * p_axis.z * t;
	zyxs = p_axis.y * sine;
	elements[0][2] = xyzt + zyxs;
	elements[2][0] = xyzt - zyxs;

	xyzt = p_axis.y * p_axis.z * t;
	zyxs = p_axis.x * sine;
	elements[1][2] = xyzt - zyxs;
	elements[2][1] = xyzt + zyxs;
}

void Matrix3x3::SetAxisAngleScale(const Vector3& p_axis, num_fd p_angle, const Vector3& p_scale) {
	SetDiagonal(p_scale);
	Rotate(p_axis, p_angle);
}

void Matrix3x3::SetEulerScale(const Vector3& p_euler, const Vector3& p_scale) {
	SetDiagonal(p_scale);
	Rotate(p_euler);
}

void Matrix3x3::SetQuatScale(const Quaternion& p_quat, const Vector3& p_scale) {
	SetDiagonal(p_scale);
	Rotate(p_quat);
}

void Matrix3x3::SetOrthogonalIndex(int p_index) {
	*this = _ortho_bases[p_index];
}

void Matrix3x3::SetDiagonal(const Vector3& p_diagonal) {
	elements[0][0] = p_diagonal.x;
	elements[0][1] = 0;
	elements[0][2] = 0;

	elements[1][0] = 0;
	elements[1][1] = p_diagonal.y;
	elements[1][2] = 0;

	elements[2][0] = 0;
	elements[2][1] = 0;
	elements[2][2] = p_diagonal.z;
}

Matrix3x3 Matrix3x3::Slerp(const Matrix3x3& p_to, const num_fd& p_weight) const {
	Quaternion from(*this);
	Quaternion to(p_to);

	Matrix3x3 b(from.Slerp(to, p_weight));
	b.elements[0] *= Math::Lerp(elements[0].Length(), p_to.elements[0].Length(), p_weight);
	b.elements[1] *= Math::Lerp(elements[1].Length(), p_to.elements[1].Length(), p_weight);
	b.elements[2] *= Math::Lerp(elements[2].Length(), p_to.elements[2].Length(), p_weight);

	return b;
}

void Matrix3x3::operator*=(const Matrix3x3& p_matrix) {
	Set(
		p_matrix.Tdotx(elements[0]), p_matrix.Tdoty(elements[0]), p_matrix.Tdotz(elements[0]),
		p_matrix.Tdotx(elements[1]), p_matrix.Tdoty(elements[1]), p_matrix.Tdotz(elements[1]),
		p_matrix.Tdotx(elements[2]), p_matrix.Tdoty(elements[2]), p_matrix.Tdotz(elements[2]));
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& p_matrix) const {
	return Matrix3x3(
		p_matrix.Tdotx(elements[0]), p_matrix.Tdoty(elements[0]), p_matrix.Tdotz(elements[0]),
		p_matrix.Tdotx(elements[1]), p_matrix.Tdoty(elements[1]), p_matrix.Tdotz(elements[1]),
		p_matrix.Tdotx(elements[2]), p_matrix.Tdoty(elements[2]), p_matrix.Tdotz(elements[2]));
}

void Matrix3x3::operator+=(const Matrix3x3& p_matrix) {
	elements[0] += p_matrix.elements[0];
	elements[1] += p_matrix.elements[1];
	elements[2] += p_matrix.elements[2];
}

Matrix3x3 Matrix3x3::operator+(const Matrix3x3& p_matrix) const {
	Matrix3x3 ret(*this);
	ret += p_matrix;
	return ret;
}

void Matrix3x3::operator-=(const Matrix3x3& p_matrix) {
	elements[0] -= p_matrix.elements[0];
	elements[1] -= p_matrix.elements[1];
	elements[2] -= p_matrix.elements[2];
}

Matrix3x3 Matrix3x3::operator-(const Matrix3x3& p_matrix) const {
	Matrix3x3 ret(*this);
	ret -= p_matrix;
	return ret;
}

void Matrix3x3::operator*=(num_fd p_val) {
	elements[0] *= p_val;
	elements[1] *= p_val;
	elements[2] *= p_val;
}

Matrix3x3 Matrix3x3::operator*(num_fd p_val) const {
	Matrix3x3 ret(*this);
	ret *= p_val;
	return ret;
}

Vector3 Matrix3x3::XForm(const Vector3& p_vector) const {
	return Vector3(
		elements[0].Dot(p_vector),
		elements[1].Dot(p_vector),
		elements[2].Dot(p_vector));
}

Vector3 Matrix3x3::XFormInv(const Vector3& p_vector) const {
	return Vector3(
		(elements[0][0] * p_vector.x) + (elements[1][0] * p_vector.y) + (elements[2][0] * p_vector.z),
		(elements[0][1] * p_vector.x) + (elements[1][1] * p_vector.y) + (elements[2][1] * p_vector.z),
		(elements[0][2] * p_vector.x) + (elements[1][2] * p_vector.y) + (elements[2][2] * p_vector.z));
}

num_fd Matrix3x3::Determinant() const {
	return elements[0][0] * (elements[1][1] * elements[2][2] - elements[2][1] * elements[1][2]) -
		elements[1][0] * (elements[0][1] * elements[2][2] - elements[2][1] * elements[0][2]) +
		elements[2][0] * (elements[0][1] * elements[1][2] - elements[1][1] * elements[0][2]);
}

Matrix3x3 Matrix3x3::Lerp(const Matrix3x3& p_to, const num_fd& p_weight) const {
	Matrix3x3 b;
	b.elements[0] = elements[0].LinearInterpolate(p_to.elements[0], p_weight);
	b.elements[1] = elements[1].LinearInterpolate(p_to.elements[1], p_weight);
	b.elements[2] = elements[2].LinearInterpolate(p_to.elements[2], p_weight);

	return b;
}



