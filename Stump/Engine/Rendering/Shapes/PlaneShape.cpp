#include "Rendering/Shapes/PlaneShape.h"
namespace Rendering {
    PlaneShape::PlaneShape(float p_width, float p_height) : width(p_width), height(p_height)
    {
        GenerateVertices();
        GenerateIndices();
        SetupMesh();
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
}