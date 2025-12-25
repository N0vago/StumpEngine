#ifndef ST_SCENE_NODE_H
#define ST_SCENE_NODE_H
#include "Scene/Object.h"
#include "Math/Matrix3x4.h"

#include <memory>
#include <vector>


namespace Scene {
	class STScene;
	class SceneNode : public Object
	{
	protected:
		STScene* owner;
		SceneNode* parent;
	public:
		std::vector<std::shared_ptr<SceneNode>> children;

		Math::Matrix3x4 transform;

		SceneNode(const ObjectInfo& r_info, STScene* p_owner);
		virtual ~SceneNode();

		void SetParent(SceneNode* p_parent);
		SceneNode* GetParent() { return parent; }

		void AddChild(std::shared_ptr<SceneNode> p_child);
		void RemoveChild(SceneNode* p_child);

		std::shared_ptr<SceneNode> ExtractChild(SceneNode* child);

		void EnterTree();
		void ExitTree();

		virtual void OnEnable();

		virtual void OnDisable();

		virtual void Update(float p_deltaTime);

	public:
		bool isInTree = false;
	};
}
#endif // ST_SCENE_NODE_H

