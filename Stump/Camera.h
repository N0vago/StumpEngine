#ifndef ST_CAMERA_CLASS_H
#define ST_CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "Matrix4x4.h"

#include"Shader.h"

class Camera
{
public:

	Vector3 Position;
	Vector3 Orientation = Vector3(0.0f, 0.0f, -1.0f);
	Vector3 Up = Vector3(0.0f, 1.0f, 0.0f);
	Matrix4x4 cameraMatrix;

	bool firstClick = true;

	int width;
	int height;

	float speed = 5.0f;
	float sensitivity = 100.0f;


	Camera(int width, int height, Vector3 position);

	void UpdateMatrix(float FOVdeg, float nearPlane, float farPlane);

	void Inputs(GLFWwindow* window, float deltaTime);
};
#endif;

