#ifndef ST_PHYSIC_WORLD_H
#define ST_PHYSIC_WORLD_H

#include "RigidBody.h"

#include <vector>
#include <memory>
class PhysicWorld
{
	std::vector<std::shared_ptr<RigidBody>> rigidBodies;
	Vector3 gravity = Vector3(0.0f, -9.81f, 0.0f);
public:
	void AddRigidBody(std::shared_ptr<RigidBody> p_rigidBody) {
		rigidBodies.push_back(p_rigidBody);
	}
	void RemoveRigidBody(std::shared_ptr<RigidBody> p_rigidBody) {
		rigidBodies.erase(std::remove(rigidBodies.begin(), rigidBodies.end(), p_rigidBody), rigidBodies.end());
	}
	void SetGravity(const Vector3& p_gravity) {
		gravity = p_gravity;
	}
	Vector3 GetGravity() const {
		return gravity;
	}
	void StepSimulation(float deltaTime);
};
#endif // ST_PHYSIC_WORLD_H


