#include "Scene/GameObject.h"
#include "Components/Component.h"
namespace Scene {
	GameObject::GameObject(const ObjectInfo& r_info, STScene* p_owner) : SceneNode(r_info, p_owner)
	{
	}
	GameObject::~GameObject()
	{

	}
	
	void GameObject::RemoveComponent(Components::Component* p_component)
	{
		if (!p_component)
			return;

		auto it = std::find_if(
			components.begin(),
			components.end(),
			[p_component](const std::shared_ptr<Components::Component>& c)
			{
				return c.get() == p_component;
			});

		if (it == components.end())
			return;

		
		(*it)->OnDestroy();

		components.erase(it);
	}
	void GameObject::OnEnable()
	{
		SceneNode::OnEnable();
		for (auto& comp : components)
			comp->OnEnable();
	}
	void GameObject::OnDisable()
	{
		SceneNode::OnDisable();
		for (auto& comp : components)
			comp->OnDisable();
	}
	void GameObject::Update(float p_deltaTime)
	{
		SceneNode::Update(p_deltaTime);
		for (auto& comp : components)
			comp->Update(p_deltaTime);
	}
}