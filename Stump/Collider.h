#ifndef ST_COLLIDER_H
#define ST_COLLIDER_H

#include "Matrix3x4.h"
#include "AABB.h"
#include "Component.h"
#include <memory>

enum ColliderType {
	COLLIDER_SPHERE,
	COLLIDER_BOX,
	COLLIDER_PLANE
};

struct CollisionPoints {
	Vector3 pointA;
	Vector3 pointB;
	Vector3 normal;
	num_fd penetrationDepth;
	bool hasCollision;
};

class Collider : public Component
{
public:
	virtual ColliderType GetColliderType() const = 0;
	virtual AABB GetAABB() const = 0;

};

class CollisionDetector
{
	using FindContactFunc = CollisionPoints(*)(const Collider*, const Collider*);

public:

	CollisionPoints TestCollision(const Collider* p_collider1, const Collider* p_collider2);

private:
	static CollisionPoints TestSphereSphere(const Collider* p_collider1, const Collider* p_collider2);
	static CollisionPoints TestSpherePlane(const Collider* p_collider1, const Collider* p_collider2);
	static CollisionPoints TestSphereBox(const Collider* p_collider1, const Collider* p_collider2);
	static CollisionPoints TestBoxBox(const Collider* p_collider1, const Collider* p_collider2);
	static CollisionPoints TestBoxPlane(const Collider* p_collider1, const Collider* p_collider2);
	static CollisionPoints TestPlanePlane(const Collider* p_collider1, const Collider* p_collider2);

};

class SphereCollider : public Collider
{

public:
	Vector3 center;
	float radius;
	SphereCollider(Vector3 p_center, float p_radius)
		: center(p_center), radius(p_radius) {
	}
	ColliderType GetColliderType() const override
	{
		return COLLIDER_SPHERE;
	}
	virtual AABB GetAABB() const override;
};
class BoxCollider : public Collider
{
public:
	Vector3 center;
	Vector3 halfExtents;
	Matrix3x3 orientation;

	BoxCollider(Vector3 p_center, Vector3 p_halfExtents, Matrix3x3 p_orientation)
		: center(p_center), halfExtents(p_halfExtents), orientation(p_orientation) {
	}
	ColliderType GetColliderType() const override
	{
		return COLLIDER_BOX;
	}
	virtual AABB GetAABB() const override;
};
class PlaneCollider : public Collider
{
public:
	Vector3 normal;
	num_fd distance;

	PlaneCollider(Vector3 p_normal, num_fd p_distance)
		: normal(p_normal), distance(p_distance) {
	}
	ColliderType GetColliderType() const override
	{
		return COLLIDER_PLANE;
	}
	virtual AABB GetAABB() const override;
};
#endif // ST_COLLIDER_H

