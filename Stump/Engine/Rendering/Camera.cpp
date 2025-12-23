#include "Rendering/Camera.h"

namespace Rendering {
	Camera::Camera(int p_width, int p_height, Math::Vector3 p_position)
	{
		width = p_width;
		height = p_height;
		Position = p_position;
	}

	void Camera::UpdateMatrix(float p_FOVdeg, float p_nearPlane, float p_farPlane) {
		Matrix4x4 view;
		Matrix4x4 projection;

		view.SetLookAt(Position, Position + Orientation, Up);

		projection.SetPerspective(p_FOVdeg, (float)width / height, p_nearPlane, p_farPlane);

		cameraMatrix = projection * view;

	}

	void Camera::SetViewPortSize(int p_width, int p_height) {
		width = p_width;
		height = p_height;
	}
}