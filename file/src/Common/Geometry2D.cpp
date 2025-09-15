#include <cmath>
#include "Geometry2D.h"

// -------------
// -- Vector2 --
// -------------
const float Vector2::Magnitude() const {
	return hypot(x, y);
}

void Vector2::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}

Vector2 Vector2::Normalized() const {
	float mag = Magnitude();
	return Vector2(x / mag, y / mag);
}

void Vector2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
}

void Vector2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
}

void Vector2::operator*=(float scale) {
	x *= scale;
	y *= scale;
}

Vector2 Vector2::operator-() const {
	return Vector2(-x, -y);
}

Vector2 Vector2::operator*(float scale) const {
	return Vector2(x * scale, y * scale);
}

Vector2 operator+(const Vector2& va, const Vector2 vb) {
	return Vector2(va.x + vb.x, va.y + vb.y);
}

Vector2 operator-(const Vector2& va, const Vector2 vb) {
	return Vector2(va.x - vb.x, va.y - vb.y);
}

float Dot(const Vector2& va, const Vector2& vb) {
	return va.x * vb.x + va.y * vb.y;
}

float Cross(const Vector2& va, const Vector2& vb) {
	return va.x * vb.y - vb.x * va.y;
}

float operator*(const Vector2& va, const Vector2& vb) {
	return Dot(va, vb);
}

float operator%(const Vector2& va, const Vector2& vb) {
	return Cross(va, vb);
}
