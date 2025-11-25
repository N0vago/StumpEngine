#ifndef ST_MESH_H
#define ST_MESH_H

#include <string>

#include "Vector3.h"
#include "Quaternion.h"
#include "Matrix3x4.h"
#include "Texture.h"
#include "Camera.h"
#include "VertexArrayObj.h"
#include "ElementsBufferObj.h"

class Mesh
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
public:

	VertexArrayObj vertexArrayObj;
	
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
	void Draw(
		Shader& shader,
		Camera& camera,
		Matrix3x4 matrix = Matrix3x4(),
		Vector3 translation = Vector3(0.0f, 0.0f, 0.0f),
		Quaternion rotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f),
		Vector3 scale = Vector3(1.0f, 1.0f, 1.0f));
};
#endif

