#include "Core/Layers/EngineModeLayer.h"

#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Components/MeshRendererComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/ColliderComponent.h"
#include "Rendering/Shapes/PlaneShape.h"
#include "Rendering/Shapes/SphereShape.h"
#include "Rendering/Shapes/CubeShape.h"
#include "Audio/AudioListener.h"

#include <memory>
#include <algorithm>

namespace Core {

	
	EngineModeLayer::EngineModeLayer() : 
		defaultShader(std::make_shared<Shader>("Resources/default.vert", "Resources/default.frag")),
		lightShader(std::make_shared<Shader>("Resources/light.vert", "Resources/light.frag"))
	{
		float windowWidth = Core::Application::Get().GetWindow()->GetFrameBufferSize().x;
		float windowHeight = Core::Application::Get().GetWindow()->GetFrameBufferSize().y;

		editorCamera = std::make_shared<Camera>(Camera(windowWidth, windowHeight, Vector3(0.0f, 0.0f, 2.0f)));
		RenderManager::Get().SetActiveCamera(editorCamera);
		inputManager = std::make_unique<InputManager>(InputManager());
		scene = std::make_unique<STScene>();

		auto diffuseTexture = std::make_shared<Texture>("Resources/planks.png", TextureType::Diffuse, 0);
		auto specularTexture = std::make_shared<Texture>("Resources/planksSpec.png", TextureType::Specular, 1);

		Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f);
		Vector3 lightPos = Vector3(0.0f, 5.0f, 0.0f);

		auto plane = std::make_shared<RenderUnit>();
		plane->material = std::make_shared<Material>(defaultShader);
		plane->material->SetFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z);
		plane->material->SetFloat4("lightColor", lightColor.x, lightColor.y, lightColor.z, 1);
		plane->material->SetTexture(diffuseTexture);
		plane->material->SetTexture(diffuseTexture);
		plane->mesh = std::make_shared<PlaneShape>(10.0f, 10.0f);
		plane->modelMatrix = Matrix3x4();
		plane->modelMatrix.Translate(Vector3(0.0f, 1.0f, 0.0f));
		plane->modelMatrix.Rotate(Vector3(0.0f, MathF::ToRadians(90.0f), 0.0f));
		plane->modelMatrix.Scale(Vector3(1.f, 0.0f, 1.0f));

		auto lightSphere = std::make_shared<RenderUnit>();
		lightSphere->material = std::make_shared<Material>(lightShader);
		lightSphere->mesh = std::make_shared<SphereShape>(1.0f, 18, 9);
		lightSphere->modelMatrix = Matrix3x4();
		lightSphere->modelMatrix.Translate(lightPos);
		lightSphere->material->SetFloat4("lightColor", lightColor.x, lightColor.y, lightColor.z, 1);

		auto planeInstance = scene->CreateNode<GameObject>("Plane");
		planeInstance->transform = plane->modelMatrix;
		auto lightSphereInstance = scene->CreateNode<GameObject>("Light Sphere");
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
		
		scene->EnableNodes();
	}

	EngineModeLayer::~EngineModeLayer()
	{
	}

	void EngineModeLayer::OnUpdate(float p_ts)
	{
		PhysicWorld::Get().StepSimulation(p_ts);

		scene->Update(p_ts);

		CameraMove(p_ts);


		if (inputManager->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
		{
			CameraRotate(p_ts);
		}
		else {
			glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			editorCamera->firstClick = true;
		}

		editorCamera->UpdateMatrix(90.0f, 0.1f, 100.0f);

		Audio::AudioListener* listener = Audio::AudioListener::Get();
		listener->SetPosition(editorCamera->Position);
		listener->SetOrientation(editorCamera->Orientation, editorCamera->Up);

	}

	void EngineModeLayer::OnRender()
	{
		RenderManager::Get().Draw();
	}

	void EngineModeLayer::OnEvent(Core::Event& event)
	{
		inputManager->ListenEvent(event);
	}
	void EngineModeLayer::CameraMove(float& r_deltaTime)
	{
		Vector3& camPos = editorCamera->Position;
		Vector3& camOrientation = editorCamera->Orientation;
		Vector3& camUp = editorCamera->Up;
		float& camSpeed = editorCamera->speed;

		if (inputManager->IsKeyPressed(GLFW_KEY_W))
		{
			camPos += camOrientation * camSpeed * r_deltaTime;
		}
		if (inputManager->IsKeyPressed(GLFW_KEY_A))
		{
			camPos += -camOrientation.Cross(camUp).Normalized() * camSpeed * r_deltaTime;
		}
		if (inputManager->IsKeyPressed(GLFW_KEY_S))
		{
			camPos += -camOrientation * camSpeed * r_deltaTime;
		}
		if (inputManager->IsKeyPressed(GLFW_KEY_D))
		{
			camPos += camOrientation.Cross(camUp).Normalized() * camSpeed * r_deltaTime;
		}
		if (inputManager->IsKeyPressed(GLFW_KEY_SPACE))
		{
			camPos += camUp * camSpeed * r_deltaTime;
		}
		if (inputManager->IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			camPos += -camUp * camSpeed * r_deltaTime;
		}
		if (inputManager->IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			camSpeed = 10.0f;
		}
		if (!inputManager->IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			editorCamera->speed = 5.0f;
		}
	}


	void EngineModeLayer::CameraRotate(float& r_deltaTime) {

		GLFWwindow* window = Core::Application::Get().GetWindow()->GetHandle();
		Vector2 frameBuffer = Core::Application::Get().GetWindow()->GetFrameBufferSize();

		float width = frameBuffer.y;
		float height = frameBuffer.x;

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (editorCamera->firstClick)
		{

			glfwSetCursorPos(window, (width / 2), (height / 2));
			editorCamera->firstClick = false;
		}

		float rotX = editorCamera->sensitivity * (float)(inputManager->GetMousePosition().y - (height / 2)) / height * r_deltaTime;
		float rotY = editorCamera->sensitivity * (float)(inputManager->GetMousePosition().x - (width / 2)) / width * r_deltaTime;

		Vector3 newOrientation = editorCamera->Orientation.RotatedAroundAxis(editorCamera->Orientation.Cross(editorCamera->Up).Normalized(), -rotX);

		if (MathF::Abs(newOrientation.AngleTo(editorCamera->Up) - MathF::ToRadiansf(90.0f)) <= MathF::ToRadiansf(85.0f))
		{
			editorCamera->Orientation = newOrientation;
		}

		editorCamera->Orientation.RotateAroundAxis(editorCamera->Up, -rotY);

		glfwSetCursorPos(window, (width / 2), (height / 2));
	}

}