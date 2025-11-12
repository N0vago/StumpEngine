#include "STVertexBufferObj.h"


STVertexBufferObj::STVertexBufferObj(std::vector<STVertex>& vertices) {

	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(STVertex), vertices.data(), GL_STATIC_DRAW);

	//print vertex data for debugging
	/*printf("Vertex Buffer Data:\n");
	for (size_t i = 0; i < vertices.size(); i++) {
		printf("Vertex %zu: Position(%.2f, %.2f, %.2f), Normal(%.2f, %.2f, %.2f), Color(%.2f, %.2f, %.2f), TexCoord(%.2f, %.2f)\n",
			i,
			vertices[i].position.x, vertices[i].position.y, vertices[i].position.z,
			vertices[i].normal.x, vertices[i].normal.y, vertices[i].normal.z,
			vertices[i].color.x, vertices[i].color.y, vertices[i].color.z,
			vertices[i].texCoord.x, vertices[i].texCoord.y
		);
	}*/
}

void STVertexBufferObj::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void STVertexBufferObj::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void STVertexBufferObj::Delete() {
	glDeleteBuffers(1, &ID);
}