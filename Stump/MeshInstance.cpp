#include "MeshInstance.h"
#include "RenderManager.h"

#include <array>
void MeshInstance::OnRender()
{
	mesh->GetShader().SetMat4("camMatrix", camera.cameraMatrix.matrix[0], false, true);
	mesh->GetShader().SetMat4("model", GetTransform().ToRenderMatrix().data(), true, true);
	mesh->GetShader().SetVec3("camPos", camera.Position, true);
	mesh->Draw();
}

void MeshInstance::Update(float p_deltaTime)
{
	SceneNode::Update(p_deltaTime);

}

void MeshInstance::OnAwake()
{
	SceneNode::OnAwake();
	RenderManager::Get().AddToRender(this);
}
void MeshInstance::OnSleep()
{
	SceneNode::OnSleep();
	RenderManager::Get().RemoveFromRender(this);
}
