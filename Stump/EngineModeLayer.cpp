#include "EngineModeLayer.h"

#include "PlaneShape.h"
#include "SphereShape.h"
#include "MeshInstance.h"
#include <memory>
#include <algorithm>

EngineModeLayer::EngineModeLayer() {

	float windowWidth = Core::Application::Get().GetWindow()->GetFrameBufferSize().x;
	float windowHeight = Core::Application::Get().GetWindow()->GetFrameBufferSize().y;

	camera = std::make_unique<Camera>(Camera(windowWidth, windowHeight, Vector3(0.0f, 0.0f, 2.0f)));
	renderManager = std::make_unique<RenderManager>(RenderManager(*camera));
	inputManager = std::make_unique<InputManager>(InputManager());
	
	sceneRoot = std::make_unique<SceneNode>(ObjectInfo(0, "Root"));

	Texture textures[] =
	{
		Texture("planks.png", "diffuse", 0),
		Texture("planksSpec.png", "specular", 1)
	};
	std::vector<Texture> texs(textures, textures + sizeof(textures) / sizeof(Texture));

	Shader defaultShader("default.vert", "default.frag");
	Shader lightShader("light.vert", "light.frag");

	PlaneShape plane(10.0f, 10.0f, defaultShader);
	SphereShape lightSphere(1.0f, 36, 18, lightShader);

	MeshInstance lightSphereInstance(ObjectInfo(1, "LightSphere"), &lightSphere);
	MeshInstance planeInstance(ObjectInfo(2, "Floor"), &plane);

	Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f);
	Vector3 lightPos = Vector3(0.0f, 1.0f, 0.0f);

	Matrix3x4 lightMatrix;
	lightMatrix.Translate(lightPos);

	Matrix3x4 floorMatrix;
	floorMatrix.Translate(Vector3(0.0f, 0.0f, 0.0f));
	floorMatrix.Rotate(Vector3(Math::ToRadians(90.0f), 0.0f, 0.0f));
	floorMatrix.Scale(Vector3(100.0f, 1.0f, 100.0f));

	plane.ApplyTexture(texs);
	

	planeInstance.SetTransform(floorMatrix);
	lightSphereInstance.SetTransform(lightMatrix);


	plane.GetShader().Activate();
	plane.GetShader().SetMat4("camMatrix", camera->cameraMatrix.matrix[0], false);
	plane.GetShader().SetMat4("model", floorMatrix.ToRenderMatrix(), false);
	plane.GetShader().SetVec3("camPos", camera->Position, false);
	plane.GetShader().SetFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z, false);
	plane.GetShader().SetFloat4("lightColor", lightColor.x, lightColor.y, lightColor.z, 1);

	lightSphere.GetShader().Activate();
	lightSphere.GetShader().SetMat4("model", lightMatrix.ToRenderMatrix(), false);
	lightSphere.GetShader().SetFloat4("lightColor", lightColor.x, lightColor.y, lightColor.z, 1);

	sceneRoot->EnterTree();

	sceneRoot->AddChild(&planeInstance);
	sceneRoot->AddChild(&lightSphereInstance);
}

EngineModeLayer::~EngineModeLayer()
{
}

void EngineModeLayer::OnUpdate(float p_ts)
{

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
	
	camera->UpdateMatrix(45.0f, 0.1f, 100.0f);
}

void EngineModeLayer::OnRender()
{
	renderManager->DrawMeshes();
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

