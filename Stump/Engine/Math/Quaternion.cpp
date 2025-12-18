#include "Math/Quaternion.h"
#include "Math/Matrix3x3.h"
namespace Math {
	Quaternion::Quaternion() : x(0), y(0), z(0), w(0) {}

	Quaternion::Quaternion(num_fd p_x, num_fd p_y, num_fd p_z, num_fd p_w) : x(p_x), y(p_y), z(p_z), w(p_w) {}

	Quaternion::Quaternion(const Quaternion& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

	Quaternion::Quaternion(const Vector3& euler) { SetEuler(euler); }

	Quaternion::Quaternion(const Vector3& axis, const num_fd& angle) { SetAxisAngle(axis, angle); }

	Quaternion::Quaternion(const Vector3& vec1, Vector3& vec2) {
		Vector3 c = vec1.Cross(vec2);
		num_fd d = vec1.Dot(vec2);

		if (d < -1 + (num_fd)MathF::EPSILON) {
			x = 0;
			y = 1;
			z = 0;
			w = 0;
		}
		else {
			num_fd s = MathF::Sqrt((1 + d) * 2);
			num_fd rs = 1 / s;

			x = c.x * rs;
			y = c.y * rs;
			z = c.z * rs;
			w = s * 0.5f;
		}
	}

	Quaternion Quaternion::Slerp(const Quaternion& p_to, const num_fd& p_weight) const {
		Quaternion to1;
		num_fd omega, cosom, sinom, scale0, scale1;

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

		if ((1 - cosom) > (num_fd)MathF::EPSILON) {
			omega = MathF::Acos(cosom);
			sinom = MathF::Sin(omega);
			scale0 = MathF::Sin((1 - p_weight) * omega) / sinom;
			scale1 = MathF::Sin(p_weight * omega) / sinom;
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

		num_fd cos_a1 = MathF::Cos(half_a1);
		num_fd sin_a1 = MathF::Sin(half_a1);
		num_fd cos_a2 = MathF::Cos(half_a2);
		num_fd sin_a2 = MathF::Sin(half_a2);
		num_fd cos_a3 = MathF::Cos(half_a3);
		num_fd sin_a3 = MathF::Sin(half_a3);

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

		num_fd cos_a1 = MathF::Cos(half_a1);
		num_fd sin_a1 = MathF::Sin(half_a1);
		num_fd cos_a2 = MathF::Cos(half_a2);
		num_fd sin_a2 = MathF::Sin(half_a2);
		num_fd cos_a3 = MathF::Cos(half_a3);
		num_fd sin_a3 = MathF::Sin(half_a3);

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
		return MathF::Sqrt(LengthSquared());
	}

	num_fd Quaternion::AngleTo(const Quaternion& p_to) const {
		num_fd d = Dot(p_to);
		return MathF::Acos(MathF::Clamp(d * d * 2 - 1, -1, 1));
	}

	bool Quaternion::IsEqualApprox(const Quaternion& p_q) const {
		return MathF::Abs(x - p_q.x) < MathF::EPSILON &&
			MathF::Abs(y - p_q.y) < MathF::EPSILON &&
			MathF::Abs(z - p_q.z) < MathF::EPSILON &&
			MathF::Abs(w - p_q.w) < MathF::EPSILON;
	}
	void Quaternion::Normalize() {
		*this / Length();
	}
	Quaternion Quaternion::Normalized() const {
		return *this / Length();
	}
	bool Quaternion::IsNormalized() const {
		return MathF::Abs(Length() - 1.0f) < MathF::EPSILON;
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

	num_fd Quaternion::Dot(const Quaternion& p_q) const {
		return x * p_q.x + y * p_q.y + z * p_q.z + w * p_q.w;
	}

	num_fd Quaternion::LengthSquared() const {
		return Dot(*this);
	}

	void Quaternion::SetAxisAngle(const Vector3& axis, const num_fd& angle) {
		num_fd d = axis.Length();
		if (d == 0) {
			Set(0, 0, 0, 0);
		}
		else {
			num_fd sin_angle = MathF::Sin(angle * 0.5f);
			num_fd cos_angle = MathF::Cos(angle * 0.5f);
			num_fd s = sin_angle / d;
			Set(axis.x * s, axis.y * s, axis.z * s,
				cos_angle);
		}
	}

	void Quaternion::operator+=(const Quaternion& p_q) {
		x += p_q.x;
		y += p_q.y;
		z += p_q.z;
		w += p_q.w;
	}

	void Quaternion::operator-=(const Quaternion& p_q) {
		x -= p_q.x;
		y -= p_q.y;
		z -= p_q.z;
		w -= p_q.w;
	}

	void Quaternion::operator*=(const num_fd& s) {
		x *= s;
		y *= s;
		z *= s;
		w *= s;
	}

	void Quaternion::operator/=(const num_fd& s) {
		*this *= 1 / s;
	}

	Quaternion Quaternion::operator+(const Quaternion& q2) const {
		const Quaternion& q1 = *this;
		return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
	}

	Quaternion Quaternion::operator-(const Quaternion& q2) const {
		const Quaternion& q1 = *this;
		return Quaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
	}

	Quaternion Quaternion::operator-() const {
		const Quaternion& q2 = *this;
		return Quaternion(-q2.x, -q2.y, -q2.z, -q2.w);
	}

	Quaternion Quaternion::operator*(const num_fd& s) const {
		return Quaternion(x * s, y * s, z * s, w * s);
	}

	Quaternion Quaternion::operator/(const num_fd& s) const {
		return *this * (1 / s);
	}

	bool Quaternion::operator==(const Quaternion& p_quat) const {
		return x == p_quat.x && y == p_quat.y && z == p_quat.z && w == p_quat.w;
	}

	bool Quaternion::operator!=(const Quaternion& p_quat) const {
		return x != p_quat.x || y != p_quat.y || z != p_quat.z || w != p_quat.w;
	}


}