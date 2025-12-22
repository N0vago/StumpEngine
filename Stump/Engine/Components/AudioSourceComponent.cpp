#include "Components/AudioSourceComponent.h"

namespace Components {
	AudioSourceComponent::AudioSourceComponent() : 
		Component(ComponentInfo{ .name = "AudioSource", .category = "Audio", .allowMultiple = true, .removable = true}),
		source(std::move(std::make_unique<Audio::AudioSource>()))
	{

	}
	AudioSourceComponent::AudioSourceComponent(const char* p_soundFileName) :
		Component(ComponentInfo{ .name = "AudioSource", .category = "Audio", .allowMultiple = true, .removable = true }),
		source(std::move(std::make_unique<Audio::AudioSource>()))
	{
		source->SetSound(p_soundFileName);
	}
	AudioSourceComponent::~AudioSourceComponent()
	{
	}
}