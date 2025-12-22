#ifndef ST_RIGID_BODY_COMPONENT_H
#define ST_RIGID_BODY_COMPONENT_H

#include "Components/Component.h"
#include "Physic/RigidBody.h"

#include <memory>
namespace Components {
	class RigidBodyComponent : public Component {
    public:
        std::shared_ptr<Physic::RigidBody> body;

        bool isStatic = false;
        bool affectedByGravity = true;
        float mass = 1.0f;


        void OnCreate() override;
        void OnEnable() override;
        void OnDisable() override;

        void Update(float p_deltaTime) override;
	};
}
#endif //ST_RIGID_BODY_COMPONENT_H