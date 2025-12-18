#ifndef ST_VECTOR3_H
#define ST_VECTOR3_H

#include "Math/MathF.h"
#include "Math/Typedefs.h"

namespace Math {
	struct Vector3 {

		union {
			struct {
				num_fd x;
				num_fd y;
				num_fd z;
			};
			num_fd coord[3];
		};
		Vector3();

		Vector3(num_fd p_x, num_fd p_y, num_fd p_z);

		Vector3(const Vector3& p_other);

		const num_fd& operator[](int index) const {
			return coord[index];
		}

		num_fd& operator[](int index) {
			return coord[index];
		}

		void SetAll(num_fd p_value) {
			x = y = z = p_value;
		}

		int MinAxis() const {
			return x < y ? (x < z ? 0 : 2) : (y < z ? 1 : 2);
		}

		int MaxAxis() const {
			return x < y ? (y < z ? 2 : 1) : (x < z ? 2 : 0);
		}

		num_fd Length() const;
		num_fd LengthSquared() const;

		void Normalize();
		Vector3 Normalized() const;
		bool IsNormalized() const;
		Vector3 Inverse() const;
		Vector3 LimitLength(const num_fd p_len = 1.0) const;

		void Zero();

		void Snap(Vector3 p_val);
		Vector3 Snapped(Vector3 p_val) const;

		Vector3 Slerp(const Vector3& p_to, num_fd p_weight) const;
		Vector3 LinearInterpolate(const Vector3& p_to, num_fd p_weight) const;

		Vector3 Cross(const Vector3& p_b) const;
		num_fd Dot(const Vector3& p_b) const;

		Vector3 Abs() const;
		Vector3 Floor() const;
		Vector3 Sign() const;
		Vector3 Ceil() const;
		Vector3 Round() const;

		num_fd DistanceTo(const Vector3& p_to) const;
		num_fd DistanceSquaredTo(const Vector3& p_to) const;

		Vector3 Posmod(const num_fd p_mod) const;
		Vector3 Posmodv(const Vector3& p_modv) const;
		Vector3 Project(const Vector3& p_to) const;

		num_fd AngleTo(const Vector3& p_to) const;
		num_fd SignedAngleTo(const Vector3& p_to, const Vector3& p_axis) const;
		Vector3 DirectionTo(const Vector3& p_to) const;
		void RotateAroundAxis(const Vector3& p_axis, num_fd p_angle);
		Vector3 RotatedAroundAxis(const Vector3& p_axis, num_fd p_angle) const;

		Vector3 Slide(const Vector3& p_normal) const;
		Vector3 Bounce(const Vector3& p_normal) const;
		Vector3 Reflect(const Vector3& p_normal) const;

		bool IsEqualApprox(const Vector3& p_v) const;
		bool IsEqualApprox(const Vector3& p_v, num_fd p_tolerance) const;

		Vector3& operator+=(const Vector3& p_v);
		Vector3 operator+(const Vector3& p_v) const;
		Vector3& operator-=(const Vector3& p_v);
		Vector3 operator-(const Vector3& p_v) const;
		Vector3& operator*=(const Vector3& p_v);
		Vector3 operator*(const Vector3& p_v) const;
		Vector3& operator/=(const Vector3& p_v);
		Vector3 operator/(const Vector3& p_v) const;

		Vector3& operator*=(num_fd p_scalar);
		Vector3 operator*(num_fd p_scalar) const;
		Vector3& operator/=(num_fd p_scalar);
		Vector3 operator/(num_fd p_scalar) const;

		Vector3 operator-() const;

		bool operator==(const Vector3& p_v) const;
		bool operator!=(const Vector3& p_v) const;
		bool operator<(const Vector3& p_v) const;
		bool operator<=(const Vector3& p_v) const;
		bool operator>(const Vector3& p_v) const;
		bool operator>=(const Vector3& p_v) const;

	};
}
#endif // VECTOR3_H

