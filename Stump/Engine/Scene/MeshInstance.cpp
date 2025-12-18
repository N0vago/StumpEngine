#include "Scene/MeshInstance.h"
namespace Scene {
	MeshInstance::MeshInstance(const ObjectInfo& r_objectInfo, std::shared_ptr<RenderObject> p_renderObject) : SceneNode(r_objectInfo), renderObject(std::move(p_renderObject))
	{
	}

	void MeshInstance::OnAwake()
	{
		RenderManager::Get().AddToRender(renderObject.get());
	}

	void MeshInstance::Update(float p_deltaTime)
	{
	}

	void MeshInstance::OnSleep()
	{
		RenderManager::Get().RemoveFromRender(renderObject.get());
	}
}