#include "Vector3.h"


Vector3 Vector3::LimitLength(const num_fd p_len) const
{
	num_fd len = Length();
	if (len > p_len && len > 0.0f) {
		num_fd scale = p_len / len;
		return Vector3(x * scale, y * scale, z * scale);
	}
	return *this;
}

void Vector3::Snap(Vector3 p_val)
{
	x = Math::Stepify(x, p_val.x);
	y = Math::Stepify(y, p_val.y);
	z = Math::Stepify(z, p_val.z);
}

Vector3 Vector3::Snapped(Vector3 p_val) const
{
	Vector3 v = *this;
	v.Snap(p_val);
	return v;
}

bool Vector3::IsEqualApprox(const Vector3& p_v) const
{
	return IsEqualApprox(p_v, Math::EPSILON);
}

bool Vector3::IsEqualApprox(const Vector3& p_v, num_fd p_tolerance) const
{
	return Math::IsEqualApprox(x, p_v.x, p_tolerance) &&
		Math::IsEqualApprox(y, p_v.y, p_tolerance) &&
		Math::IsEqualApprox(z, p_v.z, p_tolerance);
}

num_fd Vector3::Length() const
{
	return Math::Sqrtf(x * x + y * y + z * z);
}

num_fd Vector3::LengthSquared() const
{
	return x * x + y * y + z * z;
}

void Vector3::Normalize()
{
	num_fd len = Length();
	if (len > 0.0f) {
		x /= len;
		y /= len;
		z /= len;
	}
}

Vector3 Vector3::Normalized() const
{
	num_fd len = Length();
	if (len > 0.0f) {
		return Vector3(x / len, y / len, z / len);
	}
	return Vector3(0.0f, 0.0f, 0.0f);
}
bool Vector3::IsNormalized() const
{
	return Math::Absf(Length() - 1.0f) < 1e-6f;
}
Vector3 Vector3::Inverse() const
{
	return Vector3(-x, -y, -z);
}

void Vector3::Zero()
{
	x = y = z = 0.0f;
}


Vector3 Vector3::Slerp(const Vector3& p_to, num_fd p_weight) const
{
	// Calculate the angle between the two vectors
	num_fd dot_product = Math::Clampf(this->Dot(p_to), -1.0f, 1.0f);
	num_fd theta = Math::Acosf(dot_product) * p_weight;
	// Calculate the relative vector
	Vector3 relative_vec = (p_to - (*this) * dot_product).Normalized();
	// Calculate the final vector
	return ((*this) * Math::Cosf(theta)) + (relative_vec * Math::Sinf(theta));
}
Vector3 Vector3::LinearInterpolate(const Vector3& p_to, num_fd p_weight) const
{
	return (*this) * (1.0f - p_weight) + p_to * p_weight;
}

Vector3 Vector3::Cross(const Vector3& p_b) const
{
	return Vector3(
		y * p_b.z - z * p_b.y,
		z * p_b.x - x * p_b.z,
		x * p_b.y - y * p_b.x
	);
}

num_fd Vector3::Dot(const Vector3& p_b) const
{
	return x * p_b.x + y * p_b.y + z * p_b.z;
}

Vector3 Vector3::Abs() const
{
	return Vector3(Math::Absf(x), Math::Absf(y), Math::Absf(z));
}

Vector3 Vector3::Floor() const
{
	return Vector3(Math::Floorf(x), Math::Floorf(y), Math::Floorf(z));
}

Vector3 Vector3::Sign() const
{
	return Vector3(
		(x > 0.0f) ? 1.0f : ((x < 0.0f) ? -1.0f : 0.0f),
		(y > 0.0f) ? 1.0f : ((y < 0.0f) ? -1.0f : 0.0f),
		(z > 0.0f) ? 1.0f : ((z < 0.0f) ? -1.0f : 0.0f)
	);
}

Vector3 Vector3::Ceil() const
{
	return Vector3(Math::Ceilf(x), Math::Ceilf(y), Math::Ceilf(z));
}

Vector3 Vector3::Round() const
{
	return Vector3(Math::Roundf(x), Math::Roundf(y), Math::Roundf(z));
}

num_fd Vector3::DistanceTo(const Vector3& p_to) const
{
	return (*this - p_to).Length();
}

num_fd Vector3::DistanceSquaredTo(const Vector3& p_to) const
{
	return (*this - p_to).LengthSquared();
}

Vector3 Vector3::Posmod(const num_fd p_mod) const
{
	return Vector3(
		Math::Fposmod(x, p_mod),
		Math::Fposmod(y, p_mod),
		Math::Fposmod(z, p_mod)
	);
}

Vector3 Vector3::Posmodv(const Vector3& p_modv) const
{
	return Vector3(
		Math::Fposmod(x, p_modv.x),
		Math::Fposmod(y, p_modv.y),
		Math::Fposmod(z, p_modv.z)
	);
}

Vector3 Vector3::Project(const Vector3& p_to) const
{
	return p_to * (Dot(p_to) / p_to.LengthSquared());
}

num_fd Vector3::AngleTo(const Vector3& p_to) const
{
	num_fd dot_product = Math::Clampf(this->Dot(p_to) / (this->Length() * p_to.Length()), -1.0f, 1.0f);
	return Math::Acosf(dot_product);
}

num_fd Vector3::SignedAngleTo(const Vector3& p_to, const Vector3& p_axis) const
{
	Vector3 cross_to = Cross(p_to);
	num_fd unsigned_angle = Math::Atan2(cross_to.Length(), Dot(p_to));
	num_fd sign = cross_to.Dot(p_axis);
	return (sign < 0) ? -unsigned_angle : unsigned_angle;
}

Vector3 Vector3::DirectionTo(const Vector3& p_to) const
{
	return (p_to - (*this)).Normalized();
}

Vector3 Vector3::Slide(const Vector3& p_normal) const
{
	return (*this) - p_normal * this->Dot(p_normal);
}

Vector3 Vector3::Bounce(const Vector3& p_normal) const
{
	return -Reflect(p_normal);
}

Vector3 Vector3::Reflect(const Vector3& p_normal) const
{
	return  p_normal * 2.0f * this->Dot(p_normal) - (*this);
}


Vector3& Vector3::operator+=(const Vector3& p_v)
{
	x += p_v.x;
	y += p_v.y;
	z += p_v.z;
	return *this;
}

Vector3 Vector3::operator+(const Vector3& p_v) const
{
	return Vector3(x + p_v.x, y + p_v.y, z + p_v.z);
}

Vector3& Vector3::operator-=(const Vector3& p_v)
{
	x -= p_v.x;
	y -= p_v.y;
	z -= p_v.z;
	return *this;
}

Vector3 Vector3::operator-(const Vector3& p_v) const
{
	return Vector3(x - p_v.x, y - p_v.y, z - p_v.z);
}

Vector3& Vector3::operator*=(const Vector3& p_v)
{
	x *= p_v.x;
	y *= p_v.y;
	z *= p_v.z;
	return *this;
}
Vector3& Vector3::operator*=(num_fd p_scalar) {
	x *= p_scalar;
	y *= p_scalar;
	z *= p_scalar;
	return *this;
}

Vector3 Vector3::operator*(const Vector3& p_v) const
{
	return Vector3(x * p_v.x, y * p_v.y, z * p_v.z);
}

Vector3 operator*(num_fd p_scalar, const Vector3& p_vec) {
	return p_vec * p_scalar;
}

Vector3 Vector3::operator*(num_fd p_scalar) const {
	return Vector3(x * p_scalar, y * p_scalar, z * p_scalar);
}
Vector3& Vector3::operator/=(const Vector3& p_v)
{
	x /= p_v.x;
	y /= p_v.y;
	z /= p_v.z;
	return *this;
}

Vector3 Vector3::operator/(const Vector3& p_v) const
{
	return Vector3(x / p_v.x, y / p_v.y, z / p_v.z);
}

Vector3& Vector3::operator/=(num_fd p_scalar)
{
	x /= p_scalar;
	y /= p_scalar;
	z /= p_scalar;
	return *this;
}

Vector3 Vector3::operator/(num_fd p_scalar) const
{
	return Vector3(x / p_scalar, y / p_scalar, z / p_scalar);
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

bool Vector3::operator==(const Vector3& p_v) const
{
	return x == p_v.x && y == p_v.y && z == p_v.z;
}

bool Vector3::operator!=(const Vector3& p_v) const
{
	return x != p_v.x || y != p_v.y || z != p_v.z;
}

bool Vector3::operator<(const Vector3& p_v) const
{
	if (x == p_v.x) {
		if (y == p_v.y) {
			return z > p_v.z;
		}
		else {
			return y > p_v.y;
		}
	}
	else {
		return x > p_v.x;
	}
}

bool Vector3::operator<=(const Vector3& p_v) const
{
	if (x == p_v.x) {
		if (y == p_v.y) {
			return z <= p_v.z;
		}
		else {
			return y < p_v.y;
		}
	}
	else {
		return x < p_v.x;
	}
}
bool Vector3::operator>(const Vector3& p_v) const
{
	if (x == p_v.x) {
		if (y == p_v.y) {
			return z > p_v.z;
		}
		else {
			return y > p_v.y;
		}
	}
	else {
		return x > p_v.x;
	}
}

bool Vector3::operator>=(const Vector3& p_v) const
{
	if (x == p_v.x) {
		if (y == p_v.y) {
			return z >= p_v.z;
		}
		else {
			return y > p_v.y;
		}
	}
	else {
		return x > p_v.x;
	}
}

