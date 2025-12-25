#ifndef ST_DEFAULT_SCENE_FACTORY_H
#define ST_DEFAULT_SCENE_FACTORY_H
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Components/MeshRendererComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ColliderComponent.h"
#include "Rendering/Shapes/PlaneShape.h"
#include "Rendering/Shapes/SphereShape.h"
#include "Rendering/Shapes/CubeShape.h"
#include "Audio/AudioListener.h"

class DefaultSceneFactory {
public:
    static void Create(Scene::STScene* p_scene)
    {
		auto diffuseTexture = std::make_shared<Rendering::Texture>("Assets/planks.png", TextureType::Diffuse, 0);
		auto specularTexture = std::make_shared<Rendering::Texture>("Assets/planksSpec.png", TextureType::Specular, 1);

		Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f);
		Vector3 lightPos = Vector3(0.0f, 5.0f, 0.0f);

		auto plane = std::make_shared<Rendering::RenderUnit>();
		plane->material = std::make_shared<Rendering::Material>(std::make_shared<Rendering::Shader>("Assets/default.vert", "Assets/default.frag"));
		plane->material->SetFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z);
		plane->material->SetFloat4("lightColor", lightColor.x, lightColor.y, lightColor.z, 1);
		plane->material->SetTexture(diffuseTexture);
		plane->material->SetTexture(diffuseTexture);
		plane->mesh = std::make_shared<Rendering::PlaneShape>(10.0f, 10.0f);
		plane->modelMatrix = Matrix3x4();
		plane->modelMatrix.Translate(Vector3(0.0f, 1.0f, 0.0f));
		plane->modelMatrix.Rotate(Vector3(0.0f, MathF::ToRadians(90.0f), 0.0f));
		plane->modelMatrix.Scale(Vector3(1.f, 0.0f, 1.0f));

		auto lightSphere = std::make_shared<Rendering::RenderUnit>();
		lightSphere->material = std::make_shared<Rendering::Material>(std::make_shared<Rendering::Shader>("Assets/light.vert", "Assets/light.frag"));
		lightSphere->mesh = std::make_shared<Rendering::SphereShape>(1.0f, 18, 9);
		lightSphere->modelMatrix = Matrix3x4();
		lightSphere->modelMatrix.Translate(lightPos);
		lightSphere->material->SetFloat4("lightColor", lightColor.x, lightColor.y, lightColor.z, 1);

		auto planeInstance = p_scene->CreateNode<Scene::GameObject>("Plane");
		planeInstance->transform = plane->modelMatrix;
		auto lightSphereInstance = p_scene->CreateNode<Scene::GameObject>("Light Sphere");
		lightSphereInstance->transform = lightSphere->modelMatrix;

		auto planeMesh = planeInstance->AddComponent<Components::MeshRendererComponent>();
		planeMesh->SetMesh(plane->mesh);
		planeMesh->SetMaterial(plane->material);

		auto lightSphereMesh = lightSphereInstance->AddComponent<Components::MeshRendererComponent>();
		lightSphereMesh->SetMesh(lightSphere->mesh);
		lightSphereMesh->SetMaterial(lightSphere->material);

		auto boxCollider = planeInstance->AddComponent<Components::BoxColliderComponent>();
		boxCollider->halfExtents = Vector3(10.0f, 1.0f, 10.0f);

		auto sphereCollider = lightSphereInstance->AddComponent<Components::SphereColliderComponent>();
		sphereCollider->radius = 1.0f;

		auto planeRigidBody = planeInstance->AddComponent<Components::RigidBodyComponent>();
		planeRigidBody->affectedByGravity = false;
		planeRigidBody->isStatic = true;
		auto lightSphereRigidBody = lightSphereInstance->AddComponent<Components::RigidBodyComponent>();
    }
};


#endif //ST_DEFAULT_SCENE_FACTORY_H