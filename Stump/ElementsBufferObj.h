#ifndef ST_ELEMENTS_BUFFER_OBJ_H
#define ST_ELEMENTS_BUFFER_OBJ_H

#include <glad/glad.h>
#include <vector>

class ElementsBufferObj
{
public:
	GLuint ID;

	ElementsBufferObj(std::vector<GLuint>& indices);

	void Bind();

	void Unbind();

	void Delete();
};

#endif

