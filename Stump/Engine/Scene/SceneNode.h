#ifndef ST_SCENE_NODE_H
#define ST_SCENE_NODE_H
#include "Scene/Object.h"
#include "Math/Matrix3x4.h"

#include <memory>
#include <vector>

namespace Scene {
	class SceneNode : public Object
	{
		SceneNode* parent;
		std::vector<std::unique_ptr<SceneNode>> children;
	public:

		Math::Matrix3x4 transform;

		SceneNode(const ObjectInfo& p_info);
		~SceneNode();

		void SetParent(SceneNode* p_parent);

		void AddChild(std::unique_ptr<SceneNode> p_child);
		void RemoveChild(SceneNode* p_child);

		void EnterTree();
		void ExitTree();

		virtual void OnAwake();

		virtual void OnSleep();

		virtual void Update(float p_deltaTime);

	public:
		bool isInTree = false;
	};
}
#endif // ST_SCENE_NODE_H

