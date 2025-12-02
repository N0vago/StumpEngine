#include "Core/Application.h"


#include <GLFW/glfw3.h>

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
	}

	Application::~Application()
	{
		window->Destroy();

		glfwTerminate();

		Instance = nullptr;
	}

	void Application::Run() {
		
	}
}