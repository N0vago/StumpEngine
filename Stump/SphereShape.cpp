#include "SphereShape.h"

SphereShape::SphereShape(float p_radius, int p_sectorCount, int p_stackCount, Shader& p_shader) : Shape(p_shader), radius(p_radius), sectorCount(p_sectorCount), stackCount(p_stackCount)
{
    GenerateVertices();
    GenerateIndices();
	SetupMesh();
}

void SphereShape::GenerateVertices()
{
    vertices.clear();
    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = Math::PI / 2 - i * Math::PI / stackCount; // from pi/2 to -pi/2
        float xy = radius * cosf(stackAngle);               // r * cos(u)
        float z = radius * sinf(stackAngle);                // r * sin(u)
        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * Math::PI / sectorCount; // from 0 to 2pi
            float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
            float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
            Vector3 position(x, y, z);
            Vector3 normal = position.Normalized();
            Vector2 texCoords((float)j / sectorCount, (float)i / stackCount);
            vertices.push_back({ position, normal, Vector3(1.0f, 1.0f, 1.0f), texCoords });
        }
    }
}

void SphereShape::GenerateIndices()
{
    indices.clear();
    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);     // beginning of current stack
        int k2 = k1 + sectorCount + 1;      // beginning of next stack
        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}