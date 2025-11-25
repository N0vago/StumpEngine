#include "Vector2.h"
#include "Math.h"

Vector2::Vector2() : x(0.0f), y(0.0f) {}

Vector2::Vector2(num_fd p_x, num_fd p_y) : x(p_x), y(p_y) {}

Vector2::Vector2(const Vector2& p_other) : x(p_other.x), y(p_other.y) {}

Vector2 Vector2::LimitLength(const num_fd p_len) const
{
	num_fd len = Length();
	if (len > p_len && len > 0.0f) {
		num_fd scale = p_len / len;
		return Vector2(x * scale, y * scale);
	}
	return *this;
}

void Vector2::Snap(Vector2 p_val)
{
	x = Math::Stepify(x, p_val.x);
	y = Math::Stepify(y, p_val.y);
}

Vector2 Vector2::Snapped(Vector2 p_val) const
{
	Vector2 v = *this;
	v.Snap(p_val);
	return v;
}

bool Vector2::IsEqualApprox(const Vector2& p_v) const
{
	return IsEqualApprox(p_v, Math::EPSILON);
}

bool Vector2::IsEqualApprox(const Vector2& p_v, num_fd p_tolerance) const
{
	return Math::IsEqualApprox(x, p_v.x, p_tolerance) &&
		Math::IsEqualApprox(y, p_v.y, p_tolerance);
}

num_fd Vector2::Length() const
{
	return Math::Sqrtf(x * x + y * y);
}

num_fd Vector2::LengthSquared() const
{
	return x * x + y * y;
}

void Vector2::Normalize()
{
	num_fd len = Length();
	if (len > 0.0f) {
		x /= len;
		y /= len;
	}
}

Vector2 Vector2::Normalized() const
{
	num_fd len = Length();
	if (len > 0.0f) {
		return Vector2(x / len, y / len);
	}
	return Vector2(0.0f, 0.0f);
}
bool Vector2::IsNormalized() const
{
	return Math::Absf(Length() - 1.0f) < 1e-6f;
}

void Vector2::Zero()
{
	x = y = 0.0f;
}
Vector2 Vector2::LinearInterpolate(const Vector2& p_to, num_fd p_weight) const {
	Vector2 res = *this;

	res.x += (p_weight * (p_to.x - x));
	res.y += (p_weight * (p_to.y - y));

	return res;
}

Vector2 Vector2::LinearInterpolate(const Vector2& p_a, const Vector2& p_b, num_fd p_weight) {
	Vector2 res = p_a;

	res.x += (p_weight * (p_b.x - p_a.x));
	res.y += (p_weight * (p_b.y - p_a.y));

	return res;
}

Vector2 Vector2::Slerp(const Vector2& p_to, num_fd p_weight) const
{
	num_fd theta = AngleTo(p_to);
	return Rotated(theta * p_weight);
}

num_fd Vector2::Cross(const Vector2& p_b) const
{
	return (y * p_b.x - x * p_b.y);
}

num_fd Vector2::Dot(const Vector2& p_b) const
{
	return x * p_b.x + y * p_b.y;
}

Vector2 Vector2::Abs() const
{
	return Vector2(Math::Absf(x), Math::Absf(y));
}

Vector2 Vector2::Floor() const
{
	return Vector2(Math::Floorf(x), Math::Floorf(y));
}

Vector2 Vector2::Sign() const
{
	return Vector2(
		(x > 0.0f) ? 1.0f : ((x < 0.0f) ? -1.0f : 0.0f),
		(y > 0.0f) ? 1.0f : ((y < 0.0f) ? -1.0f : 0.0f)
	);
}

Vector2 Vector2::Ceil() const
{
	return Vector2(Math::Ceilf(x), Math::Ceilf(y));
}

Vector2 Vector2::Round() const
{
	return Vector2(Math::Roundf(x), Math::Roundf(y));
}

num_fd Vector2::DistanceTo(const Vector2& p_to) const
{
	return (*this - p_to).Length();
}

num_fd Vector2::DistanceSquaredTo(const Vector2& p_to) const
{
	return (*this - p_to).LengthSquared();
}

Vector2 Vector2::Posmod(const num_fd p_mod) const
{
	return Vector2(
		Math::Fposmod(x, p_mod),
		Math::Fposmod(y, p_mod)
	);
}

Vector2 Vector2::Posmodv(const Vector2& p_modv) const
{
	return Vector2(
		Math::Fposmod(x, p_modv.x),
		Math::Fposmod(y, p_modv.y)
	);
}

Vector2 Vector2::Project(const Vector2& p_to) const
{
	return p_to * (Dot(p_to) / p_to.LengthSquared());
}

num_fd Vector2::Angle() const
{
	return Math::Atan2f(y, x);
}

num_fd Vector2::AngleTo(const Vector2& p_to) const
{
	return Math::Atan2f(Cross(p_to), Dot(p_to));
}

num_fd Vector2::AngleToPoint(const Vector2& p_to) const
{
	return Math::Atan2f(y - p_to.y, x - p_to.x);
}

void Vector2::SetRotation(num_fd p_radians)
{
	x = Math::Cosf(p_radians);
	y = Math::Sinf(p_radians);
}

void Vector2::Rotate(num_fd p_radians)
{
	num_fd cos_theta = Math::Cosf(p_radians);
	num_fd sin_theta = Math::Sinf(p_radians);
	num_fd new_x = x * cos_theta - y * sin_theta;
	num_fd new_y = x * sin_theta + y * cos_theta;
	x = new_x;
	y = new_y;
}

Vector2 Vector2::Rotated(num_fd p_radians) const
{
	Vector2 v;
	v.SetRotation(Angle() + p_radians);
	v *= Length();
	return v;
}

Vector2 Vector2::DirectionTo(const Vector2& p_to) const
{
	return (p_to - (*this)).Normalized();
}

Vector2 Vector2::Slide(const Vector2& p_normal) const
{
	return (*this) - p_normal * this->Dot(p_normal);
}

Vector2 Vector2::Bounce(const Vector2& p_normal) const
{
	return -Reflect(p_normal);
}

Vector2 Vector2::Reflect(const Vector2& p_normal) const
{
	return  p_normal * 2.0f * this->Dot(p_normal) - (*this);
}

Vector2& Vector2::operator+=(const Vector2& p_v)
{
	x += p_v.x;
	y += p_v.y;
	return *this;
}

Vector2 Vector2::operator+(const Vector2& p_v) const
{
	return Vector2(x + p_v.x, y + p_v.y);
}

Vector2& Vector2::operator-=(const Vector2& p_v)
{
	x -= p_v.x;
	y -= p_v.y;
	return *this;
}

Vector2 Vector2::operator-(const Vector2& p_v) const
{
	return Vector2(x - p_v.x, y - p_v.y);
}

Vector2& Vector2::operator*=(const Vector2& p_v)
{
	x *= p_v.x;
	y *= p_v.y;
	return *this;
}
Vector2& Vector2::operator*=(num_fd p_scalar) {
	x *= p_scalar;
	y *= p_scalar;
	return *this;
}

Vector2 Vector2::operator*(const Vector2& p_v) const
{
	return Vector2(x * p_v.x, y * p_v.y);
}

Vector2 operator*(num_fd p_scalar, const Vector2& p_vec) {
	return p_scalar * p_vec;
}

Vector2 Vector2::operator*(num_fd p_scalar) const {
	return Vector2(x * p_scalar, y * p_scalar);
}
Vector2& Vector2::operator/=(const Vector2& p_v)
{
	x /= p_v.x;
	y /= p_v.y;
	return *this;
}

Vector2 Vector2::operator/(const Vector2& p_v) const
{
	return Vector2(x / p_v.x, y / p_v.y);
}

Vector2& Vector2::operator/=(num_fd p_scalar)
{
	x /= p_scalar;
	y /= p_scalar;
	return *this;
}

Vector2 Vector2::operator/(num_fd p_scalar) const
{
	return Vector2(x / p_scalar, y / p_scalar);
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

bool Vector2::operator==(const Vector2& p_v) const
{
	return x == p_v.x && y == p_v.y;
}

bool Vector2::operator!=(const Vector2& p_v) const
{
	return x != p_v.x || y != p_v.y;
}

bool Vector2::operator<(const Vector2& p_v) const
{
	if (x == p_v.x) {
		if (y == p_v.y) {
			return false;
		}
		else {
			return y > p_v.y;
		}
	}
	else {
		return x > p_v.x;
	}
}

bool Vector2::operator<=(const Vector2& p_v) const
{
	if (x == p_v.x) {
		if (y == p_v.y) {
			return false;
		}
		else {
			return y < p_v.y;
		}
	}
	else {
		return x < p_v.x;
	}
}
bool Vector2::operator>(const Vector2& p_v) const
{
	if (x == p_v.x) {
		if (y == p_v.y) {
			return false;
		}
		else {
			return y > p_v.y;
		}
	}
	else {
		return x > p_v.x;
	}
}

bool Vector2::operator>=(const Vector2& p_v) const
{
	if (x == p_v.x) {
		if (y == p_v.y) {
			return false;
		}
		else {
			return y > p_v.y;
		}
	}
	else {
		return x > p_v.x;
	}
}