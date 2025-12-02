#ifndef ST_MESH_H
#define ST_MESH_H

#include <vector>
#include "Object.h"
#include "Vertex.h"
#include "Texture.h"

class Mesh : Object
{
public:
	std::vector<Vertex> vertices;

	std::vector<uint32_t> indices;

	std::vector<Texture> meshTextures;

	Matrix3x4 meshTransform;

	Shader meshShader;

	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, Shader shader) : vertices(vertices), indices(indices), meshShader(shader) {

	}
	void ApplyTexture(std::vector<Texture> textures);
};
#endif

