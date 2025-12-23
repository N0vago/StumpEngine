#ifndef ST_ENGINE_LAYER_H
#define ST_ENGINE_LAYER_H


#include "Core/Application.h"
#include "Core/InputManager.h"
#include "Rendering/RenderManager.h"
#include "Physic/PhysicWorld.h"
#include "Scene/Scene.h"
#include "Audio/AudioSource.h"
#include "GUI/ViewportWindow.h"
using namespace Core;
using namespace Rendering;
using namespace Physic;
using namespace Scene;

namespace Core {
	class EngineModeLayer : public Core::Layer
	{
		std::unique_ptr<InputManager> inputManager;
		std::unique_ptr<STScene> scene;

		std::shared_ptr<Camera> editorCamera;
		std::shared_ptr<Shader> defaultShader;
		std::shared_ptr<Shader> lightShader;

		std::unique_ptr<GUI::ViewportWindow> sceneView;
		std::vector<std::unique_ptr<GUI::GuiWindow>> windows;
	public:
		EngineModeLayer();
		virtual ~EngineModeLayer();

		virtual void OnUpdate(float p_ts) override;
		virtual void OnRender() override;
		virtual void OnEvent(Core::Event& event) override;

	private:

		void CameraMove(float& r_deltaTime);

		void CameraRotate(float& r_deltaTime);

	};
}
#endif // ST_ENGINE_LAYER_H

