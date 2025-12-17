#ifndef ST_ENGINE_LAYER_H
#define ST_ENGINE_LAYER_H


#include "Application.h"
#include "RenderManager.h"
#include "PhysicWorld.h"
#include "InputManager.h"

class SceneNode;
class EngineModeLayer : public Core::Layer
{
	std::shared_ptr<Camera> camera;
	std::unique_ptr<InputManager> inputManager;

	std::unique_ptr<SceneNode> sceneRoot;

	std::shared_ptr<Shader> defaultShader;
	std::shared_ptr<Shader> lightShader;
public:
	EngineModeLayer();
	virtual ~EngineModeLayer();

	virtual void OnUpdate(float p_ts) override;
	virtual void OnRender() override;
	virtual void OnEvent(Core::Event& event) override;

private:

	void CameraMove(float& r_deltaTime);

	void CameraRotate(float& r_deltaTime);
	
};
#endif // ST_ENGINE_LAYER_H

