#ifndef ST_GUI_LAYER_H
#define ST_GUI_LAYER_H

#include "Core/Layer.h"
#include "GUI/ViewportWindow.h"
#include "GUI/LogWindow.h"
#include "GUI/HierarchyWindow.h"
#include "GUI/FileSystemWindow.h"
namespace Core {
	class GuiLayer : public Layer {
		std::unique_ptr<GUI::ViewportWindow> sceneView;
		std::unique_ptr<GUI::LogWindow> logWindow;
		std::unique_ptr<GUI::HierarchyWindow> hierarchyWindow;
		std::unique_ptr<GUI::FileSystemWindow> fileSystemWindow;
		std::vector<std::unique_ptr<GUI::GuiWindow>> windows;

	public:
		GuiLayer();
		virtual void OnUpdate(float p_deltaTime) override;
		virtual void OnRender() override;


		GUI::ViewportWindow* GetViewportWindow() { return sceneView.get(); }
		GUI::LogWindow* GetLogWindow() { return logWindow.get(); }
		GUI::HierarchyWindow* GetHierarchyWindow() { return hierarchyWindow.get(); }
		GUI::FileSystemWindow* GetFileSystemWindow() { return fileSystemWindow.get(); }
	};
}
#endif // !ST_GUI_LAYER_H
