#include <algorithm>
#include <cmath>
#include <numbers>
#include "Geometry.h"

// 注意: 全ての関数を正常にコンパイル・動作させるには、C++20以上が必要です

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
	return n - (int)n > 0.0f ? (int)n + 1 : (int)n;
}

int RoundUp(double n) {
	return n - (int)n > 0.0 ? (int)n + 1 : (int)n;
}

float DegToRad(float n) {
	return n * (float)std::numbers::pi / 180.0f;
}

double DegToRad(double n) {
	return n * std::numbers::pi / 180.0f;
}

float RadToDeg(float n) {
	return n / (float)std::numbers::pi * 180.0f;
}

double RadToDeg(double n) {
	return n / std::numbers::pi * 180.0f;
}

float Lerp(float start, float end, float lerp) {
	lerp = std::min(std::max(lerp, 0.0f), 1.0f);

	return start + (end - start) * lerp;
}

float LerpRad(float start, float end, float lerp) {
	lerp = std::min(std::max(lerp, 0.0f), 1.0f);

	float ret = 0.f;
	float diff = end - start;

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

	return ret;
}

void Color::Normalize() {
	float high = std::max({ r, g, b });

	if (high > 255) {
		high = 255 / high;
		r *= high;
		b *= high;
		g *= high;
	}
}

Color Color::Normalized() const {
	Color ret = { r, g, b };
	ret.Normalize();
	return ret;
}

unsigned int Color::GetColorHex() {
	Normalize();
	unsigned int ret = 0u;
	ret += (unsigned int)r * 0x10000u;
	ret += (unsigned int)g * 0x100u;
	ret += (unsigned int)b;
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
	return { x / mag, y / mag };
}

float Vector2::Angle() const {
	return atan2f(y, x);
}

float Vector2::AngleDegree() const {
	return Angle() / (float)std::numbers::pi * 180.f;
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
	return { x * scale, y * scale };
}

Vector2 Vector2::operator-() const {
	return { -x, -y };
}

Vector2 operator+(const Vector2& va, const Vector2& vb) {
	return { va.x + vb.x, va.y + vb.y };
}

Vector2 operator-(const Vector2& va, const Vector2& vb) {
	return { va.x - vb.x, va.y - vb.y };
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
	return { x * scale, y * scale, z * scale };
}

Vector3 Vector3::operator-() const {
	return { -x, -y, -z };
}

Vector3 operator+(const Vector3& va, const Vector3& vb) {
	return { va.x + vb.x, va.y + vb.y, va.z + vb.z };
}

Vector3 operator-(const Vector3& va, const Vector3& vb) {
	return { va.x - vb.x, va.y - vb.y, va.z - vb.z };
}

bool operator==(const Vector3& va, const Vector3& vb) {
	return va.x == vb.x && va.y == vb.y && va.z == vb.z;
}

float Dot(const Vector3& va, const Vector3& vb) {
	return va.x * vb.x + va.y * vb.y + va.z * vb.z;
}

Vector3 Cross(const Vector3& va, const Vector3& vb) {
	return { va.z * vb.y - va.y * vb.z,
			va.z * vb.x - va.x * vb.z,
			va.x * vb.y - vb.x * va.y };
}

float operator*(const Vector3& va, const Vector3& vb) {
	return Dot(va, vb);
}

Vector3 operator%(const Vector3& va, const Vector3& vb) {
	return Cross(va, vb);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mat) const {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m[i][k] * mat.m[k][j];
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
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
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
		1.0f, 0.0f,			0.0f,			0.0f,
		0.0f, cosf(angle),	-sinf(angle),	0.0f,
		0.0f, sinf(angle),	cosf(angle),	0.0f,
		0.0f, 0.0f,			0.0f,			1.0f
	};
}

Matrix4x4 RotationMatrixY(float angle) {
	return {
		cosf(angle),	0.0f, sinf(angle),	0.0f,
		0.0f,			1.0f, 0.0f,			0.0f,
		-sinf(angle),	0.0f, cosf(angle),	0.0f,
		0.0f,			0.0f, 0.0f,			1.0f
	};
}

Matrix4x4 RotationMatrixZ(float angle) {
	return {
		cosf(angle),	-sinf(angle),	0.0f, 0.0f,
		sinf(angle),	cosf(angle),	0.0f, 0.0f,
		0.0f,			0.0f,			1.0f, 0.0f,
		0.0f,			0.0f,			0.0f, 1.0f
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
