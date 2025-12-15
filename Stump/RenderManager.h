#ifndef ST_RENDER_MANAGER_H
#define ST_RENDER_MANAGER_H

#include <iostream>
#include "MeshInstance.h"

#include <memory>
#include <GLFW/glfw3.h>
class RenderManager
{
	std::vector<MeshInstance*> meshes;

public:

	RenderManager();
	~RenderManager();

	void DrawMeshes();

	void AddToRender(MeshInstance* p_mesh);

	void RemoveFromRender(MeshInstance* p_mesh);

	static RenderManager& Get();

};


#endif