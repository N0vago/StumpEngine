#include "RenderManager.h"

#include <assert.h>
#include <algorithm>

static RenderManager* Instance = nullptr;

RenderManager::RenderManager()
{
    Instance = this;
}

RenderManager::~RenderManager()
{
	Instance = nullptr;
}

void RenderManager::DrawMeshes()
{

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.26f, 0.11f, 0.32f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
    for(auto& mesh : meshes)
    {
        if (meshes.size() == 0) break;

		mesh->OnRender();
    }

}

void RenderManager::AddToRender(MeshInstance* p_mesh) {
    if (!p_mesh)
        return;
    meshes.push_back(p_mesh);
}

void RenderManager::RemoveFromRender(MeshInstance* p_mesh) {
    meshes.erase(std::remove_if(meshes.begin(), meshes.end(),
        [p_mesh](MeshInstance* mesh) {
            return mesh == p_mesh;
        }), meshes.end());
}

RenderManager& RenderManager::Get()
{
    assert(Instance && "RenderManager instance does not exist!");
    return *Instance;
}
