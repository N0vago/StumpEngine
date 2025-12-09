#include "RenderManager.h"

#include <assert.h>

static RenderManager* Instance = nullptr;

RenderManager::RenderManager(Camera& r_camera) : camera(r_camera)
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
		mesh->Draw();
    }

}

void RenderManager::AddToRender(const Mesh* p_mesh) {

    meshes.push_back(std::make_unique<Mesh>(p_mesh));
}

void RenderManager::RemoveFromRender(const Mesh* p_mesh) {
    meshes.erase(std::remove_if(meshes.begin(), meshes.end(),
        [p_mesh](const std::unique_ptr<Mesh>& mesh) {
            return mesh.get() == p_mesh;
        }), meshes.end());
}

RenderManager& RenderManager::Get()
{
    assert(Instance && "RenderManager instance does not exist!");
    return *Instance;
}
