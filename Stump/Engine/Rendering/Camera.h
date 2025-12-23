#ifndef ST_CAMERA_CLASS_H
#define ST_CAMERA_CLASS_H

#include "Math/Matrix4x4.h"
#include "Rendering/Shader.h"
using namespace Math;
namespace Rendering {
	class Camera
	{
		int width;
		int height;
	public:

		Math::Vector3 Position;
		Math::Vector3 Orientation = Math::Vector3(0.0f, 0.0f, -1.0f);
		Math::Vector3 Up = Math::Vector3(0.0f, 1.0f, 0.0f);
		Matrix4x4 cameraMatrix;

		bool firstClick = true;

		float speed = 5.0f;
		float sensitivity = 10.0f;


		Camera(int p_width, int p_height, Math::Vector3 p_position);

		void UpdateMatrix(float p_FOVdeg, float p_nearPlane, float p_farPlane);

		void SetViewPortSize(int p_width, int p_height);
	};
}
#endif;

