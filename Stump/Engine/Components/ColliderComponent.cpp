#include "Components/ColliderComponent.h"
#include "Physic/PhysicWorld.h"
namespace Components {
	void ColliderComponent::OnCreate()
	{
		if (!collider)
			return;
		Matrix3x4& t = owner->transform;

		collider->SetPosition(t.origin);
	}
	void ColliderComponent::OnEnable()
	{
		if (!collider)
			return;
		Matrix3x4& t = owner->transform;

		collider->SetPosition(t.origin);
		
	}
	void ColliderComponent::OnDisable()
	{
	}
	void ColliderComponent::Update(float p_deltaTime)
	{
		if (!collider)
			return;
		Matrix3x4& t = owner->transform;

		collider->SetPosition(t.origin);
	}
}