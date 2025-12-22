#ifndef ST_AUDIO_SOURCE_COMPONENT_H
#define ST_AUDIO_SOURCE_COMPONENT_H

#include "Components/Component.h"
#include "Audio/AudioSource.h"
#include "Audio/AudioBuffer.h"

#include <memory>
namespace Components {
	class AudioSourceComponent : public Component {
		//TODO: create soundFileName field and update sound buffer based on value of this field
		std::unique_ptr<Audio::AudioSource> source;
	public:
		AudioSourceComponent();
		AudioSourceComponent(const char* p_soundFileName);
		~AudioSourceComponent();

		void Play() { source->Play(); }

		void Pause() { source->Pause(); }

		void Stop() { source->Stop(); }

		bool IsPlaying() const { return source->IsPlaying(); }

		void SetPich(float p_pitch) { source->SetPich(p_pitch); }

		void SetGain(float p_gain) { source->SetGain(p_gain); }

		void SetPosition(Math::Vector3 p_position) { source->SetPosition(p_position); }

		void SetVelocity(Math::Vector3 p_velocity) { source->SetVelocity(p_velocity); }

		void SetLoopSound(bool p_isLooping) { source->SetLoopSound(p_isLooping); }

		void SetSound(const char* p_soundFileName) { source->SetSound(p_soundFileName); }

	};
}
#endif //ST_AUDIO_SOURCE_COMPONENT_H