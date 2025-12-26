#pragma once
#include "Scene/Scene.h"
#include "Scene/GameObject.h"

#include "Components/MeshRendererComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ColliderComponent.h"

#include "Rendering/Shapes/SphereShape.h"
#include "Rendering/Material.h"
struct PerformanceTestConfig
{
    int objectCount = 1000;

    bool enablePhysics = false;
    bool enableColliders = false;
    bool enableGravity = false;

    float spacing = 1.5f;
};
class PerformanceSceneFactory
{
public:
    static void Create(Scene::STScene* scene,
        const PerformanceTestConfig& config,
        std::shared_ptr<Rendering::Material> material)
    {
        int gridSize = static_cast<int>(std::sqrt(config.objectCount));
        int created = 0;

        for (int x = 0; x < gridSize && created < config.objectCount; ++x)
        {
            for (int z = 0; z < gridSize && created < config.objectCount; ++z)
            {
                auto go = scene->CreateNode<Scene::GameObject>(
                    "TestObject_" + std::to_string(created));

                go->transform.Translate({
                    x * config.spacing,
                    5.0f,
                    z * config.spacing
                    });

                auto* mesh = go->AddComponent<Components::MeshRendererComponent>();
                mesh->SetMesh(std::make_shared<Rendering::SphereShape>(0.5f, 16, 8));
                mesh->SetMaterial(material);

                if (config.enableColliders)
                {
                    auto* collider = go->AddComponent<Components::SphereColliderComponent>();
                    collider->radius = 0.5f;
                }

                if (config.enablePhysics)
                {
                    auto* rb = go->AddComponent<Components::RigidBodyComponent>();
                    rb->affectedByGravity = config.enableGravity;
                    rb->isStatic = false;
                }

                created++;
            }
        }
    }
};
class PerformanceTestRunner
{
public:
    static void RunRenderOnly(Scene::STScene* scene,
        std::shared_ptr<Rendering::Material> material,
        int count)
    {
        PerformanceTestConfig cfg;
        cfg.objectCount = count;
        cfg.enablePhysics = false;
        cfg.enableColliders = false;

        PerformanceSceneFactory::Create(scene, cfg, material);
    }

    static void RunWithPhysics(Scene::STScene* scene,
        std::shared_ptr<Rendering::Material> material,
        int count)
    {
        PerformanceTestConfig cfg;
        cfg.objectCount = count;
        cfg.enablePhysics = true;
        cfg.enableColliders = true;
        cfg.enableGravity = true;

        PerformanceSceneFactory::Create(scene, cfg, material);
    }
};
struct FrameStats
{
    float deltaTime;
    float fps;
};
