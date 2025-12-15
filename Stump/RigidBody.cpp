#include "RigidBody.h"

void RigidBody::Integrate(float deltaTime)
{
	if (isStatic) {
		return;
	}
	// a = F / m
	Vector3 acceleration = force / mass;
	// v = v0 + a * dt
	velocity += acceleration * deltaTime;
	// p = p0 + v * dt
	position += velocity * deltaTime;
	// Reset force
	force = Vector3(0, 0, 0);
}
