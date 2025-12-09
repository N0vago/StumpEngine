#include "MeshInstance.h"

void MeshInstance::Update(float p_deltaTime)
{
	SceneNode::Update(p_deltaTime);

	mesh->GetShader().Activate();
	mesh->GetShader().SetMat4("camMatrix", camera.cameraMatrix.matrix[0], false);
	mesh->GetShader().SetMat4("model", GetTransform().ToRenderMatrix(), false);
	mesh->GetShader().SetVec3("camPos", camera.Position, false);
}

void MeshInstance::OnAwake()
{
	SceneNode::OnAwake();
	RenderManager::Get().AddToRender(mesh.get());
}
void MeshInstance::OnSleep()
{
	SceneNode::OnSleep();
	RenderManager::Get().RemoveFromRender(mesh.get());
}
