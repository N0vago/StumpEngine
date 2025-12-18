#ifndef ST_VERTEX_ARRAY_OBJ_H
#define ST_VERTEX_ARRAY_OBJ_H

#include "Rendering/Buffers/VertexBufferObj.h"
#include <glad/glad.h>

namespace Rendering {
	class VertexArrayObj
	{
	public:
		GLuint ID;

		VertexArrayObj();

		void LinkAttrib(VertexBufferObj& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

		void Bind();

		void Unbind();

		void Delete();
	};
}
#endif

