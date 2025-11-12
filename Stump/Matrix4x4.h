#ifndef ST_MATRIX4X4_H
#define ST_MATRIX4X4_H

#include "Matrix3x4.h"
#include "Vector3.h"
#include "Plane.h"
#include <glm/glm.hpp>
#include <vector>
class Matrix4x4
{
public:

	enum Planes {
		PLANE_NEAR,
		PLANE_FAR,
		PLANE_LEFT,
		PLANE_TOP,
		PLANE_RIGHT,
		PLANE_BOTTOM
	};

	num_fd matrix[4][4];

	void SetIdentity();
	void SetZero();
	void SetLightBias();
	void SetPerspective(num_fd p_fovy_degrees, num_fd p_aspect, num_fd p_near, num_fd p_far, bool p_flip_fov = false);
	void SetForHMD(int p_eye, num_fd p_aspect, num_fd p_intraocular_dist, num_fd p_display_width, num_fd p_display_to_lens, num_fd p_oversample, num_fd p_near, num_fd p_far);
	void SetOrthogonal(num_fd p_left, num_fd p_right, num_fd p_bottom, num_fd p_top, num_fd p_near, num_fd p_far);
	void SetOrthogonal(num_fd p_size, num_fd p_aspect, num_fd p_near, num_fd p_far, bool p_flip_fov = false);
	void SetFrustum(num_fd p_left, num_fd p_right, num_fd p_bottom, num_fd p_top, num_fd p_near, num_fd p_far);

	static num_fd get_fovy(num_fd p_fovx, num_fd p_aspect) {
		return Math::ToDegrees(Math::Atan(p_aspect * Math::Tan(Math::ToRadians(p_fovx) * 0.5)) * 2.0);
	}

	num_fd GetZFar() const;
	num_fd GetZNear() const;
	num_fd GetAspect() const;
	num_fd GetFOV() const;
	glm::mat4 GetGlmMat4() const;
	bool IsOrthogonal() const;

	std::vector<Plane> GetProjectionPlanes(const Matrix3x4& p_transform) const;

	bool GetEndpoints(const Matrix3x4& p_transform, Vector3* p_8points) const;
	Vector3 GetViewportHalfExtents() const;

	void Invert();
	Matrix4x4 Inverse() const;

	Matrix4x4 operator*(const Matrix4x4& p_matrix) const;

	Vector3 XForm(const Vector3& p_vec3) const;

	void MakeScale(const Vector3& p_scale);

	operator Matrix3x4() const;

	Matrix4x4();
	Matrix4x4(const Matrix3x4& p_transform);
	Matrix4x4(glm::mat4 p_mat4);
	~Matrix4x4();
};

Vector3 Matrix4x4::XForm(const Vector3& p_vec3) const {
	Vector3 ret;
	ret.x = matrix[0][0] * p_vec3.x + matrix[1][0] * p_vec3.y + matrix[2][0] * p_vec3.z + matrix[3][0];
	ret.y = matrix[0][1] * p_vec3.x + matrix[1][1] * p_vec3.y + matrix[2][1] * p_vec3.z + matrix[3][1];
	ret.z = matrix[0][2] * p_vec3.x + matrix[1][2] * p_vec3.y + matrix[2][2] * p_vec3.z + matrix[3][2];
	num_fd w = matrix[0][3] * p_vec3.x + matrix[1][3] * p_vec3.y + matrix[2][3] * p_vec3.z + matrix[3][3];
	return ret / w;
}

#endif
