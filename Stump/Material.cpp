#include "Material.h"
void Material::SetTexture(Texture* p_texture)
{
    textures[p_texture->type] = p_texture;
}
void Material::Bind() const
{
    shader->Activate();

    for (const auto& [name, uniform] : uniforms)
    {
        switch (uniform.type)
        {

        case UniformType::Int:
            shader->SetInt(name.c_str(), uniform.i);
            break;

        case UniformType::Float:
            shader->SetFloat(name.c_str(), uniform.f);
            break;
        
        case UniformType::Float3:
            shader->SetFloat3(name.c_str(), uniform.f3[0], uniform.f3[1], uniform.f3[2]);
            break;
        case UniformType::Float4:
            shader->SetFloat4(name.c_str(), uniform.f4[0], uniform.f4[1], uniform.f4[2], uniform.f4[3]);
            break;

        case UniformType::Vec3:
            shader->SetVec3(name.c_str(), uniform.vec3);
            break;

        case UniformType::Mat4:
            shader->SetMat4(name.c_str(), uniform.mat4);
            break;
        }

        uint32_t diffuseIndex = 0;
        uint32_t specularIndex = 0;

        for (const auto& [type, texture] : textures)
        {
            std::string uniformName;

            switch (type)
            {
            case TextureType::Diffuse:
                uniformName = "diffuse" + std::to_string(diffuseIndex++);
                break;
            case TextureType::Specular:
                uniformName = "specular" + std::to_string(specularIndex++);
                break;
            }

            texture->Bind();

            shader->SetInt(uniformName.c_str(), texture->unit);
        }
    }
}
void Material::SetFloat(const std::string& r_name, float p_value)
{
    uniforms[r_name] = UniformValue{ .type = UniformType::Float, .f = p_value };
}

void Material::SetFloat3(const std::string& r_name, float p_x, float p_y, float p_z)
{
    uniforms[r_name] = UniformValue{ .type = UniformType::Float3, .f3 = { p_x, p_y, p_z } };
}

void Material::SetFloat4(const std::string& r_name, float p_x, float p_y, float p_z, float p_w)
{
    uniforms[r_name] = UniformValue{ .type = UniformType::Float4, .f4 = { p_x, p_y, p_z, p_w } };
}

void Material::SetInt(const std::string& r_name, int p_value)
{
    uniforms[r_name] = UniformValue{ .type = UniformType::Int, .i = p_value };
}

void Material::SetVec3(const std::string& r_name, Vector3& r_value)
{
    uniforms[r_name] = UniformValue{ .type = UniformType::Vec3, .vec3 = r_value};
}

void Material::SetMat4(const std::string& r_name, float* p_value)
{
    uniforms[r_name] = UniformValue{ .type = UniformType::Mat4, .mat4 = p_value };
}