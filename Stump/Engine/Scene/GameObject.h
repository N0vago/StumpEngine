#ifndef ST_GAME_OBJECT_H
#define ST_GAME_OBJECT_H

#include "Scene/SceneNode.h"
#include <memory>
#include <vector>
#include <type_traits>
namespace Components {
    class Component;
}
namespace Scene {
	class GameObject : public SceneNode
	{
    public:
        GameObject(const ObjectInfo& r_info, STScene* p_owner);
        ~GameObject() override;

        
        template<std::derived_from<Components::Component> T, typename... Args>
        T* AddComponent(Args&&... r_args)
        {
            auto comp = std::make_shared<T>(std::forward<Args>(r_args)...);
            T* raw = comp.get();

            raw->owner = this;

            components.push_back(std::move(comp));

            raw->OnCreate();

            return raw;
        }

        template<std::derived_from<Components::Component> T>
        T* GetComponent()
        {
            for (auto& comp : components)
            {
                if (auto casted = dynamic_cast<T*>(comp.get()))
                    return casted;
            }
            return nullptr;
        }

        template<std::derived_from<Components::Component> T>
        bool HasComponent() const
        {
            for (const auto& comp : components)
            {
                if (dynamic_cast<T*>(comp.get()))
                    return true;
            }
            return false;
        }

        void RemoveComponent(Components::Component* p_component);

        // lifecycle override
        void OnEnable() override;
        void OnDisable() override;
        void Update(float p_deltaTime) override;

    private:
        std::vector<std::shared_ptr<Components::Component>> components;
	};
    
}
#endif //ST_GAME_OBJECT_H