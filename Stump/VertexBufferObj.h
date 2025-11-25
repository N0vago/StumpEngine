#ifndef ST_VERTEX_BUFFER_OBJ_H
#define ST_VERTEX_BUFFER_OBJ_H

#include <glad/glad.h>
#include "Vector3.h"
#include "Vector2.h"
#include <vector>

struct Vertex {
	Vector3 position;
	Vector3 normal;
	Vector3 color;
	Vector2 texCoord;
};

class VertexBufferObj
{
public:
	GLuint ID;

	VertexBufferObj(std::vector<Vertex>& vertices);

	void Bind();

	void Unbind();
	
	void Delete();
};

#endif

