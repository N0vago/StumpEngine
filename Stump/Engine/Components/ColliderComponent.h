#ifndef ST_COLLIDER_COMPONENT_H
#define ST_COLLIDER_COMPONENT_H

#include "Components/Component.h"
#include "Physic/Collider.h"

#include <memory>
namespace Components {
	class ColliderComponent : public Component {
	protected:
		std::shared_ptr<Physic::Collider> collider;
	public:
		virtual ~ColliderComponent() = default;

        std::shared_ptr<Physic::Collider> GetCollider() const { return std::move(collider); }
		
		ComponentInfo GetInfo() const override {
			return {
				"Collider",
				"Physics",
				false,
				true
			};
		}

        void OnCreate() override;
		void OnEnable() override;
		void OnDisable() override;

		void Update(float p_deltaTime) override;

	};

    class BoxColliderComponent : public ColliderComponent
    {
    public:
        Vector3 halfExtents{ 0.5f, 0.5f, 0.5f };

        BoxColliderComponent()
        {
            info = ComponentInfo{ 
                    "BoxCollider",
                    "Physics",
                    false,
                    true
            };
            collider = std::make_shared<Physic::BoxCollider>(
                Math::Vector3(),
                halfExtents,
                Math::Matrix3x3()
            );
        }

        ComponentInfo GetInfo() const override
        {
            return {
                "BoxCollider",
                "Physics",
                false,
                true
            };
        }

        void Update(float dt) override
        {
            ColliderComponent::Update(dt);
            auto* box = static_cast<Physic::BoxCollider*>(collider.get());
            box->halfExtents = halfExtents;

        }
    };

    class SphereColliderComponent : public ColliderComponent
    {
    public:
        float radius = 0.5f;

        SphereColliderComponent()
        {
            info = ComponentInfo{
                "SphereCollider",
                "Physics",
                false,
                true
            };
            collider = std::make_shared<Physic::SphereCollider>(
                Vector3(),
                radius
            );
        }

        ComponentInfo GetInfo() const override
        {
            return {
                "SphereCollider",
                "Physics",
                false,
                true
            };
        }

        void Update(float dt) override
        {
            auto* sphere = static_cast<Physic::SphereCollider*>(collider.get());
            sphere->radius = radius;

            ColliderComponent::Update(dt);
        }
    };

    class PlaneColliderComponent : public ColliderComponent
    {
    public:
        Vector3 normal{ 0, 1, 0 };
        float distance = 0.0f;

        PlaneColliderComponent()
        {
            info = ComponentInfo{
                "PlaneCollider",
                "Physics",
                false,
                true
            };
            collider = std::make_shared<Physic::PlaneCollider>(normal, distance);
        }

        ComponentInfo GetInfo() const override
        {
            return {
                "PlaneCollider",
                "Physics",
                false,
                true
            };
        }

        void Update(float dt) override
        {
            auto* plane = static_cast<Physic::PlaneCollider*>(collider.get());
            plane->normal = normal;
            plane->distance = distance;

            ColliderComponent::Update(dt);
        }
    };
}
#endif //ST_COLLIDER_COMPONENT_H