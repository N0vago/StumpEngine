#include "STVertexArrayObj.h"

STVertexArrayObj::STVertexArrayObj()
{
	glGenVertexArrays(1, &ID);
}

// Links a VBO to the VAO using a certain layout
void STVertexArrayObj::LinkAttrib(STVertexBufferObj& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Binds the VAO
void STVertexArrayObj::Bind()
{
	glBindVertexArray(ID);
}

// Unbinds the VAO
void STVertexArrayObj::Unbind()
{
	glBindVertexArray(0);
}

// Deletes the VAO
void STVertexArrayObj::Delete()
{
	glDeleteVertexArrays(1, &ID);
}