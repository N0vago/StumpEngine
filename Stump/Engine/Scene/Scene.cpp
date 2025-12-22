#include "Scene/Scene.h"
#include <type_traits>
namespace Scene {

	STScene::STScene() : root(std::move(std::make_unique<SceneNode>(ObjectInfo(0, "Root"), this)))
	{
		root->EnterTree();
	}

	STScene::~STScene()
	{
		root->ExitTree();
	}

	void STScene::Update(float p_deltaTime)
	{
		root->Update(p_deltaTime);
	}
	void STScene::EnableNodes() {
		root->OnEnable();
	}
	void STScene::DisableNodes() {
		root->OnDisable();
	}

	void STScene::DestroyNode(SceneNode* p_node)
	{
		if (!p_node || p_node == root.get())
			return;

		p_node->ExitTree();

		p_node->GetParent()->RemoveChild(p_node);
	}

	void STScene::ReparentNode(SceneNode* p_child, SceneNode* p_newParent)
	{
		if (!p_child || !p_newParent)
			return;

		SceneNode* oldParent = p_child->GetParent();
		if (!oldParent)
			return;

		std::shared_ptr<SceneNode> owned = oldParent->ExtractChild(p_child);
		p_newParent->AddChild(std::move(owned));
	}



	int STScene::GenerateID() {
		static int nextID = 0;
		return nextID++;
	}
}