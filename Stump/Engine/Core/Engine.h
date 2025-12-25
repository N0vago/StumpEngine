#ifndef ST_ENGINE_H
#define ST_ENGINE_H

#include "Core/Layer.h"
#include "Core/Window.h"
#include "Core/Event.h"
#include "Scene/Scene.h"
#include "Math/Vector2.h"

#include <string>
#include <vector>
#include <memory>
#include <set>
#include <functional>
using namespace Math;
namespace Core {
	enum class EngineState {
		Editor,
		Play
	};
	struct EngineInfo {
		std::string Name = "Stump Engine";
		WindowInfo WindowInfo;
	};
	class Engine
	{
	public:
		Engine(const EngineInfo& r_appInfo = EngineInfo());
		~Engine();

		void Run();
		void Stop();

		void RaiseEvent(Event& r_event);

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		void PushLayer() {
			layerStack.push_back(std::make_unique<TLayer>());
		}

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		TLayer* GetLayer()
		{
			for (const std::unique_ptr<Layer>& layer : layerStack) {
				if (TLayer* castedLayer = dynamic_cast<TLayer*>(layer.get())) {
					return castedLayer;
				}
			}
			return nullptr;
		}

		void SetScene(std::shared_ptr<Scene::STScene> p_scene) { initialScene = std::move(p_scene); }

		Scene::STScene* GetScene() { return initialScene.get(); }

		void SetEngineState(EngineState p_state);
		EngineState GetEngineState() const { return engineState; }

		Vector2 GetFrameBufferSize() const;

		std::shared_ptr<Window> GetWindow() const { return window; }

		static Engine& Get();

		static float GetTime();
	private:
		EngineInfo appInfo;
		std::shared_ptr<Window> window;
		bool running = false;

		std::shared_ptr<Scene::STScene> initialScene;

		EngineState engineState = EngineState::Editor;

		std::vector<std::unique_ptr<Layer>> layerStack;

		friend class Layer;
	};
}
#endif // ST_ENGINE_H
