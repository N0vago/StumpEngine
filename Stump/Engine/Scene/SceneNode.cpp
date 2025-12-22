#include "Scene/SceneNode.h"
#include "Scene/Scene.h"
#include <iostream>
#include <algorithm>
namespace Scene {
	SceneNode::SceneNode(const ObjectInfo& r_info, STScene* p_owner) : Object(r_info), owner(p_owner)
	{
		parent = nullptr;
	}

	SceneNode::~SceneNode()
	{
		parent = nullptr;
		children.clear();
	}

	void SceneNode::SetParent(SceneNode* p_parent)
	{
		if (!owner || parent == p_parent)
			return;

		owner->ReparentNode(this, p_parent);
	}

	void SceneNode::AddChild(std::unique_ptr<SceneNode> p_child)
	{
		if (!p_child)
			return;

		p_child->parent = this;

		if (!p_child->isInTree && isInTree)
			p_child->EnterTree();
		children.push_back(std::move(p_child));
	}

	void SceneNode::RemoveChild(SceneNode* p_child)
	{

		auto it = std::find_if(children.begin(), children.end(),
			[&](const std::unique_ptr<SceneNode>& c) {
				return c.get() == p_child;
			});

		if (it == children.end())
			return;

		if ((*it)->isInTree)
			(*it)->ExitTree();

		(*it)->parent = nullptr;

		children.erase(it);
	}
	std::unique_ptr<SceneNode> SceneNode::ExtractChild(SceneNode* child) {
		auto it = std::find_if(children.begin(), children.end(),
			[child](const std::unique_ptr<SceneNode>& c)
			{
				return c.get() == child;
			});

		if (it == children.end())
			return nullptr;

		if ((*it)->isInTree)
			(*it)->ExitTree();

		(*it)->parent = nullptr;

		std::unique_ptr<SceneNode> result = std::move(*it);
		children.erase(it);

		return result;
	}

	void SceneNode::EnterTree()
	{
		if (isInTree)
			return;

		isInTree = true;

		for (auto& child : children)
		{
			child->EnterTree();
		}
	}

	void SceneNode::ExitTree()
	{
		if (!isInTree)
			return;

		isInTree = false;
		for (auto& child : children)
		{
			child->ExitTree();
		}
	}

	void SceneNode::OnEnable()
	{
		for (auto& child : children)
		{
			child->OnEnable();
		}
	}

	void SceneNode::OnDisable()
	{
		for (auto& child : children)
		{
			child->OnDisable();
		}
	}

	void SceneNode::Update(float p_deltaTime)
	{
		if (!isInTree)
			return;

		if (children.size() == 0)
			return;

		for (auto& child : children)
		{
			child->Update(p_deltaTime);
		}
	}
}