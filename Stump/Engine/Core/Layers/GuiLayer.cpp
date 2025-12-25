#include "Core/Layers/GuiLayer.h"
#include "Core/Engine.h"
#include <ThirdParty/imgui/imgui_internal.h>
namespace Core {
	GuiLayer::GuiLayer()
	{
		logWindow = std::make_unique<GUI::LogWindow>("Logs");
		sceneView = std::make_unique<GUI::ViewportWindow>("Scene");
		hierarchyWindow = std::make_unique<GUI::HierarchyWindow>("Hierarchy",Core::Engine::Get().GetScene());
		fileSystemWindow = std::make_unique<GUI::FileSystemWindow>("File System", "Assets");

		windows.emplace_back(sceneView.get());
		windows.emplace_back(logWindow.get());
		windows.emplace_back(hierarchyWindow.get());
		windows.emplace_back(fileSystemWindow.get());
	}
	void GuiLayer::OnUpdate(float p_deltaTime)
	{
	}
	void GuiLayer::OnRender()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		// TOP TOOLBAR (Play / Stop)
		{
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, 32.0f));

			ImGuiWindowFlags toolbarFlags =
				ImGuiWindowFlags_NoDocking |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoScrollbar |
				ImGuiWindowFlags_NoSavedSettings;

			ImGui::Begin("##Toolbar", nullptr, toolbarFlags);

			auto& engine = Core::Engine::Get();
			bool isPlay = engine.GetEngineState() == EngineState::Play;

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 80.0f) * 0.5f);

			if (!isPlay)
			{
				if (ImGui::Button("Play", ImVec2(80, 22)))
					engine.SetEngineState(EngineState::Play);
			}
			else
			{
				if (ImGui::Button("Stop", ImVec2(80, 22)))
					engine.SetEngineState(EngineState::Editor);
			}

			ImGui::End();
		}
		// DOCKSPACE ROOT
		static bool dockspaceOpen = true;
		static bool dockLayoutInitialized = false;

		ImGuiWindowFlags dockspaceFlags =
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

		ImGui::SetNextWindowPos(
			ImVec2(viewport->Pos.x, viewport->Pos.y + 32.0f)
		);
		ImGui::SetNextWindowSize(
			ImVec2(viewport->Size.x, viewport->Size.y - 32.0f)
		);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin("DockSpaceRoot", &dockspaceOpen, dockspaceFlags);
		ImGui::PopStyleVar(2);

		ImGuiID dockspaceID = ImGui::GetID("MainDockSpace");
		ImGui::DockSpace(dockspaceID, ImVec2(0, 0));

		// DEFAULT DOCK LAYOUT (one-time)
		if (!dockLayoutInitialized)
		{
			dockLayoutInitialized = true;

			ImGuiID dockMain = dockspaceID;
			ImGuiID dockLeft, dockBottom, dockRight;

			ImGui::DockBuilderRemoveNode(dockMain);
			ImGui::DockBuilderAddNode(
				dockMain,
				ImGuiDockNodeFlags_DockSpace
			);
			ImGui::DockBuilderSetNodeSize(dockMain, viewport->Size);

			dockLeft = ImGui::DockBuilderSplitNode(
				dockMain,
				ImGuiDir_Left,
				0.22f,
				nullptr,
				&dockMain
			);

			dockBottom = ImGui::DockBuilderSplitNode(
				dockMain,
				ImGuiDir_Down,
				0.25f,
				nullptr,
				&dockMain
			);

			ImGui::DockBuilderDockWindow("Hierarchy", dockLeft);
			ImGui::DockBuilderDockWindow("Scene", dockMain);
			ImGui::DockBuilderDockWindow("File System", dockBottom);
			ImGui::DockBuilderDockWindow("Logs", dockBottom);

			ImGui::DockBuilderFinish(dockMain);
		}

		ImGui::End();

		// DRAW WINDOWS
		for (auto& window : windows)
			window->Draw();

		// RENDER
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// MULTI VIEWPORT SUPPORT
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup);
		}
	}
}