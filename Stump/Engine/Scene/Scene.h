#ifndef ST_SCENE_H
#define ST_SCENE_H

#include "Scene/SceneNode.h"
#include <memory>
#include <vector>

namespace Scene {
	class STScene {
		std::unique_ptr<SceneNode> root;
	public:
        STScene();
        ~STScene();

        void Update(float p_deltaTime);
        void EnableNodes();
        void DisableNodes();

        template<std::derived_from<SceneNode> T, typename... Args>
        std::shared_ptr<T> CreateNode(const std::string& r_name, Args&&... r_args)
        {
            auto node = std::make_shared<T>(ObjectInfo(GenerateID(), r_name), this, std::forward<Args>(r_args)...);

            root->AddChild(node);

            return node;
        }

        void DestroyNode(SceneNode* p_node);
        void ReparentNode(SceneNode* p_child, SceneNode* p_newParent);

        template<std::derived_from<SceneNode> T>
        T* FindByName(const std::string& r_name)
        {
            auto node = root.get();
            if (!node) return nullptr;

            if (node->GetInfo().name == r_name)
                if (auto casted = dynamic_cast<T*>(node))
                    return casted;

            for (auto& child : node->children)
                if (auto found = FindByName<T>(child.get(), r_name))
                    return found;

            return nullptr;
        }
        template<typename Func>
        void ForEachNode(Func&& r_fn)
        {
            if (!root) return;
            ForEachNodeImpl(*root, r_fn);
        }

        SceneNode* GetRoot() { return root.get(); }
    private:
        template<typename Func>
        void ForEachNodeImpl(SceneNode& r_node, Func& r_fn)
        {
            r_fn(r_node);
            for (auto& child : r_node.children)
            {
                ForEachNodeImpl(*child, r_fn);
            }
        }
        int GenerateID();
	};
}
#endif //ST_SCENE_H