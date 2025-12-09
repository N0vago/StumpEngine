#ifndef ST_MESH_H
#define ST_MESH_H

#include "Object.h"
#include "Vertex.h"
#include "Texture.h"
#include "VertexArrayObj.h"
#include "ElementsBufferObj.h"
#include "VertexBufferObj.h"
#include <vector>

class Mesh
{
	VertexArrayObj VAO;
protected:
	std::vector<Vertex> vertices;

	std::vector<uint32_t> indices;

	std::vector<Texture> meshTextures;

	Shader& meshShader;

	void SetupMesh();
public:

	Mesh(Shader& p_shader) : meshShader(p_shader) {}

	Mesh(std::vector<Vertex>& r_vertices, std::vector<uint32_t>& r_indices, Shader& p_shader);

	void Draw();

	void ApplyTexture(std::vector<Texture> textures);

	Shader& GetShader() { return meshShader; }
};
#endif

