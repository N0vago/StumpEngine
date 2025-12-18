#ifndef ST_PLANE_H
#define ST_PLANE_H

#include "Math/Vector3.h"
namespace Math {
	class Plane
	{
	public:
		Vector3 normal;
		num_fd d;

		Plane();
		Plane(num_fd p_a, num_fd p_b, num_fd p_c, num_fd p_d);

		Plane(const Vector3& p_normal, num_fd p_d);
		Plane(const Vector3& p_point, const Vector3& p_normal);
		Plane(const Vector3& p_point1, const Vector3& p_point2, const Vector3& p_point3, ClockDirection p_dir = CLOCKWISE);

		void SetNormal(const Vector3& p_normal);
		Vector3 GetNormal() const { return normal; };

		void Normalize();
		Plane Normalized() const;

		Vector3 Center() const { return normal * d; }
		Vector3 GetAnyPoint() const;
		Vector3 GetAnyPerpendicularNormal() const;

		bool IsPointOver(const Vector3& p_point) const;
		num_fd DistanceTo(const Vector3& p_point) const;
		bool HasPoint(const Vector3& p_point, num_fd _epsilon = MathF::UNIT_EPSILON) const;


		bool Intersect3(const Plane& p_plane1, const Plane& p_plane2, Vector3* r_result = nullptr) const;
		bool IntersectsRay(const Vector3& p_from, const Vector3& p_dir, Vector3* p_intersection) const;
		bool IntersectsSegment(const Vector3& p_begin, const Vector3& p_end, Vector3* p_intersection) const;

		Vector3 Project(const Vector3& p_point) const {
			return p_point - normal * DistanceTo(p_point);
		}

		Plane operator-() const { return Plane(-normal, -d); }
		bool IsEqualApprox(const Plane& p_plane) const;

		bool operator==(const Plane& p_plane) const;
		bool operator!=(const Plane& p_plane) const;


	};
}

#endif // ST_PLANE_H
