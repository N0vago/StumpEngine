#ifndef ST_MESH_INSTANCE_H
#define ST_MESH_INSTANCE_H
#include "SceneNode.h"
#include "RenderManager.h"
#include "Camera.h"

#include <memory>

class MeshInstance : public SceneNode
{
	std::shared_ptr<Mesh> mesh;

	Camera& camera;
public:
	MeshInstance(const ObjectInfo& p_info, std::shared_ptr<Mesh> p_mesh, Camera& r_camera) : SceneNode(p_info), mesh(std::move(p_mesh)), camera(r_camera) {}
	
protected:
	virtual void Update(float p_deltaTime) override;

	virtual void OnAwake() override;
	virtual void OnSleep() override;
};
#endif // ST_MESH_INSTANCE_H

