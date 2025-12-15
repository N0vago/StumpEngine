#ifndef ST_COLLIDER_H
#define ST_COLLIDER_H

#include "Matrix3x4.h"
#include "RigidBody.h"
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

class Collider
{
public:

	Matrix3x4 localTransform;

	std::unique_ptr<RigidBody> attachedRigidBody;

	Collider(Matrix3x4 p_localTransform) : localTransform(p_localTransform), attachedRigidBody(nullptr) {}
	~Collider() = default;

	virtual ColliderType GetColliderType() const = 0;

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
	SphereCollider(Matrix3x4 p_transform, float p_radius)
		: Collider(p_transform), radius(p_radius) {
		center = localTransform.origin;
	}
	ColliderType GetColliderType() const override
	{
		return COLLIDER_SPHERE;
	}
};
class BoxCollider : public Collider
{
public:
	Vector3 center;
	Vector3 halfExtents;
	Matrix3x3 orientation;

	BoxCollider(Matrix3x4 p_transform)
		: Collider(p_transform) {
		center = localTransform.origin;	
		orientation = localTransform.mat3;
		halfExtents = Vector3(1.0f, 1.0f, 1.0f);
		
	}
	ColliderType GetColliderType() const override
	{
		return COLLIDER_BOX;
	}
};
class PlaneCollider : public Collider
{
public:
	Vector3 normal;
	num_fd distance;

	PlaneCollider(Matrix3x4 p_transform, Vector3 p_normal, num_fd p_distance)
		: Collider(p_transform), normal(p_normal), distance(p_distance) {
	}
	ColliderType GetColliderType() const override
	{
		return COLLIDER_PLANE;
	}
};
#endif // ST_COLLIDER_H

