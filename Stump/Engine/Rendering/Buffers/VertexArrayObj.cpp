#include "Rendering/Buffers/VertexArrayObj.h"
namespace Rendering {
	VertexArrayObj::VertexArrayObj()
	{
		glGenVertexArrays(1, &ID);
	}

	// Links a VBO to the VAO using a certain layout
	void VertexArrayObj::LinkAttrib(VertexBufferObj& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
	{
		VBO.Bind();
		glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(layout);
		VBO.Unbind();
	}

	// Binds the VAO
	void VertexArrayObj::Bind()
	{
		glBindVertexArray(ID);
	}

	// Unbinds the VAO
	void VertexArrayObj::Unbind()
	{
		glBindVertexArray(0);
	}

	// Deletes the VAO
	void VertexArrayObj::Delete()
	{
		glDeleteVertexArrays(1, &ID);
	}
}