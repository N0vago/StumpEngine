#include <stdio.h>
#include <iostream>
#include <stb/stb_image.h>

#include "RenderManager.h"
#include "Mesh.h"
#include "Math.h"

static RenderManager renderManager(800, 800);

// Vertices coordinates
Vertex vertices[] =
{ 
    Vertex{Vector3(-1.0f, 0.0f,  1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)},
    Vertex{Vector3(-1.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f)},
    Vertex{Vector3(1.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f)},
    Vertex{Vector3(1.0f, 0.0f,  1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3
};

std::vector<Vertex> GetCircleVertices(float radius)
{
    std::vector<Vertex> circleVertices;
    for (int i = 0; i < 360; ++i)
    {
        float angle = i * 2 * Math::PI / 360;
        float x = radius * Math::Cosf(angle);
        float z = radius * Math::Sinf(angle);
        circleVertices.push_back(Vertex{ Vector3(x, 0.0f, z) });
    }
    return circleVertices;
}

std::vector<GLuint> GetCircleIndices()
{
    std::vector<GLuint> circleIndices;
    for (int i = 0; i < 360; ++i)
    {
        circleIndices.push_back(i);
        circleIndices.push_back((i + 1) % 360);
        circleIndices.push_back(360); // Center vertex
    }
    return circleIndices;
}

std::vector<Vertex> GetSphereVertices(float radius, int sectorCount, int stackCount)
{
    std::vector<Vertex> sphereVertices;
    for (int i = 0; i <= 180; ++i)
    {
        float stackAngle = Math::PI / 2 - i * Math::PI / 180; // from pi/2 to -pi/2
        float xy = radius * cos(stackAngle); // r * cos(u)
        float z = radius * sin(stackAngle);  // r * sin(u)
        for (int j = 0; j <= 360; ++j)
        {
            float sectorAngle = j * 2 * Math::PI / 360; // from 0 to 2pi
            float x = xy * cos(sectorAngle); // r * cos(u) * cos(v)
            float y = xy * sin(sectorAngle); // r * cos(u) * sin(v)
            sphereVertices.push_back(Vertex{ Vector3(x, y, z) });
        }
    }
    return sphereVertices;
}

std::vector<GLuint> GetSphereIndices(int sectorCount, int stackCount)
{
    std::vector<GLuint> sphereIndices;
    for (int i = 0; i < 180; ++i)
    {
        int k1 = i * (360 + 1);     // beginning of current stack
        int k2 = k1 + 360 + 1;      // beginning of next stack
        for (int j = 0; j < 360; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                sphereIndices.push_back(k1);
                sphereIndices.push_back(k2);
                sphereIndices.push_back(k1 + 1);
            }
            if (i != (180 - 1))
            {
                sphereIndices.push_back(k1 + 1);
                sphereIndices.push_back(k2);
                sphereIndices.push_back(k2 + 1);
            }
        }
    }
    return sphereIndices;
}

float* ToMatrix4x4(Matrix3x4 mat3x4)
{
    static float out[4][4];

	out[0][0] = mat3x4.mat3[0][0];
	out[0][1] = mat3x4.mat3[0][1];
	out[0][2] = mat3x4.mat3[0][2];
	out[0][3] = mat3x4.origin[0];
	out[1][0] = mat3x4.mat3[1][0];
	out[1][1] = mat3x4.mat3[1][1];
	out[1][2] = mat3x4.mat3[1][2];
	out[1][3] = mat3x4.origin[1];
	out[2][0] = mat3x4.mat3[2][0];
	out[2][1] = mat3x4.mat3[2][1];
	out[2][2] = mat3x4.mat3[2][2];
	out[2][3] = mat3x4.origin[2];

    out[3][0] = 0;
    out[3][1] = 0;
    out[3][2] = 0;
    out[3][3] = 1;

	for (int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
			std::cout << out[i][j] << " ";
        }
		std::cout << std::endl;
    }

    return &out[0][0];
}

int main()
{
    Texture textures[] =
    {
        Texture("planks.png", "diffuse", 0),
        Texture("planksSpec.png", "specular", 1)
    };

	std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector<GLuint> inds(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<Texture> texs(textures, textures + sizeof(textures) / sizeof(Texture));

	std::vector<Vertex> lightVerts = GetSphereVertices(0.1f, 36, 18);
	std::vector<GLuint> lightInds = GetSphereIndices(36, 18);

	Mesh floor(verts, inds, Shader("default.vert", "default.frag"));

    floor.ApplyTexture(texs);

	Mesh lightCube(lightVerts, lightInds, Shader("light.vert", "light.frag"));

    Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f);
    Vector3 lightPos = Vector3(0.0f, 1.0f, 0.0f);

    Matrix3x4 lightMatrix;
    lightMatrix.Translate(lightPos);

    Vector3 floorPos = Vector3(0.0f, 0.0f, 0.0f);

    Matrix3x4 floorMatrix;
    floorMatrix.Translate(floorPos);
	floorMatrix.Scale(Vector3(100.0f, 1.0f, 100.0f));

    lightCube.meshShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightCube.meshShader.ID, "model"), 1, GL_FALSE, ToMatrix4x4(lightMatrix));
    glUniform4f(glGetUniformLocation(lightCube.meshShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, 1);
    floor.meshShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(floor.meshShader.ID, "model"), 1, GL_FALSE, ToMatrix4x4(floorMatrix));
    glUniform4f(glGetUniformLocation(floor.meshShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, 1);
    glUniform3f(glGetUniformLocation(floor.meshShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);



    renderManager.Init();

    renderManager.AddToRender(lightCube);
    renderManager.AddToRender(floor);
    return 0;
};