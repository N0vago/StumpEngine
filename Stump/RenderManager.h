#ifndef ST_RENDER_MANAGER_H
#define ST_RENDER_MANAGER_H

#include <iostream>
#include "Camera.h"
#include "Mesh.h"

#include <memory>
#include <GLFW/glfw3.h>
class RenderManager
{
	std::vector<std::unique_ptr<Mesh>> meshes;

	Camera& camera;

public:

	RenderManager(Camera& r_camera);
	~RenderManager();

	void DrawMeshes();

	void AddToRender(const Mesh* p_mesh);

	void RemoveFromRender(const Mesh* p_mesh);

	static RenderManager& Get();

};


#endif