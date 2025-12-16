#ifndef ST_GAME_OBJECT_H
#define ST_GAME_OBJECT_H
#include "SceneNode.h"
#include "Component.h"

#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <cassert>
class GameObject : public SceneNode
{
private:
	std::vector<std::unique_ptr<Component>> components;
	std::unordered_map<std::type_index, Component*> componentMap;

	bool isBuilt = false;
	bool dirtyBuild = false;
public:
	GameObject(const ObjectInfo& p_info) : SceneNode(p_info) {}
	~GameObject() {}

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args);

	template<typename T>
	T* GetComponent();

	template<typename T>
	const T* GetComponent() const;

	template<typename T>
	void RemoveComponent();

	void Build();

	virtual void Update(float p_deltaTime) override;

	void Render();


};
#endif //ST_GAME_OBJECT_H

