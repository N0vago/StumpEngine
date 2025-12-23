#ifndef ST_RENDER_MANAGER_H
#define ST_RENDER_MANAGER_H

#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Rendering/Camera.h"
#include "Math/Matrix3x4.h"

#include <iostream>
#include <memory>
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

		uint32_t sceneFBO = 0;
		uint32_t sceneColorTex = 0;
		uint32_t sceneDepthRBO = 0;

		uint32_t viewportWidth = 1280;
		uint32_t viewportHeight = 720;

	public:

		RenderManager();
		~RenderManager();

		void DrawCall();

		void ResizeViewport(uint32_t p_w, uint32_t p_h);

		void AddToRender(RenderUnit* p_mesh);

		void RemoveFromRender(RenderUnit* p_mesh);

		void SetActiveCamera(std::shared_ptr<Camera> p_camera) { activeCamera = std::move(p_camera); }

		uint32_t GetSceneTexture() const { return sceneColorTex; }

		static RenderManager& Get();
	private:
		void RenderScene();
	};
}

#endif