#include "PhysicWorld.h"

static PhysicWorld* Instance = nullptr;

PhysicWorld::PhysicWorld()
{
	Instance = this;
}

PhysicWorld::~PhysicWorld()
{
	Instance = nullptr;
}

void PhysicWorld::StepSimulation(float deltaTime)
{
	for (auto& body : rigidBodies) {
		if (!body->isStatic) {
			if (body->affectedByGravity)
				body->ApplyForce(gravity * body->mass);
			body->Integrate(deltaTime);
		}
	}

	std::vector<Collision> potentialCollisions;
	BroadPhase(potentialCollisions);

	for (auto& collision : potentialCollisions) {
		NarrowPhase(collision);
	}

}

PhysicWorld& PhysicWorld::Get()
{
	return *Instance;
}

void PhysicWorld::BroadPhase(std::vector<Collision>& r_out)
{
	for (size_t i = 0; i < rigidBodies.size(); ++i) {
		for(size_t j = i + 1; j < rigidBodies.size(); ++j) {
			
			if (rigidBodies[i]->collider == nullptr || rigidBodies[j]->collider == nullptr)	
				if (rigidBodies[i]->isStatic && rigidBodies[j]->isStatic)
					continue;


			Collision collision;
			collision.bodyA = rigidBodies[i];
			collision.bodyB = rigidBodies[j];

			if (rigidBodies[i]->collider->GetAABB().IsIntersects(rigidBodies[j]->collider->GetAABB())) {
				r_out.push_back(collision);
			}
		}
	}
}

void PhysicWorld::NarrowPhase(Collision& potentialCollisions)
{
	CollisionDetector detector;
	CollisionPoints points = detector.TestCollision(
		potentialCollisions.bodyA->collider.get(),
		potentialCollisions.bodyB->collider.get()
	);
	if (points.hasCollision) {

		potentialCollisions.points = points;
		ResolveCollision(potentialCollisions);
		ApplyAngularImpulse(potentialCollisions);
		ApplyImpulse(potentialCollisions);
		if (onCollision) {
			onCollision(potentialCollisions);
		}
	}
}

void PhysicWorld::ResolveCollision(Collision& collision)
{	
	if (!collision.bodyA || !collision.bodyB)
		return;

	CollisionPoints& cp = collision.points;

	if (!cp.hasCollision || cp.penetrationDepth <= 0.0f)
		return;

	auto& bodyA = collision.bodyA;
	auto& bodyB = collision.bodyB;

	// --- Inverse mass ---
	float invMassA = bodyA->isStatic ? 0.0f : 1.0f / bodyA->mass;
	float invMassB = bodyB->isStatic ? 0.0f : 1.0f / bodyB->mass;

	// Если оба статические — ничего не делаем
	if (invMassA + invMassB == 0.0f)
		return;

	// --- Positional correction parameters ---
	const float percent = 0.7f;   // 70% исправления (PhysX style)
	const float slop = 0.001f;    // допустимая погрешность

	float penetration = std::max(cp.penetrationDepth - slop, 0.0f);

	Vector3 correction =
		cp.normal * (penetration * percent / (invMassA + invMassB));

	// --- Apply correction ---
	if (!bodyA->isStatic)
		bodyA->transform->origin -= correction * invMassA;

	if (!bodyB->isStatic)
		bodyB->transform->origin += correction * invMassB;

}

void PhysicWorld::ApplyImpulse(Collision& collision)
{
	auto& bodyA = collision.bodyA;
	auto& bodyB = collision.bodyB;
	auto& cp = collision.points;

	if (!cp.hasCollision)
		return;

	Vector3 vA = bodyA->GetVelocity();
	Vector3 vB = bodyB->GetVelocity();

	Vector3 relativeVelocity = vB - vA;
	float velocityAlongNormal = relativeVelocity.Dot(cp.normal);

	// Если тела уже расходятся — ничего не делаем
	if (velocityAlongNormal > 0.0f)
		return;

	float restitution = 0.3f; // можно вынести в PhysicsMaterial

	float invMassA = bodyA->isStatic ? 0.0f : 1.0f / bodyA->mass;
	float invMassB = bodyB->isStatic ? 0.0f : 1.0f / bodyB->mass;

	float j = -(1.0f + restitution) * velocityAlongNormal;
	j /= (invMassA + invMassB);

	Vector3 impulse = cp.normal * j;

	if (!bodyA->isStatic)
		bodyA->velocity -= impulse * invMassA;

	if (!bodyB->isStatic)
		bodyB->velocity += impulse * invMassB;
}

void PhysicWorld::ApplyAngularImpulse(Collision& collision)
{
	auto& bodyA = collision.bodyA;
	auto& bodyB = collision.bodyB;
	auto& cp = collision.points;

	Vector3 contactPoint = (cp.pointA + cp.pointB) * 0.5f;

	Vector3 rA = contactPoint - bodyA->GetPosition();
	Vector3 rB = contactPoint - bodyB->GetPosition();

	float invInertiaA = bodyA->isStatic ? 0.0f : 1.0f / bodyA->inertia;
	float invInertiaB = bodyB->isStatic ? 0.0f : 1.0f / bodyB->inertia;

	Vector3 relativeVelocity =
		(bodyB->velocity + bodyB->angularVelocity.Cross(rB)) -
		(bodyA->velocity + bodyA->angularVelocity.Cross(rA));

	float velAlongNormal = relativeVelocity.Dot(cp.normal);
	if (velAlongNormal > 0.0f)
		return;

	float restitution = 0.3f;

	float invMassA = bodyA->isStatic ? 0.0f : 1.0f / bodyA->mass;
	float invMassB = bodyB->isStatic ? 0.0f : 1.0f / bodyB->mass;

	float angularTerm =
		cp.normal.Dot(
			(rA.Cross(cp.normal) * invInertiaA).Cross(rA) +
			(rB.Cross(cp.normal) * invInertiaB).Cross(rB));

	float j = -(1.0f + restitution) * velAlongNormal;
	j /= (invMassA + invMassB + angularTerm);

	Vector3 impulse = cp.normal * j;

	if (!bodyA->isStatic)
		bodyA->angularVelocity -= rA.Cross(impulse) * invInertiaA;

	if (!bodyB->isStatic)
		bodyB->angularVelocity += rB.Cross(impulse) * invInertiaB;
	
}

