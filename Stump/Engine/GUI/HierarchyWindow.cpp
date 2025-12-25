#include "GUI/HierarchyWindow.h"

namespace GUI {
    HierarchyWindow::HierarchyWindow(const std::string& r_name,Scene::STScene* p_scene)
        : GuiWindow(r_name), scene(p_scene)
    {
    }

    void HierarchyWindow::OnDraw()
    {
        if (!scene || !scene->GetRoot())
            return;

        DrawNode(scene->GetRoot());
    }

    void HierarchyWindow::DrawNode(Scene::SceneNode* node)
    {
        if (!node)
            return;

        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_SpanAvailWidth;

        if (node == selectedNode)
            flags |= ImGuiTreeNodeFlags_Selected;

        if (node->children.empty())
            flags |= ImGuiTreeNodeFlags_Leaf;

        bool opened = ImGui::TreeNodeEx(
            (void*)node,
            flags,
            "%s",
            node->GetInfo().name.c_str()
        );
        // Selection
        if (ImGui::IsItemClicked())
        {
            selectedNode = node;
        }
        // Drag source
        if (node != scene->GetRoot())
        {
            if (ImGui::BeginDragDropSource())
            {
                Scene::SceneNode* payloadNode = node;
                ImGui::SetDragDropPayload(
                    "SCENE_NODE",
                    &payloadNode,
                    sizeof(Scene::SceneNode*)
                );

                ImGui::Text("%s", node->GetInfo().name.c_str());
                ImGui::EndDragDropSource();
            }
        }
        // Drop target
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload =
                ImGui::AcceptDragDropPayload("SCENE_NODE"))
            {
                Scene::SceneNode* dropped =
                    *(Scene::SceneNode**)payload->Data;

                if (dropped &&
                    dropped != node &&
                    dropped->GetParent() != node)
                {
                    scene->ReparentNode(dropped, node);
                }
            }
            ImGui::EndDragDropTarget();
        }

        // Context menu
        if (ImGui::BeginPopupContextItem())
        {
            if (node != scene->GetRoot())
            {
                if (ImGui::MenuItem("Delete"))
                {
                    scene->DestroyNode(node);

                    if (selectedNode == node)
                        selectedNode = nullptr;

                    ImGui::EndPopup();

                    if (opened)
                        ImGui::TreePop();

                    return;
                }
            }
            ImGui::EndPopup();
        }

        // Children
        if (opened)
        {
            for (auto& child : node->children)
            {
                DrawNode(child.get());
            }
            ImGui::TreePop();
        }
    }
}