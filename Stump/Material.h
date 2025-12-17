#ifndef ST_MATERIAL_H
#define ST_MATERIAL_H
#include "Shader.h"
#include "Texture.h"
#include "Vector3.h"

#include <array>
#include <unordered_map>
#include <variant>

using UniformData = std::variant<
    int,
    float,
    Vector3,
    std::array<float, 3>,
    std::array<float, 4>,
    std::array<float, 16>
>;

enum class UniformType
{
    Float,
    Float3,
    Float4,
    Vec3,
    Mat4,
    Int
};

struct UniformValue
{
    UniformType type;

    UniformData data;
};
class Material
{
public:
    Material(std::shared_ptr<Shader> p_shader) : shader(std::move(p_shader)) {}

    void SetTexture(Texture* p_texture);

    void Bind() const;

    void SetInt(const std::string& r_name, int p_value);
    void SetFloat(const std::string& r_name, float p_value);
    void SetFloat3(const std::string& r_name, float p_x, float p_y, float p_z);
    void SetFloat4(const std::string& r_name, float p_x, float p_y, float p_z, float p_w);
    void SetVec3(const std::string& r_name, Vector3& r_value);
    void SetMat4(const std::string& r_name, std::array<float, 16> p_value);

    Shader* GetShader() const { return shader.get(); }

private:
    std::shared_ptr<Shader> shader;

    std::unordered_map<TextureType, Texture*> textures;
    std::unordered_map<std::string, UniformValue> uniforms;
};
#endif //ST_MATERIAL_H

