#ifndef ST_RIGID_BODY_H
#define ST_RIGID_BODY_H
#include "Vector3.h"
#include "Collider.h"
#include "Component.h"

class RigidBody : public Component
{
public:
	Matrix3x4* transform;
	std::unique_ptr<Collider> collider;

	Vector3 velocity;
	Vector3 angularVelocity;
	Vector3 force;
	Vector3 torque;

	float mass = 1.0f;
	float inertia = 1.0f;

	bool isStatic = false;
	bool affectedByGravity = true;

	RigidBody(Matrix3x4* p_transform) : transform(p_transform) {}
	RigidBody(Matrix3x4* p_transform, bool p_mass, bool p_isStatic)
		: transform(p_transform), mass(p_mass), isStatic(p_isStatic) {
	}
	~RigidBody() {}


	Vector3 GetPosition() const { return transform->origin; }
	Vector3 GetVelocity() const { return velocity; }

	void ApplyForce(const Vector3& p_force) { force += p_force; }
	void SetVelocity(const Vector3& p_velocity) { velocity = p_velocity; }
private:
	void Integrate(float deltaTime);

	friend class PhysicWorld;
};
#endif // ST_RIGID_BODY_H

