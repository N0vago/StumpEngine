#include "MeshInstance.h"

void MeshInstance::Update(float p_deltaTime)
{

}

void MeshInstance::OnAwake()
{
	RenderManager::Get().AddToRender(mesh.get());
}
void MeshInstance::OnSleep()
{
	RenderManager::Get().RemoveFromRender(mesh.get());
}
