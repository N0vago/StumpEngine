#ifndef ST_COLLIDER_H
#define ST_COLLIDER_H

#include "Math/Matrix3x4.h"
#include "Math/Vector3.h"
#include "Math/AABB.h"
#include <memory>
using namespace Math;
namespace Physic {
	enum ColliderType {
		COLLIDER_SPHERE,
		COLLIDER_BOX,
		COLLIDER_PLANE
	};

	struct CollisionPoints {
		Math::Vector3 pointA;
		Math::Vector3 pointB;
		Math::Vector3 normal;
		num_fd penetrationDepth;
		bool hasCollision;
	};

	class Collider
	{
	public:
		virtual ColliderType GetColliderType() const = 0;
		virtual AABB GetAABB() const = 0;

	};

	class CollisionDetector
	{
		using FindContactFunc = CollisionPoints(*)(const Collider*, const Collider*);

	public:

		CollisionPoints TestCollision(const Collider* p_collider1, const Collider* p_collider2);

	private:
		static CollisionPoints TestSphereSphere(const Collider* p_collider1, const Collider* p_collider2);
		static CollisionPoints TestSpherePlane(const Collider* p_collider1, const Collider* p_collider2);
		static CollisionPoints TestSphereBox(const Collider* p_collider1, const Collider* p_collider2);
		static CollisionPoints TestBoxBox(const Collider* p_collider1, const Collider* p_collider2);
		static CollisionPoints TestBoxPlane(const Collider* p_collider1, const Collider* p_collider2);
		static CollisionPoints TestPlanePlane(const Collider* p_collider1, const Collider* p_collider2);

	};

	class SphereCollider : public Collider
	{

	public:
		Math::Vector3 center;
		float radius;
		SphereCollider(Math::Vector3 p_center, float p_radius)
			: center(p_center), radius(p_radius) {
		}
		ColliderType GetColliderType() const override
		{
			return COLLIDER_SPHERE;
		}
		virtual AABB GetAABB() const override;
	};
	class BoxCollider : public Collider
	{
	public:
		Math::Vector3 center;
		Math::Vector3 halfExtents;
		Matrix3x3 orientation;

		BoxCollider(Math::Vector3 p_center, Math::Vector3 p_halfExtents, Matrix3x3 p_orientation)
			: center(p_center), halfExtents(p_halfExtents), orientation(p_orientation) {
		}
		ColliderType GetColliderType() const override
		{
			return COLLIDER_BOX;
		}
		virtual AABB GetAABB() const override;
	};
	class PlaneCollider : public Collider
	{
	public:
		Math::Vector3 normal;
		num_fd distance;

		PlaneCollider(Math::Vector3 p_normal, num_fd p_distance)
			: normal(p_normal), distance(p_distance) {
		}
		ColliderType GetColliderType() const override
		{
			return COLLIDER_PLANE;
		}
		virtual AABB GetAABB() const override;
	};

}
#endif // ST_COLLIDER_H

