#include "Mesh.h"
#include "Camera.h"


Mesh::Mesh(std::vector<Vertex>& r_vertices, std::vector<uint32_t>& r_indices, Shader& p_shader) : vertices(r_vertices), indices(r_indices), meshShader(p_shader)
{
    SetupMesh();
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

void Mesh::ApplyTexture(std::vector<Texture> textures) {
    meshTextures = textures;

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
        textures[i].TextureUnit(meshShader, (type + num).c_str(), i);
        textures[i].Bind();
    }
}


void Mesh::Draw()
{
    meshShader.Activate();

    VAO.Bind();

    glDrawElements(GL_TRIANGLES,
        indices.size(),
        GL_UNSIGNED_INT,
        0);
}
