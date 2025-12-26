#include "Rendering/LightSystem.h"
#include "Components/LightComponent.h"
#include "Scene/GameObject.h"
#include "Rendering/Shader.h"
namespace Rendering {
    void LightSystem::Collect(Scene::STScene& r_scene)
    {
        lights.clear();

        r_scene.ForEachNode([&](Scene::SceneNode& node)
            {
                if (lights.size() >= MaxLights)
                    return;

                auto* go = dynamic_cast<Scene::GameObject*>(&node);
                if (!go)
                    return;

                auto* light = go->GetComponent<Components::LightComponent>();
                if (!light)
                    return;

                GPULight gpu{};
                gpu.type = static_cast<int>(light->type);
                gpu.position = go->transform.origin;
                gpu.direction = Math::Vector3(0.0f, -1.0f, 0.0f);
                gpu.color = light->color;
                gpu.intensity = light->intensity;
                gpu.range = light->range;
                gpu.innerCone = light->innerCone;
                gpu.outerCone = light->outerCone;

                lights.push_back(gpu);
            });
    }
    void LightSystem::Upload(Shader& shader)
    {
        shader.Activate();

        const int lightCount = static_cast<int>(lights.size());
        shader.SetInt("u_LightCount", lightCount);

        for (int i = 0; i < lightCount; i++)
        {
            const GPULight& light = lights[i];
            const std::string base = "u_Lights[" + std::to_string(i) + "]";

            shader.SetInt((base + ".type").c_str(), light.type);
            shader.SetFloat3((base + ".position").c_str(),
                light.position.x, light.position.y, light.position.z);
            shader.SetFloat3((base + ".direction").c_str(),
                light.direction.x, light.direction.y, light.direction.z);
            shader.SetFloat3((base + ".color").c_str(),
                light.color.x, light.color.y, light.color.z);

            shader.SetFloat((base + ".intensity").c_str(), light.intensity);
            shader.SetFloat((base + ".range").c_str(), light.range);
            shader.SetFloat((base + ".innerCone").c_str(), light.innerCone);
            shader.SetFloat((base + ".outerCone").c_str(), light.outerCone);
        }
    }
}