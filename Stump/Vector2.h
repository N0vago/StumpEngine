#ifndef ST_VECTOR2_H
#define ST_VECTOR2_H

#include "Typedefs.h"

struct Vector2
{
	union {
		struct {
			num_fd x;
			num_fd y;
		};
		num_fd coord[2];
	};
	Vector2();

	Vector2(num_fd p_x, num_fd p_y);

	Vector2(const Vector2& p_other);

	const num_fd& operator[](int index) const {
		return coord[index];
	}

	num_fd& operator[](int index) {
		return coord[index];
	}

	void SetAll(num_fd p_value) {
		x = y = p_value;
	}

	int MinAxis() const {
		return x < y ? 0 : 1;
	}

	int MaxAxis() const {
		return x < y ? 1 : 0;
	}

	num_fd Length() const;
	num_fd LengthSquared() const;

	void Normalize();
	Vector2 Normalized() const;
	bool IsNormalized() const;

	Vector2 LimitLength(const num_fd p_len = 1.0) const;

	void Zero();

	void Snap(Vector2 p_val);
	Vector2 Snapped(Vector2 p_val) const;


	static Vector2 LinearInterpolate(const Vector2& p_a, const Vector2& p_b, num_fd p_weight);
	Vector2 LinearInterpolate(const Vector2& p_to, num_fd p_weight) const;
	Vector2 Slerp(const Vector2& p_to, num_fd p_weight) const;

	num_fd Cross(const Vector2& p_b) const;
	num_fd Dot(const Vector2& p_b) const;

	Vector2 Abs() const;
	Vector2 Floor() const;
	Vector2 Sign() const;
	Vector2 Ceil() const;
	Vector2 Round() const;

	num_fd DistanceTo(const Vector2& p_to) const;
	num_fd DistanceSquaredTo(const Vector2& p_to) const;

	Vector2 Posmod(const num_fd p_mod) const;
	Vector2 Posmodv(const Vector2& p_modv) const;
	Vector2 Project(const Vector2& p_to) const;

	num_fd Angle() const;
	num_fd AngleTo(const Vector2& p_to) const;
	num_fd AngleToPoint(const Vector2& p_to) const;
	Vector2 DirectionTo(const Vector2& p_to) const;

	void SetRotation(num_fd p_radians);
	void Rotate(num_fd p_radians);
	Vector2 Rotated(num_fd p_radians) const;

	Vector2 Slide(const Vector2& p_normal) const;
	Vector2 Bounce(const Vector2& p_normal) const;
	Vector2 Reflect(const Vector2& p_normal) const;

	bool IsEqualApprox(const Vector2& p_v) const;
	bool IsEqualApprox(const Vector2& p_v, num_fd p_tolerance) const;

	

	Vector2& operator+=(const Vector2& p_v);
	Vector2 operator+(const Vector2& p_v) const;
	Vector2& operator-=(const Vector2& p_v);
	Vector2 operator-(const Vector2& p_v) const;
	Vector2& operator*=(const Vector2& p_v);
	Vector2 operator*(const Vector2& p_v) const;
	Vector2& operator/=(const Vector2& p_v);
	Vector2 operator/(const Vector2& p_v) const;

	Vector2& operator*=(num_fd p_scalar);
	Vector2 operator*(num_fd p_scalar) const;
	Vector2& operator/=(num_fd p_scalar);
	Vector2 operator/(num_fd p_scalar) const;

	Vector2 operator-() const;

	bool operator==(const Vector2& p_v) const;
	bool operator!=(const Vector2& p_v) const;
	bool operator<(const Vector2& p_v) const;
	bool operator<=(const Vector2& p_v) const;
	bool operator>(const Vector2& p_v) const;
	bool operator>=(const Vector2& p_v) const;
};
#endif // !ST_VECTOR2_H

