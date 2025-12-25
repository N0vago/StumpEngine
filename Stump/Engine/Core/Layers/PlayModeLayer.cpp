#include "Core/Layers/PlayModeLayer.h"

#include "Rendering/RenderManager.h"
#include "Physic/PhysicWorld.h"
#include "Audio/AudioListener.h"
namespace Core {
    PlayModeLayer::PlayModeLayer(Scene::STScene* scene) : scene(scene) {
        
        gameCamera = std::make_shared<Rendering::Camera>(Rendering::Camera(1280, 720, Vector3(0.0f, 0.0f, 2.0f)));
        Rendering::RenderManager::Get().SetActiveCamera(gameCamera);
    }

    void PlayModeLayer::OnUpdate(float p_deltaTime) {
        Physic::PhysicWorld::Get().StepSimulation(p_deltaTime);
        scene->Update(p_deltaTime);


        Audio::AudioListener* listener = Audio::AudioListener::Get();
        listener->SetPosition(gameCamera->Position);
        listener->SetOrientation(gameCamera->Orientation, gameCamera->Up);
        
    }

    void PlayModeLayer::OnRender() {
        Rendering::RenderManager::Get().DrawCall();
    }

    void PlayModeLayer::OnEvent(Event& e) {
        
    }
}