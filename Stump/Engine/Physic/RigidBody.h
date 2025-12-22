#ifndef ST_RIGID_BODY_H
#define ST_RIGID_BODY_H
#include "Math/Vector3.h"
#include "Physic/Collider.h"
namespace Physic {
	class RigidBody
	{
	public:
		Matrix3x4* transform;
		std::shared_ptr<Collider> collider;

		Math::Vector3 velocity;
		Math::Vector3 angularVelocity;
		Math::Vector3 force;
		Math::Vector3 torque;

		float mass = 1.0f;
		float inertia = 1.0f;

		bool isStatic = false;
		bool affectedByGravity = true;

		RigidBody(Matrix3x4* p_transform) : transform(p_transform) {}
		RigidBody(Matrix3x4* p_transform, bool p_mass, bool p_isStatic)
			: transform(p_transform), mass(p_mass), isStatic(p_isStatic) {
		}
		~RigidBody() {}

		void Integrate(float deltaTime);

		Math::Vector3 GetPosition() const { return transform->origin; }
		Math::Vector3 GetVelocity() const { return velocity; }

		void ApplyForce(const Math::Vector3& p_force) { force += p_force; }
		void SetVelocity(const Math::Vector3& p_velocity) { velocity = p_velocity; }

	};
}
#endif // ST_RIGID_BODY_H

