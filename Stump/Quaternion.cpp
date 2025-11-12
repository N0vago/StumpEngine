#include "Quaternion.h"


Quaternion Quaternion::Slerp(const Quaternion& p_to, const float& p_weight) const {
	Quaternion to1;
	float omega, cosom, sinom, scale0, scale1;

	cosom = Dot(p_to);

	if (cosom < 0) {
		cosom = -cosom;
		to1.x = -p_to.x;
		to1.y = -p_to.y;
		to1.z = -p_to.z;
		to1.w = -p_to.w;
	}
	else {
		to1.x = p_to.x;
		to1.y = p_to.y;
		to1.z = p_to.z;
		to1.w = p_to.w;
	}

	if ((1 - cosom) > (float)Math::EPSILON) {
		omega = Math::Acos(cosom);
		sinom = Math::Sin(omega);
		scale0 = Math::Sin((1 - p_weight) * omega) / sinom;
		scale1 = Math::Sin(p_weight * omega) / sinom;
	}
	else {
		scale0 = 1 - p_weight;
		scale1 = p_weight;
	}

	return Quaternion(
		scale0 * x + scale1 * to1.x,
		scale0 * y + scale1 * to1.y,
		scale0 * z + scale1 * to1.z,
		scale0 * w + scale1 * to1.w);
}

void Quaternion::SetEulerXYZ(const Vector3& p_euler) {
	num_fd half_a1 = p_euler.x * 0.5f;
	num_fd half_a2 = p_euler.y * 0.5f;
	num_fd half_a3 = p_euler.z * 0.5f;

	num_fd cos_a1 = Math::Cos(half_a1);
	num_fd sin_a1 = Math::Sin(half_a1);
	num_fd cos_a2 = Math::Cos(half_a2);
	num_fd sin_a2 = Math::Sin(half_a2);
	num_fd cos_a3 = Math::Cos(half_a3);
	num_fd sin_a3 = Math::Sin(half_a3);

	Set(sin_a1 * cos_a2 * cos_a3 + sin_a2 * sin_a3 * cos_a1,
		-sin_a1 * sin_a3 * cos_a2 + sin_a2 * cos_a1 * cos_a3,
		sin_a1 * sin_a2 * cos_a3 + sin_a3 * cos_a1 * cos_a2,
		-sin_a1 * sin_a2 * sin_a3 + cos_a1 * cos_a2 * cos_a3);
}
Vector3 Quaternion::GetEulerXYZ() const {
	Matrix3x3 m(*this);
	return m.GetEulerXYZ();
}

void Quaternion::SetEulerYXZ(const Vector3& p_euler) {
	num_fd half_a1 = p_euler.y * 0.5f;
	num_fd half_a2 = p_euler.x * 0.5f;
	num_fd half_a3 = p_euler.z * 0.5f;

	// R = Y(a1).X(a2).Z(a3) convention for Euler angles.
	// Conversion to quaternion as listed in https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19770024290.pdf (page A-6)
	// a3 is the angle of the first rotation, following the notation in this reference.

	num_fd cos_a1 = Math::Cos(half_a1);
	num_fd sin_a1 = Math::Sin(half_a1);
	num_fd cos_a2 = Math::Cos(half_a2);
	num_fd sin_a2 = Math::Sin(half_a2);
	num_fd cos_a3 = Math::Cos(half_a3);
	num_fd sin_a3 = Math::Sin(half_a3);

	Set(sin_a1 * cos_a2 * sin_a3 + cos_a1 * sin_a2 * cos_a3, 
		sin_a1 * cos_a2 * cos_a3 - cos_a1 * sin_a2 * sin_a3, 
		-sin_a1 * sin_a2 * cos_a3 + cos_a1 * cos_a2 * sin_a3,
		sin_a1 * sin_a2 * sin_a3 + cos_a1 * cos_a2 * cos_a3);
}
Vector3 Quaternion::GetEulerYXZ() const {
	Matrix3x3 m(*this);
	return m.GetEulerYXZ();
}

num_fd Quaternion::Length() const {
	return Math::Sqrt(LengthSquared());
}

num_fd Quaternion::AngleTo(const Quaternion& p_to) const {
	num_fd d = Dot(p_to);
	return Math::Acos(Math::Clamp(d * d * 2 - 1, -1, 1));
}

bool Quaternion::IsEqualApprox(const Quaternion& p_q) const {
	return Math::Abs(x - p_q.x) < Math::EPSILON &&
		Math::Abs(y - p_q.y) < Math::EPSILON &&
		Math::Abs(z - p_q.z) < Math::EPSILON &&
		Math::Abs(w - p_q.w) < Math::EPSILON;
}
void Quaternion::Normalize() {
	*this / Length();
}
Quaternion Quaternion::Normalized() const {
	return *this / Length();
}
bool Quaternion::IsNormalized() const {
	return Math::Abs(Length() - 1.0f) < Math::EPSILON;
}
Quaternion Quaternion::Inverse() const {
	return Quaternion(-x, -y, -z, w);
}

void Quaternion::operator*=(const Quaternion& p_q) {
	Set(w * p_q.x + x * p_q.w + y * p_q.z - z * p_q.y,
		w * p_q.y + y * p_q.w + z * p_q.x - x * p_q.z,
		w * p_q.z + z * p_q.w + x * p_q.y - y * p_q.x,
		w * p_q.w - x * p_q.x - y * p_q.y - z * p_q.z);
}



