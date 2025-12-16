#include "GameObject.h"


template<typename T, typename... Args>
T& GameObject::AddComponent(Args&&... args)
{
	static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

	auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
	T& ref = *ptr;

	ptr->SetOwner(this); 
	components.emplace_back(std::move(ptr));

	typeMap[typeid(T)] = &ref;

	ref.OnAttach();
	dirtyBuild = true;
	return ref;
}

template<typename T>
T* GameObject::GetComponent()
{
	static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
	auto it = typeMap.find(typeid(T));
	if (it == typeMap.end()) return nullptr;
	return static_cast<T*>(it->second);
}
template<typename T>
const T* GameObject::GetComponent() const
{
	static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
	auto it = typeMap.find(typeid(T));
	if (it == typeMap.end()) return nullptr;
	return static_cast<const T*>(it->second);
}

template<typename T>
void GameObject::RemoveComponent()
{
	static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
	auto it = std::remove_if(components.begin(), components.end(),
		[](const std::unique_ptr<Component>& c) {
			return dynamic_cast<T*>(c.get()) != nullptr;
		});
	if (it != components.end())
	{
		it->get()->Destroy();
		components.erase(it, components.end());
		typeMap.erase(typeid(T));
		dirtyBuild = true;
	}
}

void GameObject::Build()
{
	if (!dirtyBuild && isBuilt) return;

	if (!isBuilt)
	{
		for (auto& c : components) c->Awake();
		for (auto& c : components) c->Start();
		isBuilt = true;
	}

	dirtyBuild = false;
}

void GameObject::Update(float p_deltaTime)
{
	SceneNode::Update(p_deltaTime);
	Build();

	for (auto& c : components)
		if (c->IsEnabled())
			c->Update(p_deltaTime);
}

void GameObject::Render()
{
	for (auto& c : components)
		if (c->IsEnabled() && isInTree)
			c->Render();

}
