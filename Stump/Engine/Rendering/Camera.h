#ifndef ST_CAMERA_CLASS_H
#define ST_CAMERA_CLASS_H

#include "Math/Matrix4x4.h"
#include "Rendering/Shader.h"
using namespace Math;
namespace Rendering {
	class Camera
	{
	public:

		Math::Vector3 Position;
		Math::Vector3 Orientation = Math::Vector3(0.0f, 0.0f, -1.0f);
		Math::Vector3 Up = Math::Vector3(0.0f, 1.0f, 0.0f);
		Matrix4x4 cameraMatrix;

		bool firstClick = true;

		int width;
		int height;

		float speed = 5.0f;
		float sensitivity = 1000.0f;


		Camera(int width, int height, Math::Vector3 position);

		void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane);

	};
}
#endif;

