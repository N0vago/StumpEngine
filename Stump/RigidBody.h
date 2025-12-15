#ifndef ST_RIGID_BODY_H
#define ST_RIGID_BODY_H

#include "Matrix3x4.h"
#include "Vector3.h"
class RigidBody
{
	Matrix3x4& transform;
	Vector3 position;
	Vector3 velocity;
	Vector3 force;
public:

	float mass = 1.0f;
	bool isStatic = false;

	RigidBody(Matrix3x4& r_transform) : transform(r_transform) {}
	RigidBody(Matrix3x4& r_transform, bool p_mass, bool p_isStatic)
		: transform(r_transform), mass(p_mass), isStatic(p_isStatic) {
	}

	void Integrate(float deltaTime);

	Vector3 GetPosition() const { return position; }
	Vector3 GetVelocity() const { return velocity; }

	void ApplyForce(const Vector3& p_force) { force += p_force; }
	void SetVelocity(const Vector3& p_velocity) { velocity = p_velocity; }
};
#endif // ST_RIGID_BODY_H

