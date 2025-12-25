#ifndef ST_PLAY_MODE_LAYER_H
#define ST_PLAY_MODE_LAYER_H

#include "Core/Layer.h"
#include "Rendering/Camera.h"
#include "Scene/Scene.h"

namespace Core {
	class PlayModeLayer : public Layer {
    private:
        std::shared_ptr<Rendering::Camera> gameCamera;
        std::shared_ptr<Scene::STScene> scene;

    public:
        PlayModeLayer(Scene::STScene* p_scene);
        virtual void OnUpdate(float p_deltaTime) override;
        virtual void OnRender() override;
        virtual void OnEvent(Event& r_event) override;
    };
}

#endif //ST_PLAY_MODE_LAYER_H