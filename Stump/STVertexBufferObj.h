#ifndef ST_VERTEX_BUFFER_OBJ_H
#define ST_VERTEX_BUFFER_OBJ_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct STVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoord;
};

class STVertexBufferObj
{
public:
	GLuint ID;

	STVertexBufferObj(std::vector<STVertex>& vertices);

	void Bind();

	void Unbind();
	
	void Delete();
};

#endif

