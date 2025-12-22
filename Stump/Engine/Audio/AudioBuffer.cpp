#include "Audio/AudioBuffer.h"
#include <sndfile.h>
#include <inttypes.h>
#include <AL\alext.h>
#include <iostream>
#include <assert.h>
namespace Audio {

	static AudioBuffer* Instance = nullptr;
	AudioBuffer::AudioBuffer() {
		Instance = this;
		soundBuffers.clear();
	}
	AudioBuffer::~AudioBuffer()
	{
		Instance = nullptr;
		for (auto& buffer : soundBuffers)
		{
			alDeleteBuffers(1, &buffer);
		}

		soundBuffers.clear();
	}
	uint32_t AudioBuffer::AddSoundEffect(const char* p_filename)
	{
		ALenum err, format;
		ALuint buffer;
		SNDFILE* sndfile;
		SF_INFO sfinfo;
		short* membuf;
		sf_count_t num_frames;
		ALsizei num_bytes;

		sndfile = sf_open(p_filename, SFM_READ, &sfinfo);
		if (!sndfile)
		{
			std::cerr << (stderr, "Could not open audio in %s: %s\n", p_filename, sf_strerror(sndfile)) << std::endl;
			return 0;
		}
		if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
		{
			std::cerr << (stderr, "Bad sample count in %s (%" PRId64 ")\n", p_filename, sfinfo.frames) << std::endl;
			sf_close(sndfile);
			return 0;
		}

		format = AL_NONE;
		if (sfinfo.channels == 1)
			format = AL_FORMAT_MONO16;
		else if (sfinfo.channels == 2)
			format = AL_FORMAT_STEREO16;
		else if (sfinfo.channels == 3)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (sfinfo.channels == 4)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT3D_16;
		}
		if (!format)
		{
			std::cerr << (stderr, "Unsupported channel count: %d\n", sfinfo.channels) << std::endl;
			sf_close(sndfile);
			return 0;
		}

		membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

		num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
		if (num_frames < 1)
		{
			free(membuf);
			sf_close(sndfile);
			std::cerr << (stderr, "Failed to read samples in %s (%" PRId64 ")\n", p_filename, num_frames) << std::endl;
			return 0;
		}
		num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

		buffer = 0;
		alGenBuffers(1, &buffer);

		alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);
		

		free(membuf);
		sf_close(sndfile);

		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			std::cerr << (stderr, "OpenAL Error: %s\n", alGetString(err)) << std::endl;
			if (buffer && alIsBuffer(buffer))
				alDeleteBuffers(1, &buffer);
			return 0;
		}

		
		soundBuffers.push_back(buffer);

		return buffer;
	}
	bool AudioBuffer::RemoveSoundEffect(const uint32_t& r_buffer)
	{
		auto it = soundBuffers.begin();
		while (it != soundBuffers.end())
		{
			if (*it == r_buffer)
			{
				alDeleteBuffers(1, &*it);

				it = soundBuffers.erase(it);

				return true;
			}
			else {
				++it;
			}
		}
		return false;  // couldn't find to remove
	}
	AudioBuffer* AudioBuffer::Get()
	{
		assert(Instance && "AudioBuffer instance does not exist!");
		return Instance;
	}
}