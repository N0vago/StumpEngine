#include "Rendering/RenderManager.h"

#include <ThirdParty/imgui/imconfig.h>
#include <ThirdParty/imgui/backends/imgui_impl_opengl3.h>
#include <ThirdParty/imgui/backends/imgui_impl_glfw.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <assert.h>
#include <algorithm>
#include <array>
namespace Rendering {
    static RenderManager* Instance = nullptr;

    RenderManager::RenderManager()
    {
        Instance = this;
    }

    RenderManager::~RenderManager()
    {
        Instance = nullptr;
    }

    void RenderManager::DrawCall()
    {
        if (!sceneFBO)
            return;

        glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);
        glViewport(0, 0, viewportWidth, viewportHeight);

        RenderScene();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void RenderManager::ResizeViewport(uint32_t p_w, uint32_t p_h) {
        if (p_w == 0 || p_h == 0)
            return;

        if (viewportWidth == p_w && viewportHeight == p_h)
            return;

        viewportWidth = p_w;
        viewportHeight = p_h;

        if (!sceneFBO)
            glGenFramebuffers(1, &sceneFBO);

        glBindFramebuffer(GL_FRAMEBUFFER, sceneFBO);

        // Color texture
        if (!sceneColorTex)
            glGenTextures(1, &sceneColorTex);

        glBindTexture(GL_TEXTURE_2D, sceneColorTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, p_w, p_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            sceneColorTex,
            0
        );

        // Depth
        if (!sceneDepthRBO)
            glGenRenderbuffers(1, &sceneDepthRBO);

        glBindRenderbuffer(GL_RENDERBUFFER, sceneDepthRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, p_w, p_h);

        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT,
            GL_RENDERBUFFER,
            sceneDepthRBO
        );

        activeCamera->SetViewPortSize(viewportWidth, viewportHeight);

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void RenderManager::AddToRender(RenderUnit* p_renderObject) {
        if (!p_renderObject)
            return;
        renderObjects.push_back(p_renderObject);
    }

    void RenderManager::RemoveFromRender(RenderUnit* p_renderObject) {
        renderObjects.erase(std::remove_if(renderObjects.begin(), renderObjects.end(),
            [p_renderObject](RenderUnit* renderObject) {
                return renderObject == p_renderObject;
            }), renderObjects.end());
    }

    void RenderManager::RenderScene() {
        if (!activeCamera)
            return;

        glEnable(GL_DEPTH_TEST);

        glClearColor(0.26f, 0.11f, 0.32f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& renderObject : renderObjects)
        {
            if (renderObjects.size() == 0) break;

            renderObject->material->Bind();

            renderObject->material->GetShader()->SetMat4("camMatrix", activeCamera->cameraMatrix.ToArray(), false, true);
            renderObject->material->GetShader()->SetVec3("camPos", activeCamera->Position, true);
            renderObject->material->GetShader()->SetMat4("model", renderObject->modelMatrix.ToRenderMatrix(), true, true);

            renderObject->mesh->Draw();

        }
    }

    RenderManager& RenderManager::Get()
    {
        assert(Instance && "RenderManager instance does not exist!");
        return *Instance;
    }
}