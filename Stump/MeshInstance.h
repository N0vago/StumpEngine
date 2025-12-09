#ifndef ST_MESH_INSTANCE_H
#define ST_MESH_INSTANCE_H
#include "SceneNode.h"
#include "RenderManager.h"

#include <memory>
class MeshInstance : public SceneNode
{
	std::shared_ptr<Mesh> mesh;
public:
	MeshInstance(const ObjectInfo& p_info, Mesh* p_mesh) : SceneNode(p_info), mesh(std::move(p_mesh)) {}
	
protected:
	virtual void Update(float p_deltaTime) override;

	virtual void OnAwake() override;
	virtual void OnSleep() override;
};
#endif // ST_MESH_INSTANCE_H

