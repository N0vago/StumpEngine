#ifndef ST_MESH_INSTANCE_H
#define ST_MESH_INSTANCE_H
#include "Rendering/RenderManager.h"
#include "Scene/SceneNode.h"

#include <memory>
using namespace Rendering;
namespace Scene {
	class MeshInstance : public SceneNode
	{
	public:
		std::shared_ptr<RenderObject> renderObject;

		MeshInstance(const ObjectInfo& r_objectInfo, std::shared_ptr<RenderObject> p_renderObject);

		virtual void OnAwake() override;

		virtual void Update(float p_deltaTime) override;

		virtual void OnSleep() override;
	};
}
#endif

