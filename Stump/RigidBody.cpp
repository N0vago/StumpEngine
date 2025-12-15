#include "RigidBody.h"

void RigidBody::Integrate(float deltaTime)
{
	if (isStatic) {
		return;
	}
	//Linear integration
	Vector3 acceleration = force / mass;
	velocity += acceleration * deltaTime;
	transform->origin += velocity * deltaTime;
	//Angular integration
	angularVelocity += torque / inertia * deltaTime;
	transform->Rotate(angularVelocity, angularVelocity.Length() * deltaTime);

	// Reset force
	force = Vector3(0, 0, 0);
	torque = Vector3(0, 0, 0);
}
