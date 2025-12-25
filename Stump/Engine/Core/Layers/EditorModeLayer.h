#ifndef ST_EDITOR_LAYER_H
#define ST_EDITOR_LAYER_H


#include "Core/Engine.h"
#include "Core/InputManager.h"
#include "Rendering/RenderManager.h"
#include "Physic/PhysicWorld.h"
#include "Scene/Scene.h"
#include "Audio/AudioSource.h"
#include "Debug/Logger.h"

using namespace Core;
using namespace Rendering;
using namespace Physic;
using namespace Scene;

namespace Core {
	class EditorModeLayer : public Core::Layer
	{
		std::shared_ptr<Camera> editorCamera;
		std::unique_ptr<InputManager> inputManager;
		STScene* scene;
	public:
		EditorModeLayer();
		virtual ~EditorModeLayer();

		virtual void OnUpdate(float p_ts) override;
		virtual void OnRender() override;
		virtual void OnEvent(Core::Event& event) override;

	private:

		void CameraMove(float& r_deltaTime);

		void CameraRotate(float& r_deltaTime);

	};
}
#endif // ST_EDITOR_LAYER_H

