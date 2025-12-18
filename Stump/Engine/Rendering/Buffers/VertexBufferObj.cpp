#include "Rendering/Buffers/VertexBufferObj.h"

namespace Rendering {
	VertexBufferObj::VertexBufferObj(std::vector<Vertex>& vertices) {

		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	}

	void VertexBufferObj::Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}

	void VertexBufferObj::Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBufferObj::Delete() {
		glDeleteBuffers(1, &ID);
	}
}