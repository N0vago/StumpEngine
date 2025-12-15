#include "PhysicWorld.h"

void PhysicWorld::StepSimulation(float deltaTime)
{
	for (auto& body : rigidBodies) {
		if (!body->isStatic) {
			body->ApplyForce(gravity * body->mass);
		}
		body->Integrate(deltaTime);
	}
}

void PhysicWorld::ResolveCollisions(float deltaTime)
{
	
}
