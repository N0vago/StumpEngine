#ifndef ST_AUDIO_SOURCE_H
#define ST_AUDIO_SOURCE_H
#include <AL\al.h>
#include <inttypes.h>
#include "Audio/AudioBuffer.h"
#include "Math/Vector3.h"
namespace Audio {
	class AudioSource {
		uint32_t source;
		uint32_t buffer = 0;
		float pitch;
		float gain;
		Math::Vector3 position;
		Math::Vector3 velocity;
		bool loopSound = false;
	public:
		AudioSource();
		~AudioSource();

		void Play();

		void Pause();

		void Stop();

		bool IsPlaying() const;

		void SetPich(float p_pitch);

		void SetGain(float p_gain);

		void SetPosition(Math::Vector3 p_position);
		
		void SetVelocity(Math::Vector3 p_velocity);

		void SetLoopSound(bool p_isLooping);

		void SetSound(const char* p_soundName);
	};
}
#endif //ST_AUDIO_SOURCE_H