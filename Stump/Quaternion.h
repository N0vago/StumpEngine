#ifndef ST_QUATERNION_H
#define ST_QUATERNION_H

#include "Vector3.h"
#include "Matrix3x3.h"
#include "Typedefs.h"
class Quaternion
{
public:

	num_fd x, y, z, w;

	Quaternion() : x(0), y(0), z(0), w(0) {}
	Quaternion(num_fd p_x, num_fd p_y, num_fd p_z, num_fd p_w) : x(p_x), y(p_y), z(p_z), w(p_w) {}
	Quaternion(const Quaternion& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
	Quaternion(const Vector3& euler) { SetEuler(euler); }
	Quaternion(const Vector3& axis, const float& angle) { SetAxisAngle(axis, angle); }
	Quaternion(const Vector3& vec1, Vector3& vec2) {
		Vector3 c = vec1.Cross(vec2);
		num_fd d = vec1.Dot(vec2);

		if (d < -1 + (num_fd)Math::EPSILON) {
			x = 0;
			y = 1;
			z = 0;
			w = 0;
		}
		else {
			num_fd s = Math::Sqrt((1 + d) * 2);
			num_fd rs = 1 / s;

			x = c.x * rs;
			y = c.y * rs;
			z = c.z * rs;
			w = s * 0.5f;
		}
	}


	num_fd Length() const;
	num_fd LengthSquared() const;
	bool IsEqualApprox(const Quaternion& p_q) const;
	void Normalize();
	Quaternion Normalized() const;
	bool IsNormalized() const;
	Quaternion Inverse() const;
	num_fd Dot(const Quaternion& p_q) const;
	num_fd AngleTo(const Quaternion& p_q) const;

	void SetEulerXYZ(const Vector3& p_euler);
	Vector3 GetEulerXYZ() const;
	void SetEulerYXZ(const Vector3& p_euler);
	Vector3 GetEulerYXZ() const;

	void SetEuler(const Vector3& p_euler) { SetEulerYXZ(p_euler); };
	Vector3 GetEuler() const { return GetEulerYXZ(); };

	Quaternion Slerp(const Quaternion& p_to, const num_fd& p_weight) const;
	
	void SetAxisAngle(const Vector3& axis, const num_fd& angle);
	void GetAxisAngle(Vector3& r_axis, num_fd& r_angle) const {
		r_angle = 2 * Math::Acos(w);
		num_fd r = ((num_fd)1) / Math::Sqrt(1 - w * w);
		r_axis.x = x * r;
		r_axis.y = y * r;
		r_axis.z = z * r;
	}

	void operator*=(const Quaternion& p_q);
	Quaternion operator*(const Quaternion& p_q) const;

	Quaternion operator*(const Vector3& v) const {
		return Quaternion(w * v.x + y * v.z - z * v.y,
			w * v.y + z * v.x - x * v.z,
			w * v.z + x * v.y - y * v.x,
			-x * v.x - y * v.y - z * v.z);
	}

	Vector3 XForm(const Vector3& v) const {

		Vector3 u(x, y, z);
		Vector3 uv = u.Cross(v);
		return v + ((uv * w) + u.Cross(uv)) * ((float)2);
	}

	void operator+=(const Quaternion& p_q);
	void operator-=(const Quaternion& p_q);
	void operator*=(const float& s);
	void operator/=(const float& s);
	Quaternion operator+(const Quaternion& q2) const;
	Quaternion operator-(const Quaternion& q2) const;
	Quaternion operator-() const;
	Quaternion operator*(const float& s) const;
	Quaternion operator/(const float& s) const;

	bool operator==(const Quaternion& p_quat) const;
	bool operator!=(const Quaternion& p_quat) const;

	inline void Set(float p_x, float p_y, float p_z, float p_w) {
		x = p_x;
		y = p_y;
		z = p_z;
		w = p_w;
	}

	Quaternion& operator=(const Quaternion& p_q) {
		x = p_q.x;
		y = p_q.y;
		z = p_q.z;
		w = p_q.w;
		return *this;
	}
};

num_fd Quaternion::Dot(const Quaternion& p_q) const {
	return x * p_q.x + y * p_q.y + z * p_q.z + w * p_q.w;
}

num_fd Quaternion::LengthSquared() const {
	return Dot(*this);
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

void Quaternion::operator*=(const float& s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}

void Quaternion::operator/=(const float& s) {
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

Quaternion Quaternion::operator*(const float& s) const {
	return Quaternion(x * s, y * s, z * s, w * s);
}

Quaternion Quaternion::operator/(const float& s) const {
	return *this * (1 / s);
}

bool Quaternion::operator==(const Quaternion& p_quat) const {
	return x == p_quat.x && y == p_quat.y && z == p_quat.z && w == p_quat.w;
}

bool Quaternion::operator!=(const Quaternion& p_quat) const {
	return x != p_quat.x || y != p_quat.y || z != p_quat.z || w != p_quat.w;
}
#endif

