#include "Components/MeshRendererComponent.h"

namespace Components {
	MeshRendererComponent::MeshRendererComponent() : Component(ComponentInfo{ "MeshRenderer", "Rendering", false, true })
	{
	}

	MeshRendererComponent::~MeshRendererComponent()
	{
		if (registered)
		{
			Rendering::RenderManager::Get().RemoveFromRender(&renderUnit);
			registered = false;
		}
	}

	void MeshRendererComponent::OnEnable()
	{
		if (registered)
			return;

		Rendering::RenderManager::Get().AddToRender(&renderUnit);
		registered = true;
	}

	void MeshRendererComponent::OnDisable()
	{
		if (!registered)
			return;

		Rendering::RenderManager::Get().RemoveFromRender(&renderUnit);
		registered = false;
	}

	void MeshRendererComponent::Update(float p_deltaTime)
	{
		renderUnit.modelMatrix = owner->transform;
	}

	void MeshRendererComponent::SetMesh(std::shared_ptr<Rendering::Mesh> mesh)
	{
		renderUnit.mesh = std::move(mesh);
	}

	void MeshRendererComponent::SetMaterial(std::shared_ptr<Rendering::Material> material)
	{
		renderUnit.material = std::move(material);
	}
}