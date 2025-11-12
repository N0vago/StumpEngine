#ifndef ST_PLANE_H
#define ST_PLANE_H

#include "Vector3.h"
class Plane
{
public:
	Vector3 normal;
	num_fd d;

	void SetNormal(const Vector3& p_normal);
	Vector3 GetNormal() const { return normal; };

	void Normalize();
	Plane Normalized() const;

	Vector3 Center() const { return normal * d; }
	Vector3 GetAnyPoint() const;
	Vector3 GetAnyPerpendicularNormal() const;

	bool IsPointOver(const Vector3& p_point) const;
	num_fd DistanceTo(const Vector3& p_point) const;
	bool HasPoint(const Vector3& p_point, num_fd _epsilon = Math::UNIT_EPSILON) const;


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

	Plane() :
		d(0) {
	}
	Plane(num_fd p_a, num_fd p_b, num_fd p_c, num_fd p_d) :
		normal(p_a, p_b, p_c),
		d(p_d) {
	}

	Plane(const Vector3& p_normal, num_fd p_d);
	Plane(const Vector3& p_point, const Vector3& p_normal);
	Plane(const Vector3& p_point1, const Vector3& p_point2, const Vector3& p_point3, ClockDirection p_dir = CLOCKWISE);
};
bool Plane::IsPointOver(const Vector3& p_point) const {
	return (normal.Dot(p_point) > d);
}

num_fd Plane::DistanceTo(const Vector3& p_point) const {
	return (normal.Dot(p_point) - d);
}

bool Plane::HasPoint(const Vector3& p_point, num_fd _epsilon) const {
	num_fd dist = normal.Dot(p_point) - d;
	dist = Math::Abs(dist);
	return (dist <= _epsilon);
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

bool Plane::operator==(const Plane& p_plane) const {
	return normal == p_plane.normal && d == p_plane.d;
}

bool Plane::operator!=(const Plane& p_plane) const {
	return normal != p_plane.normal || d != p_plane.d;
}

#endif // ST_PLANE_H
