#ifndef ST_VERTEX_BUFFER_OBJ_H
#define ST_VERTEX_BUFFER_OBJ_H

#include "Rendering/Vertex.h"
#include <vector>
#include <glad/glad.h>

namespace Rendering {

	class VertexBufferObj
	{
	public:
		GLuint ID;

		VertexBufferObj(std::vector<Vertex>& vertices);

		void Bind();

		void Unbind();

		void Delete();
	};
}
#endif

