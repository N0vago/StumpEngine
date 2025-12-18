#include "Rendering/Mesh.h"
#include "Rendering/Camera.h"

namespace Rendering {
    Mesh::Mesh()
    {
    }

    Mesh::Mesh(std::vector<Vertex>& r_vertices, std::vector<uint32_t>& r_indices) : vertices(r_vertices), indices(r_indices)
    {
        SetupMesh();
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::SetupMesh()
    {
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


    void Mesh::Draw()
    {
        VAO.Bind();

        glDrawElements(GL_TRIANGLES,
            indices.size(),
            GL_UNSIGNED_INT,
            0);
    }
}