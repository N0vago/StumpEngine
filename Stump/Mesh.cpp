#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

    VertexArrayObj VAO;
	this->vertexArrayObj = VAO;
    VAO.Bind();

    VertexBufferObj VBO(vertices);
    ElementsBufferObj EBO(indices);

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

void Mesh::Draw(
	Shader& shader,
	Camera& camera,
	Matrix3x4 matrix,
	Vector3 translation,
	Quaternion rotation,
	Vector3 scale)
{
	// Bind shader to be able to access uniforms
	shader.Activate();
	vertexArrayObj.Bind();

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string num;
		std::string type = textures[i].type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}	
	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");

	// Initialize matrices
	Matrix3x4 trans;
	Matrix3x4 rot;
	Matrix3x4 sca;
	
	// Transform the matrices to their correct form
	trans.Translate(translation);
	rot.mat3.Rotate(rotation);
	sca.Scale(scale);

	

	// Push the matrices to the vertex shader
	glUniformMatrix3x4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, trans.elements[0]);
	glUniformMatrix3x4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, rot.elements[0]);
	glUniformMatrix3x4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, sca.elements[0]);
	glUniformMatrix3x4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, matrix.elements[0]);

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
