#include "Core/Engine.h"

#include <ThirdParty/imgui/imconfig.h>
#include <ThirdParty/imgui/backends/imgui_impl_opengl3.h>
#include <ThirdParty/imgui/backends/imgui_impl_glfw.h>

#include "Core/Layers/EditorModeLayer.h"
#include "Core/Layers/PlayModeLayer.h"

#include <assert.h>
#include <iostream>
#include <ranges>

namespace Core
{

	static Engine* Instance = nullptr;

	static void GLFWErrorCallback(int p_error, const char* p_description)
	{
		std::cerr << "GLFW Error (" << p_error << "): " << p_description << std::endl;
	}

	Engine::Engine(const EngineInfo& r_appInfo) : appInfo(r_appInfo)
	{
		Instance = this;

		glfwSetErrorCallback(GLFWErrorCallback);
		glfwInit();

		if (appInfo.WindowInfo.Title.empty())
			appInfo.WindowInfo.Title = appInfo.Name;

		appInfo.WindowInfo.EventCallback = [this](Event& event) { this->RaiseEvent(event); };
		window = std::make_shared<Window>(appInfo.WindowInfo);
		window->Create();

		initialScene = std::make_unique<Scene::STScene>();

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

	Engine::~Engine()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		window->Destroy();

		glfwTerminate();

		Instance = nullptr;
	}

	void Engine::Run() {
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

	void Engine::Stop() {
		running = false;
	}

	void Engine::RaiseEvent(Event& r_event) {
		for (auto& layer : std::views::reverse(layerStack)) {
			layer->OnEvent(r_event);
			if (r_event.Handled)
				break;
		}
	}

	void Engine::SetEngineState(EngineState p_state) {
		if (engineState == p_state)
			return;

		engineState = p_state;

		auto editorLayer = GetLayer<EditorModeLayer>();
		if (p_state == EngineState::Play)
		{
			GetLayer<EditorModeLayer>()->TransitionTo<PlayModeLayer>(initialScene.get());
		}
		else
		{
			
			GetLayer<PlayModeLayer>()->TransitionTo<EditorModeLayer>();
			
		}
	}

	Vector2 Engine::GetFrameBufferSize() const {
		return window->GetFrameBufferSize();
	}

	Engine& Engine::Get() {
		assert(Instance && "Application instance does not exist!");
		return *Instance;
	}

	float Engine::GetTime() {
		return (float)glfwGetTime();
	}


}