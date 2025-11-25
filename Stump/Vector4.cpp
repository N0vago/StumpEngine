#include "Vector4.h"


Vector4::Vector4()
{
	vec3 = Vector3();
	w = 0.0f;
}
Vector4::Vector4(num_fd p_x, num_fd p_y, num_fd p_z, num_fd p_w)
{
	vec3 = Vector3(p_x, p_y, p_z);
	w = p_w;
}
Vector4::Vector4(Vector3 p_vec3, num_fd p_w)
{
	vec3 = p_vec3;
	w = p_w;
}
Vector4::Vector4(const Vector4& p_other)
{
	vec3 = p_other.vec3;
	w = p_other.w;
}