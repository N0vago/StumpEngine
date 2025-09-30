#ifndef ST_MESH_H
#define ST_MESH_H

#include <string>

#include "STTexture.h"
#include "STCamera.h"
#include "STVertexArrayObj.h"
#include "STElementsBufferObj.h"

class STMesh
{
	std::vector<STVertex> vertices;
	std::vector<GLuint> indices;
	std::vector<STTexture> textures;
public:

	STVertexArrayObj vertexArrayObj;
	
	STMesh(std::vector<STVertex>& vertices, std::vector<GLuint>& indices, std::vector<STTexture>& textures);
	void Draw(
		STShader& shader,
		STCamera& camera,
		glm::mat4 matrix = glm::mat4(1.0f),
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
};
#endif

