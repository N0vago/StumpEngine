#include "STVertexBufferObj.h"


STVertexBufferObj::STVertexBufferObj(std::vector<STVertex>& vertices) {

	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(STVertex), vertices.data(), GL_STATIC_DRAW);
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