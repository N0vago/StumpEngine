#ifndef ST_ENGINE_LAYER_H
#define ST_ENGINE_LAYER_H


#include "Application.h"
#include "Camera.h"
#include "RenderManager.h"
#include "InputManager.h"
class EngineModeLayer : public Core::Layer
{
	std::unique_ptr<Camera> camera;
	std::unique_ptr<RenderManager> renderManager;
	std::unique_ptr<InputManager> inputManager;
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

