#ifndef ST_MESH_RENDERER_COMPONENT_H
#define ST_MESH_RENDERER_COMPONENT_H

#include "Components/Component.h"
#include "Rendering/RenderManager.h"

namespace Components {
	class MeshRendererComponent : public Component {
		Rendering::RenderUnit renderUnit;
		bool registered = false;
	public:

		MeshRendererComponent();
		~MeshRendererComponent() override;

		ComponentInfo GetInfo() const override
		{
			return {
				"MeshRenderer",
				"Rendering",
				false,
				true
			};
		}

		void OnEnable() override;
		void OnDisable() override;
		void Update(float dt) override;

		void SetMesh(std::shared_ptr<Rendering::Mesh> mesh);
		void SetMaterial(std::shared_ptr<Rendering::Material> material);
	};
}
#endif //ST_MESH_RENDERER_COMPONENT_H