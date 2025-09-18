#include <cmath>
#include "Geometry.h"

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

Vector2 Vector2::operator*(float scale) const {
	return Vector2(x * scale, y * scale);
}

Vector2 Vector2::operator-() const {
	return Vector2(-x, -y);
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

float Vector3::Magnitude() const {
	return sqrt(SQMagnitude());
}

float Vector3::SQMagnitude() const {
	return x * x + y * y + z * z;
}

void Vector3::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
}

Vector3 Vector3::Normalized() const {
	float mag = Magnitude();
	return Vector3(x / mag, y / mag, z / mag);
}

void Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}

void Vector3::operator-=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

void Vector3::operator*=(float scale) {
	x *= scale;
	y *= scale;
	z *= scale;
}

Vector3 Vector3::operator*(float scale) const {
	return Vector3(x * scale, y * scale, z * scale);
}

Vector3 Vector3::operator-() const {
	return Vector3(-x, -y, -z);
}

float Dot(const Vector3& va, const Vector3& vb) {
	return va.x * vb.x + va.y * vb.y + va.z * vb.z;
}

Vector3 Cross(const Vector3& va, const Vector3& vb) {
	return Vector3(va.z * vb.y - va.y * vb.z, va.z * vb.x - va.x * vb.z, va.x * vb.y - vb.x * va.y);
}

float operator*(const Vector3& va, const Vector3& vb) {
	return Dot(va, vb);
}

Vector3 operator%(const Vector3& va, const Vector3& vb) {
	return Cross(va, vb);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m[i][k] * rhs.m[k][j];
			}
		}
	}
	return result;
}

Vector3 Matrix4x4::operator*(const Vector3& v) const {
	Vector3 result = {};
	result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
	result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
	result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
	return result;
}

Matrix4x4 IdentityMatrix() {
	return {
		1.F, 0.F, 0.F, 0.F,
		0.F, 1.F, 0.F, 0.F,
		0.F, 0.F, 1.F, 0.F,
		0.F, 0.F, 0.F, 1.F
	};
}

Matrix4x4 TransposeMatrix(const Matrix4x4& m) {
	return {
		m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
		m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
		m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
		m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]
	};
}

Matrix4x4 RotationMatrixX(float angle) {
	return {
		1.F, 0.F, 0.F, 0.F,
		0.F, cosf(angle), -sinf(angle), 0.F,
		0.F, sinf(angle), cosf(angle), 0.F,
		0.F, 0.F, 0.F, 1.F
	};
}

Matrix4x4 RotationMatrixY(float angle) {
	return {
		cosf(angle), 0.F, sinf(angle), 0.F,
		0.F, 1.F, 0.F, 0.F,
		-sinf(angle), 0.F, cosf(angle), 0.F,
		0.F, 0.F, 0.F, 1.F
	};
}

Matrix4x4 RotationMatrixZ(float angle) {
	return {
		cosf(angle), -sinf(angle), 0.F, 0.F,
		sinf(angle), cosf(angle), 0.F, 0.F,
		0.F, 0.F, 1.F, 0.F,
		0.F, 0.F, 0.F, 1.F
	};
}

Matrix4x4 TranslationMatrix(const Vector3& v) {
	Matrix4x4 result = IdentityMatrix();
	result.m[0][3] = v.x;
	result.m[1][3] = v.y;
	result.m[2][3] = v.z;
	return result;
}

Matrix4x4 TranslationMatrix(float x, float y, float z) {
	Matrix4x4 result = IdentityMatrix();
	result.m[0][3] = x;
	result.m[1][3] = y;
	result.m[2][3] = z;
	return result;
}
