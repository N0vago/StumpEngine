#include "EngineModeLayer.h"

#include "PlaneShape.h"
#include "SphereShape.h"
#include "MeshInstance.h"
#include <memory>
#include <algorithm>

EngineModeLayer::EngineModeLayer() : defaultShader(Shader("default.vert", "default.frag")), lightShader(Shader("light.vert", "light.frag")) {

	float windowWidth = Core::Application::Get().GetWindow()->GetFrameBufferSize().x;
	float windowHeight = Core::Application::Get().GetWindow()->GetFrameBufferSize().y;

	camera = std::make_unique<Camera>(Camera(windowWidth, windowHeight, Vector3(0.0f, 0.0f, 2.0f)));
	inputManager = std::make_unique<InputManager>(InputManager());
	sceneRoot = std::make_unique<SceneNode>(ObjectInfo(0, "Root"));

	Texture textures[] =
	{
		Texture("planks.png", "diffuse", 0),
		Texture("planksSpec.png", "specular", 1)
	};
	std::vector<Texture> texs(textures, textures + sizeof(textures) / sizeof(Texture));


	// Create Shapes
	auto plane = std::make_shared<PlaneShape>(10.0f, 10.0f, defaultShader);
	auto sphere = std::make_shared<SphereShape>(1.0f, 18, 9, defaultShader);
	auto lightSphere = std::make_shared<SphereShape>(1.0f, 18, 9, lightShader);

	// Create Mesh Instances
	auto planeInstance = std::make_unique<MeshInstance>(ObjectInfo(1, "Floor"), plane, *camera);
	auto sphereInstance = std::make_unique<MeshInstance>(ObjectInfo(2, "Sphere"), sphere, *camera);
	auto lightSphereInstance = std::make_unique<MeshInstance>(ObjectInfo(3, "LightSphere"), lightSphere, *camera);

	RigidBody floorBody(&planeInstance->transform, 0.0f, true);
	floorBody.collider = std::make_unique<PlaneCollider>(Vector3(0.0f, 1.0f, 0.0f), 0.0f);

	RigidBody sphereBody(&sphereInstance->transform, 1.0f, false);
	sphereBody.collider = std::make_unique<SphereCollider>(sphereInstance->transform.origin, 1.0f);

	planeInstance->transform.Translate(Vector3(0.0f, 0.0f, 0.0f));
	planeInstance->transform.Rotate(Vector3(0.0f, Math::ToRadians(90.0f), 0.0f));
	planeInstance->transform.Scale(Vector3(10.f, 0.0f, 10.0f));

	sphereInstance->transform.Translate(Vector3(0.0f, 2.5f, 0.0f));
	
	Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f);
	Vector3 lightPos = Vector3(0.0f, 5.0f, 0.0f);

	lightSphereInstance->transform.Translate(lightPos);

	// Apply Textures and Set Shader Uniforms
	plane->ApplyTexture(texs);
	plane->GetShader().Activate();
	plane->GetShader().SetFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z, true);
	plane->GetShader().SetFloat4("lightColor", lightColor.x, lightColor.y, lightColor.z, 1, true);

	sphere->GetShader().Activate();

	lightSphere->GetShader().Activate();
	lightSphere->GetShader().SetMat4("model", lightSphereInstance->transform.ToRenderMatrix().data(), true, true);
	lightSphere->GetShader().SetFloat4("lightColor", lightColor.x, lightColor.y, lightColor.z, 1, true);

	// Enter Scene Tree
	sceneRoot->EnterTree();
	sceneRoot->AddChild(std::move(planeInstance));
	sceneRoot->AddChild(std::move(sphereInstance));
	sceneRoot->AddChild(std::move(lightSphereInstance));

	// Add Rigid Bodies to Physic World
	PhysicWorld::Get().AddRigidBody(std::make_shared<RigidBody>(floorBody));
	PhysicWorld::Get().AddRigidBody(std::make_shared<RigidBody>(sphereBody));
}

EngineModeLayer::~EngineModeLayer()
{
}

void EngineModeLayer::OnUpdate(float p_ts)
{
	PhysicWorld::Get().StepSimulation(p_ts);

	sceneRoot->Update(p_ts);

	CameraMove(p_ts);

	if (inputManager->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		CameraRotate(p_ts);
	}
	else {
		glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		camera->firstClick = true;
	}
	
	camera->UpdateMatrix(90.0f, 0.1f, 100.0f);
}

void EngineModeLayer::OnRender()
{
	RenderManager::Get().DrawMeshes();
}

void EngineModeLayer::OnEvent(Core::Event& event)
{
	inputManager->ListenEvent(event);
}
void EngineModeLayer::CameraMove(float& r_deltaTime)
{
	Vector3& camPos = camera->Position;
	Vector3& camOrientation = camera->Orientation;
	Vector3& camUp = camera->Up;
	float& camSpeed = camera->speed;

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
		camera->speed = 5.0f;
	}
}


void EngineModeLayer::CameraRotate(float& r_deltaTime) {

	GLFWwindow* window = Core::Application::Get().GetWindow()->GetHandle();
	Vector2 frameBuffer = Core::Application::Get().GetWindow()->GetFrameBufferSize();

	float width = frameBuffer.y;
	float height = frameBuffer.x;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (camera->firstClick)
	{

		glfwSetCursorPos(window, (width / 2), (height / 2));
		camera->firstClick = false;
	}

	float rotX = camera->sensitivity * (float)(inputManager->GetMousePosition().y - (height / 2)) / height * r_deltaTime;
	float rotY = camera->sensitivity * (float)(inputManager->GetMousePosition().x - (width / 2)) / width * r_deltaTime;

	Vector3 newOrientation = camera->Orientation.RotatedAroundAxis(camera->Orientation.Cross(camera->Up).Normalized(), -rotX);

	if (Math::Abs(newOrientation.AngleTo(camera->Up) - Math::ToRadiansf(90.0f)) <= Math::ToRadiansf(85.0f))
	{
		camera->Orientation = newOrientation;
	}

	camera->Orientation.RotateAroundAxis(camera->Up, -rotY);

	glfwSetCursorPos(window, (width / 2), (height / 2));
}

