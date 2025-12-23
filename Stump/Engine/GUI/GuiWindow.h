#ifndef ST_GUI_WINDOW_H
#define ST_GUI_WINDOW_H
#include <ThirdParty/imgui/imconfig.h>
#include <ThirdParty/imgui/backends/imgui_impl_opengl3.h>
#include <ThirdParty/imgui/backends/imgui_impl_glfw.h>
#include <string>
namespace GUI {
	class GuiWindow {
    public:
        GuiWindow(std::string name)
            : windowName(std::move(name)) {
        }

        virtual ~GuiWindow() = default;

        void Draw()
        {
            if (!isOpen)
                return;

            OnBegin();
            OnDraw();
            OnEnd();
        }

        bool IsOpen() const { return isOpen; }

    protected:
        std::string windowName;
        bool isOpen = true;

        virtual void OnBegin()
        {
            ImGui::Begin(windowName.c_str(), &isOpen);
        }

        virtual void OnDraw() = 0;

        virtual void OnEnd()
        {
            ImGui::End();
        }
	};
}
#endif //ST_GUI_WINDOW_H