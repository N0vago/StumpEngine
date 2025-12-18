#ifndef ST_QUATERNION_H
#define ST_QUATERNION_H

#include "Math/Vector3.h"
#include "Math/Typedefs.h"
namespace Math {
	class Quaternion
	{
	public:

		num_fd x, y, z, w;

		Quaternion();
		Quaternion(num_fd p_x, num_fd p_y, num_fd p_z, num_fd p_w);
		Quaternion(const Quaternion& other);
		Quaternion(const Vector3& euler);
		Quaternion(const Vector3& axis, const num_fd& angle);
		Quaternion(const Vector3& vec1, Vector3& vec2);


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
			r_angle = 2 * MathF::Acos(w);
			num_fd r = ((num_fd)1) / MathF::Sqrt(1 - w * w);
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
			return v + ((uv * w) + u.Cross(uv)) * ((num_fd)2);
		}

		void operator+=(const Quaternion& p_q);
		void operator-=(const Quaternion& p_q);
		void operator*=(const num_fd& s);
		void operator/=(const num_fd& s);
		Quaternion operator+(const Quaternion& q2) const;
		Quaternion operator-(const Quaternion& q2) const;
		Quaternion operator-() const;
		Quaternion operator*(const num_fd& s) const;
		Quaternion operator/(const num_fd& s) const;

		bool operator==(const Quaternion& p_quat) const;
		bool operator!=(const Quaternion& p_quat) const;

		inline void Set(num_fd p_x, num_fd p_y, num_fd p_z, num_fd p_w) {
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
}
#endif

