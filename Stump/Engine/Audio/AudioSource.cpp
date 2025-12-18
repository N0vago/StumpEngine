#include "AudioSource.h"
#include <iostream>
#include "Audio/AudioDevice.h"
namespace Audio {
    AudioSource::AudioSource()
        : pitch(1.0f)
        , gain(1.0f)
        , position(Math::Vector3(0.0f, 0.0f, 0.0f))
        , velocity(Math::Vector3(0.0f, 0.0f, 0.0f))
        , loopSound(AL_FALSE)
        , buffer(0)
    {

        alGenSources(1, &source);

        alSourcef(source, AL_PITCH, pitch);
        alSourcef(source, AL_GAIN, gain);
        alSource3f(source, AL_POSITION, (ALfloat)position.x, (ALfloat)position.y, (ALfloat)position.z);
        alSource3f(source, AL_VELOCITY, (ALfloat)position.x, (ALfloat)position.y, (ALfloat)position.z);
        alSourcei(source, AL_LOOPING, loopSound);
    }

    AudioSource::~AudioSource()
    {
        alDeleteSources(1, &source);
    }

    void AudioSource::Play(const SoundID p_sound)
    {
        if (!AudioDevice::Get())
            return;
        ALuint buffer = AudioBuffer::Get()->GetSoundBuffer(p_sound);
        if (!buffer)
            return;

        if (buffer != buffer)
        {
            buffer = buffer;
            alSourcei(source, AL_BUFFER, buffer);
        }

        alSourcePlay(source);
    }

    void AudioSource::Pause()
    {
        alSourcePause(source);
    }

    void AudioSource::Stop()
    {
        alSourceStop(source);
    }

    bool AudioSource::IsPlaying() const
    {
        ALint state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }
    void AudioSource::SetPich(float p_pitch)
    {
        pitch = p_pitch;
        alSourcef(source, AL_PITCH, (ALfloat)pitch);
    }
    void AudioSource::SetGain(float p_gain)
    {
        gain = p_gain;
        alSourcef(source, AL_GAIN, (ALfloat)gain);
    }
    void AudioSource::SetPosition(Math::Vector3 p_position)
    {
        position = p_position;
        alSource3f(source, AL_POSITION, position.x, position.y, position.z);
    }
    void AudioSource::SetVelocity(Math::Vector3 p_velocity)
    {
        velocity = p_velocity;

        alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }
    void AudioSource::SetLoopSound(bool p_isLooping)
    {
        loopSound = p_isLooping;
        alSourcei(source, AL_LOOPING, loopSound);
    }
}