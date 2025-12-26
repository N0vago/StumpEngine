#ifndef ST_LIGHT_SYSTEM_H
#define ST_LIGHT_SYSTEM_H
#include <vector>
#include "Math/Vector3.h"
#include "Scene/Scene.h"
#include "Rendering/Shader.h"
namespace Rendering {
    struct GPULight
    {
        int type;
        Math::Vector3 position;
        Math::Vector3 direction;
        Math::Vector3 color;
        float intensity;
        float range;

        float innerCone;
        float outerCone;
    };
    class LightSystem
    {
    public:
        static constexpr int MaxLights = 16;
        std::vector<GPULight> lights;

        void Collect(Scene::STScene& r_scene);
        void Upload(Shader& shader);
    };

}

#endif //ST_LIGHT_SYSTEM_H