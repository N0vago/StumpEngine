#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "STMesh.h"




const unsigned int width = 800;
const unsigned int height = 800;

// Vertices coordinates
STVertex vertices[] =
{ 
    STVertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
    STVertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
    STVertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    STVertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3
};

std::vector<STVertex> GetCircleVertices(float radius)
{
    std::vector<STVertex> circleVertices;
    for (int i = 0; i < 360; ++i)
    {
        float angle = i * 2 * glm::pi<float>() / 360;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        circleVertices.push_back(STVertex{ glm::vec3(x, 0.0f, z) });
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

std::vector<STVertex> GetSphereVertices(float radius, int sectorCount, int stackCount)
{
    std::vector<STVertex> sphereVertices;
    for (int i = 0; i <= 180; ++i)
    {
        float stackAngle = glm::pi<float>() / 2 - i * glm::pi<float>() / 180; // from pi/2 to -pi/2
        float xy = radius * cos(stackAngle); // r * cos(u)
        float z = radius * sin(stackAngle);  // r * sin(u)
        for (int j = 0; j <= 360; ++j)
        {
            float sectorAngle = j * 2 * glm::pi<float>() / 360; // from 0 to 2pi
            float x = xy * cos(sectorAngle); // r * cos(u) * cos(v)
            float y = xy * sin(sectorAngle); // r * cos(u) * sin(v)
            sphereVertices.push_back(STVertex{ glm::vec3(x, y, z) });
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

int main()
{
	
    //Setup glfw
    glfwInit();

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

    STShader shader("default.vert", "default.frag");

    STTexture textures[] =
    {
        STTexture("planks.png", "diffuse", 0),
        STTexture("planksSpec.png", "specular", 1)
    };

	std::vector<STVertex> verts(vertices, vertices + sizeof(vertices) / sizeof(STVertex));
	std::vector<GLuint> inds(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<STTexture> texs(textures, textures + sizeof(textures) / sizeof(STTexture));
	STMesh floor(verts, inds, texs);

    // Shader for light cube
    STShader lightShader("light.vert", "light.frag");

	std::vector<STVertex> lightVerts = GetSphereVertices(0.1f, 36, 18);
	std::vector<GLuint> lightInds = GetSphereIndices(36, 18);
	STMesh lightCube(lightVerts, lightInds, texs);



    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 lightModel = glm::mat4(1.0f);

    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos);

	pyramidModel = glm::rotate(pyramidModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
    glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);


    // Enables the Depth Buffer

    glEnable(GL_DEPTH_TEST);

    //Camera

    STCamera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

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