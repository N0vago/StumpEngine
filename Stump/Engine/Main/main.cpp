#include "Core/Layers/EngineModeLayer.h"
#include "Audio/AudioDevice.h"
#include "Audio/AudioBuffer.h"
#include "Audio/AudioListener.h"



int main()
{

	Rendering::RenderManager renderManager = Rendering::RenderManager();
	Physic::PhysicWorld physicWorld = Physic::PhysicWorld();

	Core::AppInfo appInfo;
	appInfo.Name = "Architecture";
	appInfo.WindowInfo.Width = 1920;
	appInfo.WindowInfo.Height = 1080;

	Audio::AudioDevice audioDevice = Audio::AudioDevice();
	Audio::AudioBuffer audioBuffer = Audio::AudioBuffer();
	Audio::AudioListener audioListener = Audio::AudioListener();
	Core::Application application(appInfo);

	application.PushLayer<Core::EngineModeLayer>();
	application.Run();
    return 0;
};