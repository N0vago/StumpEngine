#ifndef ST_LIGHT_COMPONENT_H
#define ST_LIGHT_COMPONENT_H
#include "Components/Component.h"
#include "Math/Vector3.h"
namespace Components {
    enum class LightType
    {
        Directional,
        Point,
        Spot
    };

    class LightComponent : public Component
    {
    public:
        LightType type = LightType::Point;

        Math::Vector3 color = { 1.0f, 1.0f, 1.0f };
        float intensity = 1.0f;
        float range = 10.0f;

        float innerCone = 0.9f;
        float outerCone = 0.8f;

        bool castsShadows = false;

        LightComponent()
            : Component(ComponentInfo{
                "Light",
                "Rendering",
                false,
                true
                })
        {
        }

        ComponentInfo GetInfo() const override
        {
            return info;
        }
    };
}

#endif //ST_LIGHT_COMPONENT_H