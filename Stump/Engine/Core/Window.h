#ifndef ST_WINDOW_H
#define ST_WINDOW_H

#include "Core/Event.h"
#include "Math/Vector2.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <string>
#include <iostream>

using namespace Math;

namespace Core {
	struct WindowInfo
	{
		std::string Title = "Stump Engine";
		uint16_t Width = 1280;
		uint16_t Height = 720;

		bool IsResizable = true;
		bool VSync = true;

		using EventCallbackFunc = std::function<void(Event&)>;
		EventCallbackFunc EventCallback;
	};
	class Window
	{
	public:
		Window(const WindowInfo& r_windowInfo = WindowInfo());

		~Window();

		void Create();

		void Destroy();

		void Update();

		void RaiseEvent(Event& r_event);

		Vector2 GetFrameBufferSize() const;
		Vector2 GetMousePos() const;

		bool ShouldClose() const;

		GLFWwindow* GetHandle() const { return windowHandle; }

	private:

		WindowInfo windowInfo;

		GLFWwindow* windowHandle = nullptr;
	};
}
#endif // ST_WINDOW_H
