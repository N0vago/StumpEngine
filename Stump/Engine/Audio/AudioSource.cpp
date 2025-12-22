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
        if (source == 0)
        {
            std::cerr << "[Audio] Failed to generate OpenAL source\n";
        }
        alSourcef(source, AL_PITCH, pitch);
        alSourcef(source, AL_GAIN, gain);
        alSource3f(source, AL_POSITION, (ALfloat)position.x, (ALfloat)position.y, (ALfloat)position.z);
        alSource3f(source, AL_VELOCITY, (ALfloat)velocity.x, (ALfloat)velocity.y, (ALfloat)velocity.z);
        alSourcei(source, AL_LOOPING, loopSound);
        alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);
        alSourcef(source, AL_REFERENCE_DISTANCE, 1.0f);
        alSourcef(source, AL_MAX_DISTANCE, 50.0f);
        alSourcef(source, AL_ROLLOFF_FACTOR, 1.0f);
    }

    AudioSource::~AudioSource()
    {
        alDeleteSources(1, &source);
    }

    void AudioSource::Play()
    {
        if (buffer > 0)
        {
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
    void AudioSource::SetSound(const char* p_fileName) {
        buffer = AudioBuffer::Get()->AddSoundEffect(p_fileName);
    }
}