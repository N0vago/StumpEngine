#ifndef ST_COMPONENT_H
#define ST_COMPONENT_H
#include "Scene/GameObject.h"
namespace Components {

    struct ComponentInfo
    {
        const char* name;
        const char* category;
        bool allowMultiple;
        bool removable;
    };

	class Component {
    public:
        Component() : info(ComponentInfo{"Script", "Scripts", true, true}) {}
        Component(const ComponentInfo& r_info) : info(r_info) {}
        virtual ~Component() = default;

        Scene::GameObject* GetOwner() const { return owner; }

        virtual ComponentInfo GetInfo() const = 0;

        virtual void OnCreate() {}
        virtual void OnDestroy() {}

        virtual void OnEnable() {}
        virtual void OnDisable() {}

        virtual void Update(float dt) {}

    protected:
        Scene::GameObject* owner = nullptr;
        ComponentInfo info;

    private:
        friend class Scene::GameObject;
	};
}

#endif //ST_COMPONENT_H