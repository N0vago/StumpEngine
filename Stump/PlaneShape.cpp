#include "PlaneShape.h"

PlaneShape::PlaneShape(float p_width, float p_height, Shader& p_shader) : Shape(p_shader), width(p_width), height(p_height)
{
    GenerateVertices();
	GenerateIndices();
	SetupMesh();

	std::cout << "Created PlaneShape with width: " << width << " and height: " << height << std::endl;
}

void PlaneShape::GenerateVertices()
{
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    vertices = {
        { Vector3(-halfWidth, 0.0f, -halfHeight), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },
        { Vector3(halfWidth, 0.0f, -halfHeight), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) },
        { Vector3(halfWidth, 0.0f, halfHeight), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) },
        { Vector3(-halfWidth, 0.0f, halfHeight), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) }
    };
}
void PlaneShape::GenerateIndices()
{
    indices = {
        0, 1, 2,
        0, 2, 3
    };
}