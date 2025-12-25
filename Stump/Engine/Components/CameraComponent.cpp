#include "Components/CameraComponent.h"
#include "Core/Engine.h"
#include "Rendering/RenderManager.h"
namespace Components {

	CameraComponent::CameraComponent() : Component(ComponentInfo{ .name = "Camera", .category = "Camera", .allowMultiple = false, .removable = true })
	{
		float windowWidth = Core::Engine::Get().GetWindow()->GetFrameBufferSize().x;
		float windowHeight = Core::Engine::Get().GetWindow()->GetFrameBufferSize().y;

		camera = std::make_shared<Rendering::Camera>(windowWidth, windowHeight, Vector3(0.0f, 0.0f, 2.0f));
		FOVdeg = 90.0f;
		nearPlane = 0.1f;
		farPlane = 100.0f;
	}

	CameraComponent::~CameraComponent()
	{
	}

	void CameraComponent::OnEnable()
	{
		Rendering::RenderManager::Get().SetActiveCamera(camera);
	}
	void CameraComponent::OnDisable() {

	}

	void CameraComponent::Update(float p_deltaTime)
	{
		if (!owner || !camera)
			return;

		camera->Position = owner->transform.origin;
		camera->UpdateMatrix(FOVdeg, nearPlane, farPlane);
	}
}