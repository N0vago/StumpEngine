#ifndef ST_RENDER_MANAGER_H
#define ST_RENDER_MANAGER_H

#include <iostream>

#include <memory>
#include <GLFW/glfw3.h>
class Mesh;
class RenderManager
{
	std::vector<Mesh*> meshes;

public:

	RenderManager();
	~RenderManager();

	void DrawMeshes();

	void AddToRender(Mesh* p_mesh);

	void RemoveFromRender(Mesh* p_mesh);

	static RenderManager& Get();

};


#endif