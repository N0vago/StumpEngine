#ifndef ST_HIERARCHY_WINDOW_H
#define ST_HIERARCHY_WINDOW_H

#include "GUI/GuiWindow.h"
#include "Scene/Scene.h"

namespace GUI {

    class HierarchyWindow : public GuiWindow
    {
    public:
        HierarchyWindow(const std::string& r_name, Scene::STScene * p_scene);

        Scene::SceneNode* GetSelectedNode() const { return selectedNode; }

    protected:
        void OnDraw() override;

    private:
        Scene::STScene* scene = nullptr;
        Scene::SceneNode* selectedNode = nullptr;

        void DrawNode(Scene::SceneNode* node);
    };

}

#endif // ST_HIERARCHY_WINDOW_H
