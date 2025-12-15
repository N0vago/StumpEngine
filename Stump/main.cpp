#include "Application.h"
#include "EngineModeLayer.h"

int main()
{
	RenderManager renderManager = RenderManager();
	PhysicWorld physicWorld = PhysicWorld();

	Core::AppInfo appInfo;
	appInfo.Name = "Architecture";
	appInfo.WindowInfo.Width = 1920;
	appInfo.WindowInfo.Height = 1080;

	Core::Application application(appInfo);
	application.PushLayer<EngineModeLayer>();
	application.Run();
    return 0;
};