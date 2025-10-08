#include <cmath>
#include "Geometry.h"

#if _HAS_CXX20
#include <numbers>
#else
#include <math.h>
#define _USE_MATH_DEFINES
#endif

int Round(float n) {
	return n - (int)n >= 0.5f ? (int)n + 1 : (int)n;
}

int Round(double n) {
	return n - (int)n >= 0.5 ? (int)n + 1 : (int)n;
}

int RoundDown(float n) {
	return (int)n;
}

int RoundDown(double n) {
	return (int)n;
}

int RoundUp(float n) {
	return n - (int)n > 0.f ? (int)n + 1 : (int)n;
}

int RoundUp(double n) {
	return n - (int)n > 0 ? (int)n + 1 : (int)n;
}

float DegToRad(float n) {
#if _HAS_CXX20
	return n * (float)std::numbers::pi / 180.f;
#else
	return n * (float)M_PI / 180.f;
#endif
}

double DegToRad(double n) {
#if _HAS_CXX20
	return n * std::numbers::pi / 180.f;
#else
	return n * M_PI / 180.f;
#endif
}

float RadToDeg(float n) {
#if _HAS_CXX20
	return n / (float)std::numbers::pi * 180.f;
#else
	return n / (float)M_PI * 180.f;
#endif
}

double RadToDeg(double n) {
#if _HAS_CXX20
	return n / std::numbers::pi * 180.f;
#else
	return n / M_PI * 180.f;
#endif
}

float Lerp(float start, float end, float lerp) {
	if (lerp > 1.f) return 0.f;

	return start + (end - start) * lerp;
}

float LerpRad(float start, float end, float lerp) {
	if (lerp > 1.f) return 0.f;

	float ret = 0.f;
	float diff = end - start;

#if _HAS_CXX20
	if (diff < -(float)std::numbers::pi) {
		end += (float)std::numbers::pi * 2.f;
		ret = Lerp(start, end, lerp);
		while (ret >= (float)std::numbers::pi * 2.f) ret -= (float)std::numbers::pi * 2.f;
	}
	else if (diff > (float)std::numbers::pi) {
		end -= (float)std::numbers::pi * 2.f;
		ret = Lerp(start, end, lerp);
		while (ret < 0.f) ret += (float)std::numbers::pi * 2.f;
	}
	else {
		ret = Lerp(start, end, lerp);
	}
#else
	if (diff < -(float)M_PI) {
		end += (float)M_PI * 2.f;
		ret = Lerp(start, end, lerp);
		while (ret >= (float)M_PI * 2.f) ret -= (float)M_PI * 2.f;
	}
	else if (diff > (float)M_PI) {
		end -= (float)M_PI * 2.f;
		ret = Lerp(start, end, lerp);
		while (ret < 0.f) ret += (float)M_PI * 2.f;
	}
	else {
		ret = Lerp(start, end, lerp);
	}
#endif

	return ret;
}

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

float Vector2::Angle() const {
	return atan2f(y, x);
}

float Vector2::AngleDegree() const {
#if _HAS_CXX20
	return Angle() / (float)std::numbers::pi * 180.f;
#else
	return Angle() / (float)M_PI * 180.f;
#endif
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

Vector2 operator+(const Vector2& va, const Vector2& vb) {
	return Vector2(va.x + vb.x, va.y + vb.y);
}

Vector2 operator-(const Vector2& va, const Vector2& vb) {
	return Vector2(va.x - vb.x, va.y - vb.y);
}

bool operator==(const Vector2& va, const Vector2& vb) {
	return va.x == vb.x && va.y == vb.y;
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

Vector2 GetVector2FromAngle(float angle, float length) {
	return Vector2(cosf(angle), sinf(angle)) * length;
}

float Vector3::Magnitude() const {
	return sqrt(SquareMagnitude());
}

float Vector3::SquareMagnitude() const {
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

Vector3 operator+(const Vector3& va, const Vector3& vb) {
	return Vector3(va.x + vb.x, va.y + vb.y, va.z + vb.z);
}

Vector3 operator-(const Vector3& va, const Vector3& vb) {
	return Vector3(va.x - vb.x, va.y - vb.y, va.z - vb.z);
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
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
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
		1.f, 0.f, 0.f, 0.f,
		0.f, cosf(angle), -sinf(angle), 0.f,
		0.f, sinf(angle), cosf(angle), 0.f,
		0.f, 0.f, 0.f, 1.f
	};
}

Matrix4x4 RotationMatrixY(float angle) {
	return {
		cosf(angle), 0.f, sinf(angle), 0.f,
		0.f, 1.f, 0.f, 0.f,
		-sinf(angle), 0.f, cosf(angle), 0.f,
		0.f, 0.f, 0.f, 1.f
	};
}

Matrix4x4 RotationMatrixZ(float angle) {
	return {
		cosf(angle), -sinf(angle), 0.f, 0.f,
		sinf(angle), cosf(angle), 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
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
