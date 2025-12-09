#ifndef ST_SCENE_NODE_H
#define ST_SCENE_NODE_H
#include "Object.h"
#include "Matrix3x4.h"

#include <memory>
#include <vector>

class SceneNode : public Object
{
	std::unique_ptr<SceneNode> parent;
	std::vector<std::unique_ptr<SceneNode>> children;
	Matrix3x4 transform;

public:
	SceneNode(const ObjectInfo& p_info);
	~SceneNode();

	void SetParent(SceneNode* p_parent);

	void AddChild(SceneNode* p_child);
	void RemoveChild(SceneNode* p_child);

	void SetTransform(const Matrix3x4& p_transform) { transform = p_transform; }
	const Matrix3x4& GetTransform() const { return transform; }

	void EnterTree();
	void ExitTree();

	virtual void OnAwake();

	virtual void OnSleep();

	virtual void Update(float p_deltaTime);

public:
	bool isInTree = false;
};
#endif // ST_SCENE_NODE_H

