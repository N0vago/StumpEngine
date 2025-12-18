#include "Core/Layers/EngineModeLayer.h"

int main()
{

	Rendering::RenderManager renderManager = Rendering::RenderManager();
	Physic::PhysicWorld physicWorld = Physic::PhysicWorld();

	Core::AppInfo appInfo;
	appInfo.Name = "Architecture";
	appInfo.WindowInfo.Width = 1920;
	appInfo.WindowInfo.Height = 1080;

	Core::Application application(appInfo);
	application.PushLayer<Core::EngineModeLayer>();
	application.Run();
    return 0;
};