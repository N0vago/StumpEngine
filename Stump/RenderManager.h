#ifndef ST_RENDER_MANAGER_H
#define ST_RENDER_MANAGER_H

#include <iostream>
#include "Camera.h"
#include "Mesh.h"
#include "VertexBufferObj.h"
#include "ElementsBufferObj.h"
#include "VertexArrayObj.h"

#include <memory>
#include <GLFW/glfw3.h>
class RenderManager
{
	struct RenderObject
	{
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<VertexArrayObj> vao;
	};

	std::vector<RenderObject> meshes;

	Camera& camera;

public:

	RenderManager(Camera& r_camera) : camera(r_camera) {}

	void DrawMeshes();

	void AddToRender(Mesh& r_mesh);

	float* ToMatrix4x4(Matrix3x4 p_transform);
private:
	void DrawMesh(RenderObject& r_renderObject, Camera& r_camera);

};


#endif