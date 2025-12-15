#ifndef ST_RIGID_BODY_H
#define ST_RIGID_BODY_H

#include "Matrix3x4.h"
#include "Vector3.h"
#include "Collider.h"

#include <memory>

struct Collision {
	std::shared_ptr<RigidBody> bodyA;
	std::shared_ptr<RigidBody> bodyB;
	CollisionPoints points;
};

class RigidBody
{
	std::unique_ptr<Matrix3x4> transform;
	std::unique_ptr<Collider> collider;
	Vector3 velocity;
	Vector3 force;
public:

	float mass = 1.0f;
	bool isStatic = false;
	bool affectedByGravity = true;

	RigidBody(Matrix3x4* p_transform) : transform(std::make_unique<Matrix3x4>(p_transform)) {}
	RigidBody(Matrix3x4* p_transform, bool p_mass, bool p_isStatic)
		: transform(std::make_unique<Matrix3x4>(p_transform)), mass(p_mass), isStatic(p_isStatic) {
	}

	void Integrate(float deltaTime);

	Vector3 GetPosition() const { return transform->origin; }
	Vector3 GetVelocity() const { return velocity; }

	void SetCollider(Collider* p_collider) { collider = std::unique_ptr<Collider>(p_collider); }

	void ApplyForce(const Vector3& p_force) { force += p_force; }
	void SetVelocity(const Vector3& p_velocity) { velocity = p_velocity; }
};
#endif // ST_RIGID_BODY_H

