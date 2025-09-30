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

STVertex lightVertices[] =
{ 
	STVertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
    STVertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    STVertex{glm::vec3(0.1f, -0.1f, -0.1f)},
    STVertex{glm::vec3(0.1f, -0.1f,  0.1f)},
    STVertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
    STVertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
    STVertex{glm::vec3(0.1f,  0.1f, -0.1f)},
    STVertex{glm::vec3(0.1f,  0.1f,  0.1f)},
     
};

GLuint lightIndices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
};

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
    
	std::vector<STVertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(STVertex));
	std::vector<GLuint> lightInds(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	STMesh lightCube(lightVerts, lightInds, texs);



    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 1.5f, 0.0f);
    glm::mat4 lightModel = glm::mat4(1.0f);

    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 pyramidModel = glm::mat4(1.0f);
    pyramidModel = glm::translate(pyramidModel, pyramidPos);


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

		floor.Draw(shader, camera);
		lightCube.Draw(lightShader, camera);

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