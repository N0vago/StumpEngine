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
            shader->SetInt(
                name.c_str(),
                std::get<int>(uniform.data)
            );
            break;

        case UniformType::Float:
            shader->SetFloat(
                name.c_str(),
                std::get<float>(uniform.data)
            );
            break;
        
        case UniformType::Float3:
            shader->SetFloat3(
                name.c_str(),
                std::get<std::array<float, 3>>(uniform.data).at(0), 
                std::get<std::array<float, 3>>(uniform.data).at(1),
                std::get<std::array<float, 3>>(uniform.data).at(2)
            );
            break;
        case UniformType::Float4:
            shader->SetFloat4(
                name.c_str(), 
                std::get<std::array<float, 4>>(uniform.data).at(0),
                std::get<std::array<float, 4>>(uniform.data).at(1), 
                std::get<std::array<float, 4>>(uniform.data).at(2), 
                std::get<std::array<float, 4>>(uniform.data).at(3)
            );
            break;

        case UniformType::Vec3:
            shader->SetVec3(
                name.c_str(),
                std::get<Vector3>(uniform.data)
            );
            break;

        case UniformType::Mat4:
            shader->SetMat4(
                name.c_str(),
                std::get<std::array<float, 16>>(uniform.data)
            );
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
    uniforms.insert_or_assign(r_name, UniformValue{ UniformType::Float, p_value });
}

void Material::SetFloat3(const std::string& r_name, float p_x, float p_y, float p_z)
{
    uniforms.insert_or_assign(r_name, UniformValue{ UniformType::Float3, std::array<float, 3>{ p_x, p_y, p_z } });
}

void Material::SetFloat4(const std::string& r_name, float p_x, float p_y, float p_z, float p_w)
{
    uniforms.insert_or_assign(r_name, UniformValue{ UniformType::Float4, std::array<float, 4>{ p_x, p_y, p_z, p_w } });
}

void Material::SetInt(const std::string& r_name, int p_value)
{
    uniforms.insert_or_assign(r_name, UniformValue{ UniformType::Int,  p_value });
}

void Material::SetVec3(const std::string& r_name, Vector3& r_value)
{
    uniforms.insert_or_assign(r_name, UniformValue{ UniformType::Vec3, r_value});
}

void Material::SetMat4(const std::string& r_name, std::array<float, 16> p_value)
{
    uniforms.insert_or_assign(r_name, UniformValue{ UniformType::Mat4, std::array<float, 16>(p_value) });
}