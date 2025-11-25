#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "Mesh.h"
#include "Math.h"

const unsigned int width = 800;
const unsigned int height = 800;

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

    // copy
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 4; c++)
            out[r][c] = mat3x4.elements[r][c];

    out[3][0] = 0;
    out[3][1] = 0;
    out[3][2] = 0;
    out[3][3] = 1;

    return &out[0][0];
}

int main()
{
	
    //Setup glfw
    if (!glfwInit())
    {
        std::cout << "GLFW init failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Setup window
    GLFWwindow* window = glfwCreateWindow(width, height, "StumpEngine", NULL, NULL);

    if (window == NULL) {

        std::cout << "Failed to create window" << std::endl;

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, width, height);

    Shader shader("default.vert", "default.frag");

    Texture textures[] =
    {
        Texture("planks.png", "diffuse", 0),
        Texture("planksSpec.png", "specular", 1)
    };

	std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector<GLuint> inds(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<Texture> texs(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh floor(verts, inds, texs);

    // Shader for light cube
    Shader lightShader("light.vert", "light.frag");

	std::vector<Vertex> lightVerts = GetSphereVertices(0.1f, 36, 18);
	std::vector<GLuint> lightInds = GetSphereIndices(36, 18);
	Mesh lightCube(lightVerts, lightInds, texs);



    Vector3 lightColor = Vector3(1.0f, 1.0f, 1.0f);
    Vector3 lightPos = Vector3(0.0f, 1.0f, 0.0f);
	//TODO: Convert to Mat4
    Matrix3x4 lightModel;
    lightModel.Translated(lightPos);



    Vector3 pyramidPos = Vector3(0.0f, 0.0f, 0.0f);
    //TODO: Convert to Mat4
    Matrix3x4 pyramidModel;
    pyramidModel.Translated(pyramidPos);
	pyramidModel.Rotate(Vector3(1.0f, 0.0f, 0.0f), 90.0f);


    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, ToMatrix4x4(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, 1);
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, ToMatrix4x4(pyramidModel));
    glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, 1);
    glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, Vector3(0.0f, 0.0f, 2.0f));

    float rotation = 0.0f;
    double startFrameTime;
    double endFrameTime;
    float deltaTime = 1.0f;

    while (!glfwWindowShouldClose(window)) {

        startFrameTime = glfwGetTime();
        
        glClearColor(0.26f, 0.11f, 0.32f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window, deltaTime);
        camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

		floor.Draw(shader, camera, pyramidModel);
		lightCube.Draw(lightShader, camera, lightModel);

        glfwSwapBuffers(window);
        glfwPollEvents();


        endFrameTime = glfwGetTime();

        deltaTime = endFrameTime - startFrameTime;

    }

    shader.Delete();
    lightShader.Delete();

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}