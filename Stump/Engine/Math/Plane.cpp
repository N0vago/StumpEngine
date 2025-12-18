#include "Math/Plane.h"
namespace Math {
	Plane::Plane() :
		d(0) {
	}
	Plane::Plane(num_fd p_a, num_fd p_b, num_fd p_c, num_fd p_d) :
		normal(p_a, p_b, p_c),
		d(p_d) {
	}

	Plane::Plane(const Vector3& p_normal, num_fd p_d) :
		normal(p_normal),
		d(p_d) {
	}

	Plane::Plane(const Vector3& p_point, const Vector3& p_normal) :
		normal(p_normal),
		d(p_normal.Dot(p_point)) {
	}

	Plane::Plane(const Vector3& p_point1, const Vector3& p_point2, const Vector3& p_point3, ClockDirection p_dir) {
		if (p_dir == CLOCKWISE) {
			normal = (p_point1 - p_point3).Cross(p_point1 - p_point2);
		}
		else {
			normal = (p_point1 - p_point2).Cross(p_point1 - p_point3);
		}

		normal.Normalize();
		d = normal.Dot(p_point1);
	}

	bool Plane::IsPointOver(const Vector3& p_point) const {
		return (normal.Dot(p_point) > d);
	}

	num_fd Plane::DistanceTo(const Vector3& p_point) const {
		return (normal.Dot(p_point) - d);
	}

	bool Plane::HasPoint(const Vector3& p_point, num_fd _epsilon) const {
		num_fd dist = normal.Dot(p_point) - d;
		dist = MathF::Abs(dist);
		return (dist <= _epsilon);
	}



	bool Plane::operator==(const Plane& p_plane) const {
		return normal == p_plane.normal && d == p_plane.d;
	}

	bool Plane::operator!=(const Plane& p_plane) const {
		return normal != p_plane.normal || d != p_plane.d;
	}

	void Plane::SetNormal(const Vector3& p_normal) {
		normal = p_normal;
	}

	void Plane::Normalize() {
		num_fd l = normal.Length();
		if (l == 0) {
			*this = Plane(0, 0, 0, 0);
			return;
		}
		normal /= l;
		d /= l;
	}

	Plane Plane::Normalized() const {
		Plane p = *this;
		p.Normalize();
		return p;
	}

	Vector3 Plane::GetAnyPoint() const {
		return GetNormal() * d;
	}

	Vector3 Plane::GetAnyPerpendicularNormal() const {
		static const Vector3 p1 = Vector3(1, 0, 0);
		static const Vector3 p2 = Vector3(0, 1, 0);
		Vector3 p;

		if (MathF::Abs(normal.Dot(p1)) > 0.99f) {
			p = p2;
		}
		else {
			p = p1;
		}

		p -= normal * normal.Dot(p);
		p.Normalize();

		return p;
	}


	bool Plane::Intersect3(const Plane& p_plane1, const Plane& p_plane2, Vector3* r_result) const {
		const Plane& p_plane0 = *this;
		Vector3 normal0 = p_plane0.normal;
		Vector3 normal1 = p_plane1.normal;
		Vector3 normal2 = p_plane2.normal;

		num_fd denom = normal0.Cross(normal1).Dot(normal2);

		if (MathF::IsZeroApprox(denom)) {
			return false;
		}

		if (r_result) {
			*r_result = ((normal1.Cross(normal2) * p_plane0.d) +
				(normal2.Cross(normal0) * p_plane1.d) +
				(normal0.Cross(normal1) * p_plane2.d)) /
				denom;
		}

		return true;
	}

	bool Plane::IntersectsRay(const Vector3& p_from, const Vector3& p_dir, Vector3* p_intersection) const {
		Vector3 segment = p_dir;
		num_fd den = normal.Dot(segment);

		if (MathF::IsZeroApprox(den)) {
			return false;
		}

		num_fd dist = (normal.Dot(p_from) - d) / den;

		if (dist > (num_fd)MathF::UNIT_EPSILON) {

			return false;
		}

		dist = -dist;
		*p_intersection = p_from + segment * dist;

		return true;
	}

	bool Plane::IntersectsSegment(const Vector3& p_begin, const Vector3& p_end, Vector3* p_intersection) const {
		Vector3 segment = p_begin - p_end;
		num_fd den = normal.Dot(segment);

		if (MathF::IsZeroApprox(den)) {
			return false;
		}

		num_fd dist = (normal.Dot(p_begin) - d) / den;

		if (dist < (num_fd)-MathF::UNIT_EPSILON || dist >(1 + (num_fd)MathF::UNIT_EPSILON)) {
			return false;
		}

		dist = -dist;
		*p_intersection = p_begin + segment * dist;

		return true;
	}

	bool Plane::IsEqualApprox(const Plane& p_plane) const {
		return normal.IsEqualApprox(p_plane.normal) && MathF::IsEqualApprox(d, p_plane.d);
	}
}