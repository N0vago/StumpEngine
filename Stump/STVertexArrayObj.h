#ifndef ST_VERTEX_ARRAY_OBJ_H
#define ST_VERTEX_ARRAY_OBJ_H

#include <glad/glad.h>
#include "STVertexBufferObj.h"

class STVertexArrayObj
{
public:
	GLuint ID;

	STVertexArrayObj();

	void LinkAttrib(STVertexBufferObj& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	
	void Bind();
	
	void Unbind();
	
	void Delete();
};

#endif

