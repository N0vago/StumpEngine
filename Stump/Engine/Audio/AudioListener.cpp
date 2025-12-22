#include "Audio/AudioListener.h"
#include <assert.h>
#include <AL/al.h>
namespace Audio {

	static AudioListener* Instance = nullptr;

	AudioListener::AudioListener()
	{
		Instance = this;
	}
	AudioListener::~AudioListener()
	{
		Instance = nullptr;
	}
	void AudioListener::SetPosition(const Vector3& r_pos)
	{
		position = r_pos;
		alListener3f(AL_POSITION, r_pos.x, r_pos.y, r_pos.z);
	}
	void AudioListener::SetVelocity(const Vector3& r_vel)
	{
		velocity = r_vel;
		alListener3f(AL_VELOCITY, r_vel.x, r_vel.y, r_vel.z);
	}
	void AudioListener::SetOrientation(const Vector3& r_forward, const Vector3& r_up)
	{
		forward = r_forward;
		up = r_up;

		const float ori[6] = {
			r_forward.x, r_forward.y, r_forward.z,
			r_up.x,      r_up.y,      r_up.z
		};
		alListenerfv(AL_ORIENTATION, ori);
	}
	void AudioListener::SetGain(float p_gain)
	{
		gain = p_gain;
		alListenerf(AL_GAIN, p_gain);
	}
	AudioListener* AudioListener::Get()
	{
		assert(Instance && "Audio Listener instance does not exist!");
		return Instance;
	}
}