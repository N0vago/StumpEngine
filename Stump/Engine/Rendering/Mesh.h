#ifndef ST_MESH_H
#define ST_MESH_H

#include "Rendering/Vertex.h"
#include "Rendering/Buffers/VertexArrayObj.h"
#include "Rendering/Buffers/ElementsBufferObj.h"
#include "Rendering/Buffers/VertexBufferObj.h"
#include <vector>
namespace Rendering {
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
}
#endif

