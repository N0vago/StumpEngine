#ifndef ST_AUDIO_BUFFER_H
#define ST_AUDIO_BUFFER_H
#include <AL\al.h>
#include <vector>
namespace Audio {
	class AudioBuffer {
		std::vector<uint32_t> soundBuffers;

	public:
		AudioBuffer();
		~AudioBuffer();

		uint32_t AddSoundEffect(const char* p_filename);
		bool RemoveSoundEffect(const uint32_t& r_buffer);

		static AudioBuffer* Get();

	};
}
#endif //ST_AUDIO_BUFFER_H