#include "Components/RigidBodyComponent.h"
#include "Components/ColliderComponent.h"
#include "Physic/PhysicWorld.h"

namespace Components {
	void RigidBodyComponent::OnCreate()
	{
		body = std::make_shared<Physic::RigidBody>(
			&owner->transform
		);

		body->isStatic = isStatic;
		body->affectedByGravity = affectedByGravity;
		body->mass = mass;
	}
	void RigidBodyComponent::OnEnable()
	{
		ColliderComponent* colliderComp = owner->GetComponent<Components::ColliderComponent>();

		if (colliderComp)
		{
			body->collider = colliderComp->GetCollider();
		}
		else
		{
			body->collider = nullptr;
		}

		Physic::PhysicWorld::Get().AddRigidBody(body);
	}
	void RigidBodyComponent::OnDisable()
	{
		Physic::PhysicWorld::Get().RemoveRigidBody(body);
	}
	void RigidBodyComponent::Update(float p_deltaTime)
	{
		body->transform = &owner->transform;
	}
}