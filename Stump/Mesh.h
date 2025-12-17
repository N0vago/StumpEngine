#ifndef ST_MESH_H
#define ST_MESH_H

#include "Vertex.h"
#include "VertexArrayObj.h"
#include "ElementsBufferObj.h"
#include "VertexBufferObj.h"
#include <vector>

class Mesh
{
	VertexArrayObj VAO;
protected:
	std::vector<Vertex> vertices;

	std::vector<uint32_t> indices;

	void SetupMesh();
public:

	Mesh();

	Mesh(std::vector<Vertex>& r_vertices, std::vector<uint32_t>& r_indices);

	~Mesh();

	void Draw();
};
#endif

