#include "EngineModeLayer.h"

#include <algorithm>

static std::vector<Vertex> GetCircleVertices(float radius)
{
	std::vector<Vertex> circleVertices;
	for (int i = 0; i < 360; ++i)
	{
		float angle = i * 2 * Math::PI / 360;
		float x = radius * Math::Cosf(angle);
		float z = radius * Math::Sinf(angle);
		circleVertices.push_back(Vertex{ Vector3(x, 0.0f, z) });
	}
	return circleVertices;
}

static std::vector<GLuint> GetCircleIndices()
{
	std::vector<GLuint> circleIndices;
	for (int i = 0; i < 360; ++i)
	{
		circleIndices.push_back(i);
		circleIndices.push_back((i + 1) % 360);
		circleIndices.push_back(360); // Center vertex
	}
	return circleIndices;
}

static std::vector<Vertex> GetSphereVertices(float radius, int sectorCount, int stackCount)
{
	std::vector<Vertex> sphereVertices;
	for (int i = 0; i <= 180; ++i)
	{
		float stackAngle = Math::PI / 2 - i * Math::PI / 180; // from pi/2 to -pi/2
		float xy = radius * cos(stackAngle); // r * cos(u)
		float z = radius * sin(stackAngle);  // r * sin(u)
		for (int j = 0; j <= 360; ++j)
		{
			float sectorAngle = j * 2 * Math::PI / 360; // from 0 to 2pi
			float x = xy * cos(sectorAngle); // r * cos(u) * cos(v)
			float y = xy * sin(sectorAngle); // r * cos(u) * sin(v)
			sphereVertices.push_back(Vertex{ Vector3(x, y, z) });
		}
	}
	return sphereVertices;
}

static std::vector<GLuint> GetSphereIndices(int sectorCount, int stackCount)
{
	std::vector<GLuint> sphereIndices;
	for (int i = 0; i < 180; ++i)
	{
		int k1 = i * (360 + 1);     // beginning of current stack
		int k2 = k1 + 360 + 1;      // beginning of next stack
		for (int j = 0; j < 360; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				sphereIndices.push_back(k1);
				sphereIndices.push_back(k2);
				sphereIndices.push_back(k1 + 1);
			}
			if (i != (180 - 1))
			{
				sphereIndices.push_back(k1 + 1);
				sphereIndices.push_back(k2);
				sphereIndices.push_back(k2 + 1);
			}
		}
	}
	return sphereIndices;
}

EngineModeLayer::EngineModeLayer() {

	float windowWidth = Core::Application::Get().GetWindow()->GetFrameBufferSize().x;
	float windowHeight = Core::Application::Get().GetWindow()->GetFrameBufferSize().y;

	camera = std::make_unique<Camera>(Camera(windowWidth, windowHeight, Vector3(0.0f, 0.0f, 2.0f)));
	renderManager = std::make_unique<RenderManager>(RenderManager(*camera));
	inputManager = std::make_unique<InputManager>(InputManager());

	Vertex vertices[] =
	{
		Vertex{Vector3(-1.0f, 0.0f,  1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)},
		Vertex{Vector3(-1.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f)},
		Vertex{Vector3(1.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f)},
		Vertex{Vector3(1.0f, 0.0f,  1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)}
	};

	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};
	Texture textures[] =
	{
		Texture("planks.png", "diffuse", 0),
		Texture("planksSpec.png", "specular", 1)
	};

	std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector<GLuint> inds(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<Texture> texs(textures, textures + sizeof(textures) / sizeof(Texture));

	std::vector<Vertex> lightVerts = GetSphereVertices(0.1f, 36, 18);
	std::vector<GLuint> lightInds = GetSphereIndices(36, 18);

	Mesh floor(verts, inds, Shader("default.vert", "default.frag"));

	floor.ApplyTexture(texs);

	Mesh lightCube(lightVerts, lightInds, Shader("light.vert", "light.frag"));

	Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f);
	Vector3 lightPos = Vector3(0.0f, 1.0f, 0.0f);

	Matrix3x4 lightMatrix;
	lightMatrix.Translate(lightPos);

	Vector3 floorPos = Vector3(0.0f, 0.0f, 0.0f);

	Matrix3x4 floorMatrix;
	floorMatrix.Translate(floorPos);
	floorMatrix.Rotate(Vector3(Math::ToRadians(90.0f), 0.0f, 0.0f));
	floorMatrix.Scale(Vector3(100.0f, 1.0f, 100.0f));

	lightCube.meshShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightCube.meshShader.ID, "model"), 1, GL_FALSE, renderManager->ToMatrix4x4(lightMatrix));
	glUniform4f(glGetUniformLocation(lightCube.meshShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, 1);
	floor.meshShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(floor.meshShader.ID, "model"), 1, GL_FALSE, renderManager->ToMatrix4x4(floorMatrix));
	glUniform4f(glGetUniformLocation(floor.meshShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, 1);
	glUniform3f(glGetUniformLocation(floor.meshShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	renderManager->AddToRender(lightCube);
	renderManager->AddToRender(floor);
}

EngineModeLayer::~EngineModeLayer()
{
}

void EngineModeLayer::OnUpdate(float p_ts)
{

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

