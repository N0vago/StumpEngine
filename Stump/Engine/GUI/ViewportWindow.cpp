#include "GUI/ViewportWindow.h"

namespace GUI {
	void ViewportWindow::OnDraw() {
        viewportSize = ImGui::GetContentRegionAvail();

        if (viewportSize.x <= 0 || viewportSize.y <= 0)
            return;

        Rendering::RenderManager::Get().ResizeViewport(
            (uint32_t)viewportSize.x,
            (uint32_t)viewportSize.y
        );

        ImGui::Image(
            (ImTextureID)(intptr_t)Rendering::RenderManager::Get().GetSceneTexture(),
            viewportSize,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        bool hovered = ImGui::IsItemHovered();
        bool focused = ImGui::IsWindowFocused();

        viewportActive = hovered && focused;
	}
}