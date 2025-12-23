#ifndef ST_VIEWPORT_WINDOW_H
#define ST_VIEWPORT_WINDOW_H

#include "GUI/GuiWindow.h"
#include "Rendering/RenderManager.h"
namespace GUI {
	class ViewportWindow : public GuiWindow {
    public:
        ViewportWindow(const std::string& name)
            : GuiWindow(name) {
        }

        bool IsActive() const { return viewportActive; }
        ImVec2 GetSize() const { return viewportSize; }

    protected:
        bool viewportActive = false;
        ImVec2 viewportSize = { 0, 0 };

        virtual void OnDraw() override;
	};
}
#endif //ST_VIEWPORT_WINDOW_H