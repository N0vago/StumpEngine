#ifndef ST_AUDIO_DEVICE_H
#define ST_AUDIO_DEVICE_H
#include <AL/alc.h>
namespace Audio {
	class AudioDevice {
		ALCdevice* device;
		ALCcontext* context;
	public:

		AudioDevice();
		~AudioDevice();

		static AudioDevice* Get();
	};

}
#endif //ST_SOUND_DEVICE