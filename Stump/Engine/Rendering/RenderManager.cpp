#include "Rendering/RenderManager.h"

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

    void RenderManager::Draw()
    {
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

    void RenderManager::AddToRender(RenderObject* p_renderObject) {
        if (!p_renderObject)
            return;
        renderObjects.push_back(p_renderObject);
    }

    void RenderManager::RemoveFromRender(RenderObject* p_renderObject) {
        renderObjects.erase(std::remove_if(renderObjects.begin(), renderObjects.end(),
            [p_renderObject](RenderObject* renderObject) {
                return renderObject == p_renderObject;
            }), renderObjects.end());
    }

    RenderManager& RenderManager::Get()
    {
        assert(Instance && "RenderManager instance does not exist!");
        return *Instance;
    }
}