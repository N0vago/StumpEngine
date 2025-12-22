#ifndef ST_RENDER_MANAGER_H
#define ST_RENDER_MANAGER_H

#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Rendering/Camera.h"
#include "Math/Matrix3x4.h"

#include <iostream>
#include <memory>
#include <GLFW/glfw3.h>
namespace Rendering {
	struct RenderUnit
	{
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> material;
		Matrix3x4 modelMatrix;
	};

	class RenderManager
	{
		std::vector<RenderUnit*> renderObjects;

		std::shared_ptr<Camera> activeCamera;

	public:

		RenderManager();
		~RenderManager();

		void Draw();

		void AddToRender(RenderUnit* p_mesh);

		void RemoveFromRender(RenderUnit* p_mesh);

		void SetActiveCamera(std::shared_ptr<Camera> p_camera) { activeCamera = std::move(p_camera); }

		static RenderManager& Get();

	};
}

#endif