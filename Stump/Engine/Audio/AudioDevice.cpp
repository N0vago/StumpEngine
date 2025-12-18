#include "Audio/AudioDevice.h"
#include <AL\al.h>
#include <iostream>
#include <assert.h>
namespace Audio {

	static AudioDevice* Instance = nullptr;
	AudioDevice::AudioDevice() {
		Instance = this;
		device = alcOpenDevice(nullptr); // nullptr = get default device
		if (!device)
			throw("failed to get sound device");

		context = alcCreateContext(device, nullptr);  // create context
		if (!context)
			throw("Failed to set sound context");

		if (!alcMakeContextCurrent(context))   // make context current
			throw("failed to make context current");

		const ALCchar* name = nullptr;
		if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
			name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
		if (!name || alcGetError(device) != AL_NO_ERROR)
			name = alcGetString(device, ALC_DEVICE_SPECIFIER);
		printf("Opened \"%s\"\n", name);
	}
	AudioDevice::~AudioDevice()
	{
		if (!alcMakeContextCurrent(nullptr))
			throw("failed to set context to nullptr");

		alcDestroyContext(context);
		if (context)
			throw("failed to unset during close");

		if (!alcCloseDevice(device))
			throw("failed to close sound device");
	}
	AudioDevice* AudioDevice::Get()
	{
		assert(Instance && "AudioDevice instance does not exist!");
		return Instance;
	}
}