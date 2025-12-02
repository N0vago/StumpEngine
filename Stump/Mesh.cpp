#include "Mesh.h"


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
