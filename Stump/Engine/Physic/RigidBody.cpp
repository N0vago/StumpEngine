#include "Physic/RigidBody.h"

namespace Physic {
	void RigidBody::Integrate(float deltaTime)
	{
		if (isStatic) {
			return;
		}

		Vector3 acceleration = force / mass;
		velocity += acceleration * deltaTime;
		transform->origin += velocity * deltaTime;

		angularVelocity += torque / inertia * deltaTime;
		transform->Rotate(angularVelocity, angularVelocity.Length() * deltaTime);

		force = Vector3(0, 0, 0);
		torque = Vector3(0, 0, 0);
	}
}