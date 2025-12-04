#include "Camera.h"


Camera::Camera(int width, int height, Vector3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::UpdateMatrix(float FOVdeg, float nearPlane, float farPlane) {
	Matrix4x4 view;
	Matrix4x4 projection;

	view.SetLookAt(Position, Position + Orientation, Up);

	projection.SetPerspective(FOVdeg, (float)width / height, nearPlane, farPlane);

	cameraMatrix = projection * view;
	
}