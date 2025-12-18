#ifndef ST_AUDIO_BUFFER_H
#define ST_AUDIO_BUFFER_H
#include <AL\al.h>
#include <unordered_map>
namespace Audio {
	using SoundID = uint32_t;

	class AudioBuffer {
		std::unordered_map<SoundID, uint32_t> soundBuffers;

		AudioBuffer();
		~AudioBuffer();
	public:
		static AudioBuffer* Get();

		SoundID AddSoundEffect(const char* p_filename);
		bool RemoveSoundEffect(const SoundID& r_buffer);

		uint32_t GetSoundBuffer(const SoundID& r_soundID);
	};
}
#endif //ST_AUDIO_BUFFER_H