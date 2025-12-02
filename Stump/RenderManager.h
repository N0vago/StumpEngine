#ifndef ST_RENDER_MANAGER_H
#define ST_RENDER_MANAGER_H

#include <iostream>
#include "Camera.h"
#include "Mesh.h"
#include "VertexBufferObj.h"
#include "ElementsBufferObj.h"
#include "VertexArrayObj.h"

#include <GLFW/glfw3.h>
class RenderManager
{
	struct RenderObject
	{
		Mesh* mesh;
		VertexArrayObj vao;
	};

	GLFWwindow* window;

	Camera camera;

	std::vector<RenderObject> meshes;

public:

	float deltaTime = 0.0f;

	RenderManager(uint16_t p_windowWidth, uint16_t p_windowHeight);

	void Init();

	void AddToRender(Mesh& r_mesh);

private:
	void DrawMesh(RenderObject& r_renderObject, Camera& r_camera);

	float* ToMatrix4x4(Matrix3x4 p_transform);
};


#endif