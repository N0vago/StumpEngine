#ifndef ST_MESH_INSTANCE_H
#define ST_MESH_INSTANCE_H
#include "GameObject.h"
#include "Camera.h"

#include <memory>

class ObjectInstance : public GameObject
{
	std::shared_ptr<Mesh> mesh;

	Camera& camera;
public:
	ObjectInstance(const ObjectInfo& p_info, std::shared_ptr<Mesh> p_mesh, Camera& r_camera) : GameObject(p_info), mesh(std::move(p_mesh)), camera(r_camera) {}
	

protected:
	virtual void Update(float p_deltaTime) override;

};
#endif // ST_MESH_INSTANCE_H

