#ifndef ST_AUDIO_LISTENER_H
#define ST_AUDIO_LISTENER_H

#include "Math/Vector3.h"
using namespace Math;
namespace Audio {
	class AudioListener {

        Vector3 position{ 0,0,0 };
        Vector3 velocity{ 0,0,0 };
        Vector3 forward{ 0,0,-1 };
        Vector3 up{ 0,1,0 };
        float   gain = 1.0f;
    public:

        AudioListener();
        ~AudioListener();

        void SetPosition(const Vector3& r_pos);

        void SetVelocity(const Vector3& r_vel);

        void SetOrientation(const Vector3& r_forward, const Vector3& r_up);

        void SetGain(float p_gain);

        static AudioListener* Get();
	};
}
#endif //ST_AUDIO_LISTENER_H