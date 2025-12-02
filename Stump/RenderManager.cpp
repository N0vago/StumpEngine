#include "RenderManager.h"


RenderManager::RenderManager(uint16_t p_windowWidth, uint16_t p_windowHeight) : camera(Camera(p_windowWidth, p_windowHeight, Vector3(0.0f, 0.0f, 1.0f))) {
    

}

void RenderManager::Init()
{

    float startFrameTime;
    float endFrameTime;


    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {

        startFrameTime = glfwGetTime();

        glClearColor(0.26f, 0.11f, 0.32f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window, deltaTime);
        camera.UpdateMatrix(45.0f, 0.1f, 100.0f);

        
        for(auto& mesh : meshes)
        {
            if (meshes.size() == 0) break;
            std::cout << "Draw element" << "\n";
            DrawMesh(mesh, camera);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();


        endFrameTime = glfwGetTime();

        deltaTime = endFrameTime - startFrameTime;

    }

    //TODO: Delete shaders here

    glfwDestroyWindow(window);

    glfwTerminate();
}

void RenderManager::AddToRender(Mesh& r_mesh) {

    VertexArrayObj VAO;
    VAO.Bind();

    VertexBufferObj VBO(r_mesh.vertices);
    ElementsBufferObj EBO(r_mesh.indices);

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
    VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();

    meshes.push_back(RenderObject{ &r_mesh, std::move(VAO) });
}



void RenderManager::DrawMesh(RenderObject& r_renderObject, Camera& r_camera)
{
    Mesh& mesh = *r_renderObject.mesh;    
    VertexArrayObj& vao = r_renderObject.vao;

    mesh.meshShader.Activate();

    glUniform3f(glGetUniformLocation(mesh.meshShader.ID, "camPos"),
        r_camera.Position.x, r_camera.Position.y, r_camera.Position.z);

    glUniformMatrix4fv(glGetUniformLocation(mesh.meshShader.ID, "camMatrix"),
        1, GL_FALSE, r_camera.cameraMatrix.matrix[0]);

    glUniformMatrix4fv(glGetUniformLocation(mesh.meshShader.ID, "model"),
        1, GL_FALSE, ToMatrix4x4(mesh.meshTransform));

    vao.Bind();

    glDrawElements(GL_TRIANGLES,
        mesh.indices.size(),
        GL_UNSIGNED_INT,
        0);
}

float* RenderManager::ToMatrix4x4(Matrix3x4 p_mat3x4)
{
    static float out[4][4];

    out[0][0] = p_mat3x4.mat3[0][0];
    out[0][1] = p_mat3x4.mat3[0][1];
    out[0][2] = p_mat3x4.mat3[0][2];
    out[0][3] = p_mat3x4.origin[0];
    out[1][0] = p_mat3x4.mat3[1][0];
    out[1][1] = p_mat3x4.mat3[1][1];
    out[1][2] = p_mat3x4.mat3[1][2];
    out[1][3] = p_mat3x4.origin[1];
    out[2][0] = p_mat3x4.mat3[2][0];
    out[2][1] = p_mat3x4.mat3[2][1];
    out[2][2] = p_mat3x4.mat3[2][2];
    out[2][3] = p_mat3x4.origin[2];

    out[3][0] = 0;
    out[3][1] = 0;
    out[3][2] = 0;
    out[3][3] = 1;

    return &out[0][0];
}
