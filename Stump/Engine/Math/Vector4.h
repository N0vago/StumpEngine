#ifndef ST_VECTOR4_H
#define ST_VECTOR4_H

#include "Math/Vector3.h"
namespace Math {
	struct Vector4
	{
		union {
			struct {
				Vector3 vec3;
				num_fd w;
			};
			num_fd coord[4];
		};

		Vector4();

		Vector4(num_fd p_x, num_fd p_y, num_fd p_z, num_fd p_w);

		Vector4(Vector3 p_vec3, num_fd p_w);

		Vector4(const Vector4& p_other);

		const num_fd& operator[](int index) const {
			return coord[index];
		}
		num_fd& operator[](int index) {
			return coord[index];
		}

	};
}
#endif 

