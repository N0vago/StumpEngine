#ifndef ST_APPLICATION_H
#define ST_APPLICATION_H

#include "Layer.h"
#include "Window.h"
#include "Event.h"
#include "Vector2.h"

#include <string>
#include <vector>
#include <memory>
#include <set>
#include <functional>
namespace Core {
	struct AppInfo {
		std::string Name = "Stump Engine";
		WindowInfo WindowInfo;
	};
	class Application
	{
	public:
		Application(const AppInfo& r_appInfo = AppInfo());
		~Application();

		void Run();
		void Stop();

		void RaiseEvent(Event& event);

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		void PushLayer();

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		TLayer* GetLayer();

		Vector2 GetFrameBufferSize() const;

		std::shared_ptr<Window> GetWindow() const;

		static Application& Get();

		static float GetTime();
	private:
		AppInfo appInfo;
		std::shared_ptr<Window> window;
		bool m_Running = false;

		std::vector<std::unique_ptr<Layer>> layerStack;

		friend class Layer;
	};
	#endif // ST_APPLICATION_H
}
