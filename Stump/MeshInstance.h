#ifndef ST_MESH_INSTANCE_H
#define ST_MESH_INSTANCE_H
#include "SceneNode.h"
#include "RenderManager.h"

#include <memory>
class MeshInstance : public SceneNode
{
public:
	std::shared_ptr<RenderObject> renderObject;

	MeshInstance(const ObjectInfo& r_objectInfo);

	MeshInstance(const ObjectInfo& r_objectInfo, RenderObject* p_renderObject);

	virtual void OnAwake() override;

	virtual void Update(float p_deltaTime) override;

	virtual void OnSleep() override;
};
#endif

