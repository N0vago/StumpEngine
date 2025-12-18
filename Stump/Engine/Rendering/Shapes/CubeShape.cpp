#include "Rendering/Shapes/CubeShape.h"
namespace Rendering {
    CubeShape::CubeShape(float p_size) : size(p_size)
    {
        GenerateVertices();
        GenerateIndices();
        SetupMesh();
    }

    CubeShape::~CubeShape()
    {
    }
    void CubeShape::GenerateVertices()
    {
        const float half = size * 0.5f;

        std::vector<Vertex> vertices = {
            // Front face
            { Vector3(-half, -half,  half), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },
            { Vector3(half, -half,  half), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) },
            { Vector3(half,  half,  half), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) },
            { Vector3(-half,  half,  half), Vector3(0.0f, 0.0f, 1.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) },

            // Back face
            { Vector3(-half, -half, -half), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) },
            { Vector3(-half,  half, -half), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) },
            { Vector3(half,  half, -half), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) },
            { Vector3(half, -half, -half), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },

            // Left face
            { Vector3(-half, -half, -half), Vector3(-1.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },
            { Vector3(-half, -half,  half), Vector3(-1.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) },
            { Vector3(-half,  half,  half), Vector3(-1.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) },
            { Vector3(-half,  half, -half), Vector3(-1.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) },

            // Right face
            { Vector3(half, -half,  half), Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },
            { Vector3(half, -half, -half), Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) },
            { Vector3(half,  half, -half), Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) },
            { Vector3(half,  half,  half), Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) },

            // Top face
            { Vector3(-half,  half,  half), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },
            { Vector3(half,  half,  half), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) },
            { Vector3(half,  half, -half), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) },
            { Vector3(-half,  half, -half), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) },

            // Bottom face
            { Vector3(-half, -half, -half), Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 0.0f) },
            { Vector3(half, -half, -half), Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 0.0f) },
            { Vector3(half, -half,  half), Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(1.0f, 1.0f) },
            { Vector3(-half, -half,  half), Vector3(0.0f, -1.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector2(0.0f, 1.0f) }
        };
    }
    void CubeShape::GenerateIndices()
    {
        std::vector<GLuint> indices = {
                0, 1, 2, 0, 2, 3,         // Front
                4, 5, 6, 4, 6, 7,         // Back
                8, 9, 10, 8, 10, 11,      // Left
                12, 13, 14, 12, 14, 15,   // Right
                16, 17, 18, 16, 18, 19,   // Top
                20, 21, 22, 20, 22, 23    // Bottom
        };
    }
}