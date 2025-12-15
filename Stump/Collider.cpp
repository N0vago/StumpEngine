#include "Collider.h"
#include <assert.h>
#include <algorithm>
CollisionPoints CollisionDetector::TestCollision(const Collider* p_collider1, const Collider* p_collider2)
{
	static const FindContactFunc collisionTesters[3][3] = {
		{ &CollisionDetector::TestSphereSphere, &CollisionDetector::TestSpherePlane, &CollisionDetector::TestSphereBox },
		{ nullptr, &CollisionDetector::TestBoxPlane, &CollisionDetector::TestBoxBox },
		{ nullptr, nullptr, &CollisionDetector::TestPlanePlane }
	};
	bool swap = p_collider2->GetColliderType() > p_collider1->GetColliderType();

	if (swap)
	{
		std::swap(p_collider1, p_collider2);
	}
	CollisionPoints points = collisionTesters[p_collider1->GetColliderType()][p_collider2->GetColliderType()](p_collider1, p_collider2);

	if (swap)
	{
		std::swap(points.pointA, points.pointB);
		points.normal = -points.normal;
	}

	return points;
}

CollisionPoints CollisionDetector::TestSphereSphere(const Collider* p_collider1, const Collider* p_collider2)
{
	assert(p_collider1->GetColliderType() != COLLIDER_SPHERE || p_collider2->GetColliderType() != COLLIDER_SPHERE);
	auto s1 = static_cast<const SphereCollider*>(p_collider1);
	auto s2 = static_cast<const SphereCollider*>(p_collider2);

	Vector3 delta = s2->center - s1->center;
	float distSq = delta.LengthSquared();
	float r = s1->radius + s2->radius;

	if (distSq > r * r)
		return {};

	float dist = Math::Sqrt(distSq);

	CollisionPoints cp;
	cp.hasCollision = true;

	cp.normal = (dist > 0.0f) ? delta / dist : Vector3(1, 0, 0);
	cp.penetrationDepth = r - dist;

	cp.pointA = s1->center + cp.normal * s1->radius;
	cp.pointB = s2->center - cp.normal * s2->radius;

	return cp;
}

CollisionPoints CollisionDetector::TestSpherePlane(const Collider* p_collider1, const Collider* p_collider2)
{
	assert(p_collider1->GetColliderType() != COLLIDER_SPHERE || p_collider2->GetColliderType() != COLLIDER_PLANE);
	auto sphere = static_cast<const SphereCollider*>(p_collider1);
	auto plane = static_cast<const PlaneCollider*>(p_collider2);

	float distance =
		plane->normal.Dot(sphere->center) + plane->distance;

	if (distance > sphere->radius)
		return {};

	CollisionPoints cp;
	cp.hasCollision = true;

	cp.normal = plane->normal;
	cp.penetrationDepth = sphere->radius - distance;

	cp.pointA = sphere->center - plane->normal * distance;
	cp.pointB = cp.pointA - plane->normal * cp.penetrationDepth;

	return cp;
}

CollisionPoints CollisionDetector::TestSphereBox(const Collider* p_collider1, const Collider* p_collider2)
{
	assert(p_collider1->GetColliderType() != COLLIDER_SPHERE || p_collider2->GetColliderType() != COLLIDER_BOX);
	auto sphere = static_cast<const SphereCollider*>(p_collider1);
	auto box = static_cast<const BoxCollider*>(p_collider2);

	Vector3 local = box->orientation.Transposed() * (sphere->center - box->center);

	Vector3 closest = local;
	closest.x = Math::Clamp(closest.x, -box->halfExtents.x, box->halfExtents.x);
	closest.y = Math::Clamp(closest.y, -box->halfExtents.y, box->halfExtents.y);
	closest.z = Math::Clamp(closest.z, -box->halfExtents.z, box->halfExtents.z);

	Vector3 delta = local - closest;
	float distSq = delta.LengthSquared();

	if (distSq > sphere->radius * sphere->radius)
		return {};

	Vector3 worldPoint = box->orientation.operator*(closest) + box->center;
	Vector3 normal = (sphere->center - worldPoint).Normalized();

	CollisionPoints cp;
	cp.hasCollision = true;
	cp.normal = normal;
	cp.penetrationDepth = sphere->radius - std::sqrt(distSq);
	cp.pointA = sphere->center - normal * sphere->radius;
	cp.pointB = worldPoint;

	return cp;
}

CollisionPoints CollisionDetector::TestBoxBox(const Collider* p_collider1, const Collider* p_collider2)
{
	return CollisionPoints();
}

CollisionPoints CollisionDetector::TestBoxPlane(const Collider* p_collider1, const Collider* p_collider2)
{
	assert(p_collider1->GetColliderType() != COLLIDER_SPHERE || p_collider2->GetColliderType() != COLLIDER_BOX);
	auto box = static_cast<const BoxCollider*>(p_collider1);
	auto plane = static_cast<const PlaneCollider*>(p_collider2);
	float r =
		Math::Abs(plane->normal.Dot(box->orientation.GetColumn(0))) * box->halfExtents.x +
		Math::Abs(plane->normal.Dot(box->orientation.GetColumn(1))) * box->halfExtents.y +
		Math::Abs(plane->normal.Dot(box->orientation.GetColumn(2))) * box->halfExtents.z;

	float d = plane->normal.Dot(box->center) + plane->distance;

	if (Math::Abs(d) > r)
		return {};

	CollisionPoints cp;
	cp.hasCollision = true;
	cp.normal = plane->normal;
	cp.penetrationDepth = r - Math::Abs(d);
	cp.pointA = box->center - plane->normal * d;
	cp.pointB = cp.pointA - plane->normal * cp.penetrationDepth;

	return cp;
}

CollisionPoints CollisionDetector::TestPlanePlane(const Collider* p_collider1, const Collider* p_collider2)
{
	assert(p_collider1->GetColliderType() != COLLIDER_PLANE || p_collider2->GetColliderType() != COLLIDER_PLANE);
	const PlaneCollider* p1 = static_cast<const PlaneCollider*>(p_collider1);
	const PlaneCollider* p2 = static_cast<const PlaneCollider*>(p_collider2);

	if (Math::Abs(p1->normal.Dot(p2->normal)) < 0.999f)
		return {};

	float d = Math::Abs(p1->distance - p2->distance);
	if (d > 0.001f)
		return {};

	CollisionPoints cp;
	cp.hasCollision = true;
	cp.normal = p1->normal;
	cp.penetrationDepth = 0.0f;

	return cp;
}
AABB SphereCollider::GetAABB() const
{
	return AABB(center - Vector3(radius, radius, radius), center + Vector3(radius, radius, radius));
}
AABB BoxCollider::GetAABB() const
{
	Vector3 corners[8];
	corners[0] = center + orientation.GetColumn(0) * halfExtents.x + orientation.GetColumn(1) * halfExtents.y + orientation.GetColumn(2) * halfExtents.z;
	corners[1] = center + orientation.GetColumn(0) * halfExtents.x + orientation.GetColumn(1) * halfExtents.y - orientation.GetColumn(2) * halfExtents.z;
	corners[2] = center + orientation.GetColumn(0) * halfExtents.x - orientation.GetColumn(1) * halfExtents.y + orientation.GetColumn(2) * halfExtents.z;
	corners[3] = center + orientation.GetColumn(0) * halfExtents.x - orientation.GetColumn(1) * halfExtents.y - orientation.GetColumn(2) * halfExtents.z;
	corners[4] = center - orientation.GetColumn(0) * halfExtents.x + orientation.GetColumn(1) * halfExtents.y + orientation.GetColumn(2) * halfExtents.z;
	corners[5] = center - orientation.GetColumn(0) * halfExtents.x + orientation.GetColumn(1) * halfExtents.y - orientation.GetColumn(2) * halfExtents.z;
	corners[6] = center - orientation.GetColumn(0) * halfExtents.x - orientation.GetColumn(1) * halfExtents.y + orientation.GetColumn(2) * halfExtents.z;
	corners[7] = center - orientation.GetColumn(0) * halfExtents.x - orientation.GetColumn(1) * halfExtents.y - orientation.GetColumn(2) * halfExtents.z;
	Vector3 min = corners[0];
	Vector3 max = corners[0];
	for (int i = 1; i < 8; ++i)
	{
		min.x = std::min(min.x, corners[i].x);
		min.y = std::min(min.y, corners[i].y);
		min.z = std::min(min.z, corners[i].z);
		max.x = std::max(max.x, corners[i].x);
		max.y = std::max(max.y, corners[i].y);
		max.z = std::max(max.z, corners[i].z);
	}
	return AABB(min, max);
}

AABB PlaneCollider::GetAABB() const
{
	return AABB(Vector3(-INFINITY, -INFINITY, -INFINITY), Vector3(INFINITY, INFINITY, INFINITY));
}

