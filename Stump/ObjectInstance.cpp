#include "ObjectInstance.h"
#include "RenderManager.h"

#include <array>
void ObjectInstance::OnRender()
{
	mesh->GetShader().SetMat4("camMatrix", camera.cameraMatrix.matrix[0], false, true);
	mesh->GetShader().SetMat4("model", transform.ToRenderMatrix().data(), true, true);
	mesh->GetShader().SetVec3("camPos", camera.Position, true);
	mesh->Draw();
}

void ObjectInstance::Update(float p_deltaTime)
{
	SceneNode::Update(p_deltaTime);

}

void ObjectInstance::OnAwake()
{
	SceneNode::OnAwake();
	RenderManager::Get().AddToRender(this);
}
void ObjectInstance::OnSleep()
{
	SceneNode::OnSleep();
	RenderManager::Get().RemoveFromRender(this);
}
