#include "SceneNode.h"

SceneNode::SceneNode(const ObjectInfo& p_info) : Object(p_info)
{
}

SceneNode::~SceneNode()
{
	parent.reset();
	children.clear();
}

void SceneNode::SetParent(SceneNode* p_parent)
{
	if(parent.get() == p_parent)
		return;
	if(parent)
		parent->RemoveChild(this);
	if(p_parent)
		p_parent->AddChild(this);
}

void SceneNode::AddChild(SceneNode* p_child)
{
	p_child->SetParent(this);
	if(isInTree)
		p_child->EnterTree();
	children.push_back(std::move(std::make_unique<SceneNode>(p_child)));
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

	(*it)->SetParent(nullptr);

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

	for (auto& child : children)
	{
		child->Update(p_deltaTime);
	}
}
