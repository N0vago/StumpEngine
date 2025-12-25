#include "Core/Layers/EditorModeLayer.h"
#include "Utility/DefaultSceneFactory.h"
#include "Core/Layers/GuiLayer.h"

#include <memory>
#include <algorithm>

namespace Core {

	
	EditorModeLayer::EditorModeLayer()
	{
		//Fields initialization
		inputManager = std::make_unique<InputManager>(InputManager());
		scene = Core::Engine::Get().GetScene();

		editorCamera = std::make_shared<Camera>(Camera(1280,720, Vector3(0.0f, 0.0f, 2.0f)));

		RenderManager::Get().SetActiveCamera(editorCamera);

		DefaultSceneFactory::Create(scene);

		scene->EnableNodes();
	}

	EditorModeLayer::~EditorModeLayer()
	{
		
	}

	void EditorModeLayer::OnUpdate(float p_ts)
	{

		if (Core::Engine::Get().GetLayer<Core::GuiLayer>()->GetViewportWindow()->IsActive())
		{
			CameraMove(p_ts);

			if (inputManager->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
				CameraRotate(p_ts);
		}
		if (Core::Engine::Get().GetLayer<Core::GuiLayer>()->GetViewportWindow()->IsActive() && inputManager->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
		{
			glfwSetInputMode(Core::Engine::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(Core::Engine::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		editorCamera->UpdateMatrix(90.0f, 0.1f, 100.0f);

	}

	void EditorModeLayer::OnRender()
	{
		RenderManager::Get().DrawCall();
	}

	void EditorModeLayer::OnEvent(Core::Event& event)
	{
		if (!Core::Engine::Get().GetLayer<Core::GuiLayer>()->GetViewportWindow()->IsActive())
			return;
		inputManager->ListenEvent(event);
	}
	void EditorModeLayer::CameraMove(float& r_deltaTime)
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


	void EditorModeLayer::CameraRotate(float& r_deltaTime) {

		ImGuiIO& io = ImGui::GetIO();

		Vector2 delta(io.MouseDelta.x, io.MouseDelta.y);

		float rotX = delta.y * editorCamera->sensitivity * r_deltaTime;
		float rotY = delta.x * editorCamera->sensitivity * r_deltaTime;

		Vector3 right = editorCamera->Orientation.Cross(editorCamera->Up).Normalized();

		Vector3 newOrientation =
			editorCamera->Orientation.RotatedAroundAxis(right, -rotX);

		if (MathF::Abs(newOrientation.AngleTo(editorCamera->Up) - MathF::ToRadiansf(90.0f))
			<= MathF::ToRadiansf(85.0f))
		{
			editorCamera->Orientation = newOrientation;
		}

		editorCamera->Orientation.RotateAroundAxis(editorCamera->Up, -rotY);
	}

}