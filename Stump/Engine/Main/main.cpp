#include "Core/Layers/EditorModeLayer.h"
#include "Core/Layers/GuiLayer.h"
#include "Audio/AudioDevice.h"
#include "Audio/AudioBuffer.h"
#include "Audio/AudioListener.h"




int main()
{
	Rendering::RenderManager renderManager = Rendering::RenderManager();
	Physic::PhysicWorld physicWorld = Physic::PhysicWorld();
	Audio::AudioDevice audioDevice = Audio::AudioDevice();
	Audio::AudioBuffer audioBuffer = Audio::AudioBuffer();
	Audio::AudioListener audioListener = Audio::AudioListener();
	Debug::Logger logger = Debug::Logger();



	Core::EngineInfo engineInfo;
	engineInfo.Name = "Stump Engine";
	engineInfo.WindowInfo.Width = 1920;
	engineInfo.WindowInfo.Height = 1080;
	engineInfo.WindowInfo.VSync = false;
	Core::Engine engine(engineInfo);

	engine.PushLayer<Core::GuiLayer>();
	engine.PushLayer<Core::EditorModeLayer>();
	engine.Run();
    return 0;
};