#ifndef ST_MATRIX4X4_H
#define ST_MATRIX4X4_H

#include "Matrix3x4.h"
#include "Vector3.h"
#include "Plane.h"
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

	Matrix4x4();
	Matrix4x4(const Matrix3x4& p_transform);
	Matrix4x4(num_fd xx, num_fd xy, num_fd xz, num_fd xw,
			  num_fd yx, num_fd yy, num_fd yz, num_fd yw,
			  num_fd zx, num_fd zy, num_fd zz, num_fd zw,
				num_fd wx, num_fd wy, num_fd wz, num_fd ww);
	Matrix4x4(num_fd elements[4][4]);
	~Matrix4x4();

	void Set(num_fd xx, num_fd xy, num_fd xz, num_fd xw,
				num_fd yx, num_fd yy, num_fd yz, num_fd yw,
				num_fd zx, num_fd zy, num_fd zz, num_fd zw,
		num_fd wx, num_fd wy, num_fd wz, num_fd ww);
	void Set(const num_fd elements[4][4]);

	void SetIdentity();
	void SetZero();
	void SetLightBias();
	void SetLookAt(const Vector3& p_eye, const Vector3& p_target, const Vector3& p_up);
	void SetPerspective(num_fd p_fovy_degrees, num_fd p_aspect, num_fd p_near, num_fd p_far, bool p_flip_fov = false);
	void SetForHMD(int p_eye, num_fd p_aspect, num_fd p_intraocular_dist, num_fd p_display_width, num_fd p_display_to_lens, num_fd p_oversample, num_fd p_near, num_fd p_far);
	void SetOrthogonal(num_fd p_left, num_fd p_right, num_fd p_bottom, num_fd p_top, num_fd p_near, num_fd p_far);
	void SetOrthogonal(num_fd p_size, num_fd p_aspect, num_fd p_near, num_fd p_far, bool p_flip_fov = false);
	void SetFrustum(num_fd p_left, num_fd p_right, num_fd p_bottom, num_fd p_top, num_fd p_near, num_fd p_far);

	static num_fd GetFOVy(num_fd p_fovx, num_fd p_aspect) {
		return Math::ToDegrees(Math::Atan(p_aspect * Math::Tan(Math::ToRadians(p_fovx) * 0.5)) * 2.0);
	}
	std::array<float, 16> ToArray() const
	{
		std::array<float, 16> out{};
		for (int row = 0; row < 4; ++row)
			for (int col = 0; col < 4; ++col)
				out[row * 4 + col] = matrix[row][col];

		return out;
	}

	num_fd GetZFar() const;
	num_fd GetZNear() const;
	num_fd GetAspect() const;
	num_fd GetFOV() const;
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

};

#endif
