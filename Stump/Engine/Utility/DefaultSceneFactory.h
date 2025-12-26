#ifndef ST_DEFAULT_SCENE_FACTORY_H
#define ST_DEFAULT_SCENE_FACTORY_H

#include "Scene/Scene.h"
#include "Scene/GameObject.h"

#include "Components/MeshRendererComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/LightComponent.h"

#include "Rendering/Shapes/PlaneShape.h"
#include "Rendering/Shapes/SphereShape.h"
#include "Rendering/Material.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"

#include "Tests/PerformanceTest.h"

class DefaultSceneFactory
{
public:
    static void Create(Scene::STScene* p_scene)
    {

        auto diffuseTexture = std::make_shared<Rendering::Texture>(
            "Assets/planks.png", TextureType::Diffuse, 0);

        auto specularTexture = std::make_shared<Rendering::Texture>(
            "Assets/planksSpec.png", TextureType::Specular, 1);

        auto defaultMaterial = std::make_shared<Rendering::Material>(
            std::make_shared<Rendering::Shader>(
                "Assets/default.vert",
                "Assets/default.frag"));

        defaultMaterial->SetTexture(diffuseTexture);
        defaultMaterial->SetTexture(specularTexture);

        auto planeGO = p_scene->CreateNode<Scene::GameObject>("Plane");

        planeGO->transform.Translate(Math::Vector3(0.0f, 1.0f, 0.0f));
        planeGO->transform.Rotate(Math::Vector3(0.0f, MathF::ToRadians(90.0f), 0.0f));
        planeGO->transform.Scale(Math::Vector3(10.0f, 1.0f, 10.0f));

        auto* planeMesh = planeGO->AddComponent<Components::MeshRendererComponent>();
        planeMesh->SetMesh(std::make_shared<Rendering::PlaneShape>(10.0f, 10.0f));
        planeMesh->SetMaterial(defaultMaterial);

        auto* planeCollider = planeGO->AddComponent<Components::BoxColliderComponent>();
        planeCollider->halfExtents = { 10.0f, 1.0f, 10.0f };

        auto* planeRB = planeGO->AddComponent<Components::RigidBodyComponent>();
        planeRB->isStatic = true;
        planeRB->affectedByGravity = false;

        auto lightGO = p_scene->CreateNode<Scene::GameObject>("Point Light");
        lightGO->transform.Translate({ 0.0f, 5.0f, 0.0f });

        auto* light = lightGO->AddComponent<Components::LightComponent>();
        light->type = Components::LightType::Directional;
        light->color = { 1.0f, 1.0f, 1.0f };
        light->intensity = 0.5f;
        light->range = 15.0f;

        auto lightDebugMaterial = std::make_shared<Rendering::Material>(
            std::make_shared<Rendering::Shader>(
                "Assets/light.vert",
                "Assets/light.frag"));

        lightDebugMaterial->SetFloat4(
            "lightColor", 1.0f, 1.0f, 1.0f, 1.0f);

        auto* lightMesh = lightGO->AddComponent<Components::MeshRendererComponent>();
        lightMesh->SetMesh(
            std::make_shared<Rendering::SphereShape>(0.5f, 16, 8));
        lightMesh->SetMaterial(lightDebugMaterial);

        auto* lightCollider = lightGO->AddComponent<Components::SphereColliderComponent>();
        lightCollider->radius = 0.5f;

        auto* lightRB = lightGO->AddComponent<Components::RigidBodyComponent>();
        lightRB->affectedByGravity = false;

        PerformanceTestRunner::RunRenderOnly(p_scene, lightDebugMaterial, 10);
    }
};

#endif // ST_DEFAULT_SCENE_FACTORY_H
