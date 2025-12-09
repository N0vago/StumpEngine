#include "SceneNode.h"
#include <algorithm>
SceneNode::SceneNode(const ObjectInfo& p_info) : Object(p_info)
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
	/*if(parent == p_parent)
		return;
	if(parent)
		parent->RemoveChild(this);
	if(p_parent)
		p_parent->AddChild(std::move(std::make_unique<SceneNode>(this)));*/
	parent = p_parent;
}

void SceneNode::AddChild(std::unique_ptr<SceneNode> p_child)
{
	if (!p_child)
		return;

	p_child->parent = this;

	if (!p_child->isInTree)
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

void SceneNode::EnterTree()
{
	if(isInTree)
		return;

	OnAwake();

	isInTree = true;

	for (auto& child : children)
	{
		child->EnterTree();
	}
}

void SceneNode::ExitTree()
{
	if(!isInTree)
		return;

	OnSleep();

	isInTree = false;
	for (auto& child : children)
	{
		child->ExitTree();
	}
}

void SceneNode::OnAwake()
{
	std::cout << "SceneNode " << info.name << " awakened." << std::endl;
}

void SceneNode::OnSleep()
{
	std::cout << "SceneNode " << info.name << " slept." << std::endl;
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
