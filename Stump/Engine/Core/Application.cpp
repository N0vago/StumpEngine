#include "Core/Application.h"

#include <ThirdParty/imgui/imconfig.h>
#include <ThirdParty/imgui/backends/imgui_impl_opengl3.h>
#include <ThirdParty/imgui/backends/imgui_impl_glfw.h>

#include <assert.h>
#include <iostream>
#include <ranges>

namespace Core
{

	static Application* Instance = nullptr;

	static void GLFWErrorCallback(int p_error, const char* p_description)
	{
		std::cerr << "GLFW Error (" << p_error << "): " << p_description << std::endl;
	}

	Application::Application(const AppInfo& r_appInfo) : appInfo(r_appInfo)
	{
		Instance = this;

		glfwSetErrorCallback(GLFWErrorCallback);
		glfwInit();

		if (appInfo.WindowInfo.Title.empty())
			appInfo.WindowInfo.Title = appInfo.Name;

		appInfo.WindowInfo.EventCallback = [this](Event& event) { this->RaiseEvent(event); };
		window = std::make_shared<Window>(appInfo.WindowInfo);
		window->Create();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window->GetHandle(), true);
		ImGui_ImplOpenGL3_Init();
	}

	Application::~Application()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		window->Destroy();

		glfwTerminate();

		Instance = nullptr;
	}

	void Application::Run() {
		running = true;

		float lastFrameTime = GetTime();

		while (running) {
			glfwPollEvents();

			if (window->ShouldClose()) {
				Stop();
				break;
			}

			float currentFrameTime = GetTime();
			float DeltaTime = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

			for (const std::unique_ptr<Layer>& layer : layerStack)
				layer->OnUpdate(DeltaTime);

			for (const std::unique_ptr<Layer>& layer : layerStack)
				layer->OnRender();

			window->Update();
		}
	}

	void Application::Stop() {
		running = false;
	}

	void Application::RaiseEvent(Event& r_event) {
		for (auto& layer : std::views::reverse(layerStack)) {
			layer->OnEvent(r_event);
			if (r_event.Handled)
				break;
		}
	}

	Vector2 Application::GetFrameBufferSize() const {
		return window->GetFrameBufferSize();
	}

	Application& Application::Get() {
		assert(Instance && "Application instance does not exist!");
		return *Instance;
	}

	float Application::GetTime() {
		return (float)glfwGetTime();
	}


}