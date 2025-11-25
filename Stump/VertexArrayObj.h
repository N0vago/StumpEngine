#ifndef ST_VERTEX_ARRAY_OBJ_H
#define ST_VERTEX_ARRAY_OBJ_H

#include <glad/glad.h>
#include "VertexBufferObj.h"

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

#endif

