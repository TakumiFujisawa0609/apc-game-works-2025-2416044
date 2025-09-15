#pragma once

// -------------
// -- Vector2 --
// -------------
struct Vector2 {
	Vector2() : x(0), y(0) {}
	Vector2(float nx, float ny) : x(nx), y(ny) {}

	float x, y;

	const float Magnitude() const;

	void Normalize();
	Vector2 Normalized() const;

	void operator+=(const Vector2& v);
	void operator-=(const Vector2& v);
	void operator*=(float scale);
	Vector2 operator-() const;
	Vector2 operator*(float scale) const;
};

Vector2 operator+(const Vector2& va, const Vector2 vb);
Vector2 operator-(const Vector2& va, const Vector2 vb);

float Dot(const Vector2& va, const Vector2& vb);
float Cross(const Vector2& va, const Vector2& vb);

float operator*(const Vector2& va, const Vector2& vb);
float operator%(const Vector2& va, const Vector2& vb);
