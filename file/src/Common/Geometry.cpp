#include <algorithm>
#include <array>
#include <cmath>
#include <numbers>
#include "Geometry.h"

// 注意: 全ての関数を正常にコンパイル・動作させるには、C++20以上が必要です

#pragma region 汎用
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

float DegToRad(float degree) {
	return degree * (float)std::numbers::pi / 180.0f;
}

double DegToRad(double degree) {
	return degree * std::numbers::pi / 180.0;
}

float RadToDeg(float radian) {
	return radian / (float)std::numbers::pi * 180.0f;
}

double RadToDeg(double radian) {
	return radian / std::numbers::pi * 180.0;
}

float RadIn2PI(float radian) {
	while (radian >= (float)std::numbers::pi * 2.0f)
		radian -= (float)std::numbers::pi * 2.0f;
	while (radian < 0.0f)
		radian += (float)std::numbers::pi * 2.0f;

	return radian;
}

double RadIn2PI(double radian) {
	while (radian >= std::numbers::pi * 2.0)
		radian -= std::numbers::pi * 2.0;
	while (radian < 0.0)
		radian += std::numbers::pi * 2.0;

	return radian;
}

float Lerp(float start, float end, float lerp) {
	lerp = std::min(std::max(lerp, 0.0f), 1.0f);

	return start + (end - start) * lerp;
}

float LerpRad(float start, float end, float lerp) {
	lerp = std::min(std::max(lerp, 0.0f), 1.0f);

	float ret = 0.0f;
	float diff = end - start;

	if (diff < -(float)std::numbers::pi) {
		end += (float)std::numbers::pi * 2.0f;
		ret = Lerp(start, end, lerp);
		while (ret >= (float)std::numbers::pi * 2.0f) ret -= (float)std::numbers::pi * 2.0f;
	}
	else if (diff > (float)std::numbers::pi) {
		end -= (float)std::numbers::pi * 2.0f;
		ret = Lerp(start, end, lerp);
		while (ret < 0.0f) ret += (float)std::numbers::pi * 2.0f;
	}
	else {
		ret = Lerp(start, end, lerp);
	}

	return ret;
}
#pragma endregion

#pragma region 色
Color Color::Add(const Color& c, bool limit) const {
	Color ret = {};

	ret.r = r + c.r;
	ret.g = g + c.g;
	ret.b = b + c.b;

	if (limit) {
		ret.r = std::clamp(ret.r, 0.0f, 255.0f);
		ret.g = std::clamp(ret.g, 0.0f, 255.0f);
		ret.b = std::clamp(ret.b, 0.0f, 255.0f);
	}

	return ret;
}

Color Color::operator/(float f) {
	return { r / f, g / f, b / f };
}

unsigned int Color::GetColorHex() const {
	unsigned int ret = 0u;
	ret += (unsigned int)r * 0x10000u;
	ret += (unsigned int)g * 0x100u;
	ret += (unsigned int)b;
	return ret;
}
#pragma endregion

#pragma region ２次元ベクトル
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
	return Angle() / (float)std::numbers::pi * 180.0f;
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
	return { cosf(angle) * length, sinf(angle) * length };
}
#pragma endregion

#pragma region 長方形
void Rect::Expand(float add_size) {
	start.x -= add_size;
	start.y -= add_size;
	end.x += add_size;
	end.y += add_size;
}

Rect Rect::Expanded(float add_size) const {
	return { { start.x - add_size, start.y - add_size }, { end.x + add_size, end.y + add_size } };
}
#pragma endregion

#pragma region ３次元ベクトル
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
	return { va.y * vb.z - va.z * vb.y,
	         va.z * vb.x - va.x * vb.z,
	         va.x * vb.y - vb.y * va.x };
}

float operator*(const Vector3& va, const Vector3& vb) {
	return Dot(va, vb);
}

Vector3 operator%(const Vector3& va, const Vector3& vb) {
	return Cross(va, vb);
}
#pragma endregion

#pragma region ４x４行列
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mat) const {
	Matrix4x4 result = {};
	for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) for (int k = 0; k < 4; k++) {
		result.m[i][j] += m[i][k] * mat.m[k][j];
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
		1.0f, 0.0f,        0.0f,         0.0f,
		0.0f, cosf(angle), -sinf(angle), 0.0f,
		0.0f, sinf(angle), cosf(angle),  0.0f,
		0.0f, 0.0f,        0.0f,         1.0f
	};
}

Matrix4x4 RotationMatrixY(float angle) {
	return {
		cosf(angle),  0.0f, sinf(angle), 0.0f,
		0.0f,         1.0f, 0.0f,        0.0f,
		-sinf(angle), 0.0f, cosf(angle), 0.0f,
		0.0f,         0.0f, 0.0f,        1.0f
	};
}

Matrix4x4 RotationMatrixZ(float angle) {
	return {
		cosf(angle), -sinf(angle), 0.0f, 0.0f,
		sinf(angle), cosf(angle),  0.0f, 0.0f,
		0.0f,        0.0f,         1.0f, 0.0f,
		0.0f,        0.0f,         0.0f, 1.0f
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
#pragma endregion

#pragma region クォータニオン
Quaternion Quaternion::Euler(const Vector3& rad) {
	return Euler(rad.x, rad.y, rad.z);
}

Quaternion Quaternion::Euler(double rad_x, double rad_y, double rad_z) {
	Quaternion ret = {};

	rad_x = RadIn2PI(rad_x);
	rad_y = RadIn2PI(rad_y);
	rad_z = RadIn2PI(rad_z);

	double cosZ = cos(rad_z / 2.0f);
	double sinZ = sin(rad_z / 2.0f);
	double cosX = cos(rad_x / 2.0f);
	double sinX = sin(rad_x / 2.0f);
	double cosY = cos(rad_y / 2.0f);
	double sinY = sin(rad_y / 2.0f);

	ret.w = cosX * cosY * cosZ + sinX * sinY * sinZ;
	ret.x = sinX * cosY * cosZ + cosX * sinY * sinZ;
	ret.y = cosX * sinY * cosZ - sinX * cosY * sinZ;
	ret.z = cosX * cosY * sinZ - sinX * sinY * cosZ;

	return ret;
}

double Quaternion::Magnitude() const {
	return sqrt(SquareMagnitude());
}

double Quaternion::SquareMagnitude() const {
	return x * x + y * y + z * z + w * w;
}

Vector3 Quaternion::ToEuler() const {
	Vector3 ret;

	double r11 = 2 * (x * z + w * y);
	double r12 = w * w - x * x - y * y + z * z;
	double r21 = -2 * (y * z - w * x);
	double r31 = 2 * (x * y + w * z);
	double r32 = w * w - x * x + y * y - z * z;

	ret.x = static_cast<float>(asin(r21));
	ret.y = static_cast<float>(atan2(r11, r12));
	ret.z = static_cast<float>(atan2(r31, r32));

	return ret;
}

Matrix4x4 Quaternion::ToMatrix() const {
	Matrix4x4 mat = {};

	struct XYZW { float x, y, z, w; };

	XYZW fq = { (float)x, (float)y, (float)z, (float)w };

	float sx = fq.x * fq.x * 2.0f;
	float sy = fq.y * fq.y * 2.0f;
	float sz = fq.z * fq.z * 2.0f;
	float cx = fq.y * fq.z * 2.0f;
	float cy = fq.x * fq.z * 2.0f;
	float cz = fq.x * fq.y * 2.0f;
	float wx = fq.w * fq.x * 2.0f;
	float wy = fq.w * fq.y * 2.0f;
	float wz = fq.w * fq.z * 2.0f;

	mat.m[0][0] = 1.0f - (sy + sz);	mat.m[0][1] = cz + wz;			mat.m[0][2] = cy - wy;			mat.m[0][3] = 0.0f;
	mat.m[1][0] = cz - wz;			mat.m[1][1] = 1.0f - (sx + sz);	mat.m[1][2] = cx + wx;			mat.m[1][3] = 0.0f;
	mat.m[2][0] = cy + wy;			mat.m[2][1] = cx - wx;			mat.m[2][2] = 1.0f - (sx + sy);	mat.m[2][3] = 0.0f;
	mat.m[3][0] = 0.0f;				mat.m[3][1] = 0.0f;				mat.m[3][2] = 0.0f;				mat.m[3][3] = 1.0f;

	return mat;
}

Vector3 Quaternion::XYZ() const {
	return { (float)x, (float)y, (float)z };
}

void Quaternion::Normalize() {
	double mag = Magnitude();
	w /= mag;
	x /= mag;
	y /= mag;
	z /= mag;
}

Quaternion Quaternion::Normalized() const {
	double mag = Magnitude();
	return { w / mag, x / mag, y / mag, z / mag };
}

Quaternion Quaternion::Inversed() const {
	double n = 1.0f / SquareMagnitude();
	Quaternion tmp = { w, -x, -y, -z };
	return { tmp.w * n, tmp.x * n, tmp.y * n, tmp.z * n };
}

void Quaternion::operator*=(float f) {
	w *= f;
	x *= f;
	y *= f;
	z *= f;
}

Quaternion Quaternion::operator*(float f) const {
	return { w * f, x * f, y * f, z * f };
}

Quaternion Quaternion::operator+(const Quaternion& q) const {
	return { w + q.w, x + q.x, y + q.y, z + q.z };
}

Quaternion Quaternion::Multiplication(const Quaternion& q) const {
	Quaternion ret = {};
	double d1, d2, d3, d4;

	// wの計算 
	d1 = w * q.w;
	d2 = -x * q.x;
	d3 = -y * q.y;
	d4 = -z * q.z;
	ret.w = d1 + d2 + d3 + d4;

	// xの計算 
	d1 = w * q.x;
	d2 = x * q.w;
	d3 = y * q.z;
	d4 = -z * q.y;
	ret.x = d1 + d2 + d3 + d4;

	// yの計算
	d1 = w * q.y;
	d2 = y * q.w;
	d3 = z * q.x;
	d4 = -x * q.z;
	ret.y = d1 + d2 + d3 + d4;

	// zの計算
	d1 = w * q.z;
	d2 = z * q.w;
	d3 = x * q.y;
	d4 = -y * q.x;
	ret.z = d1 + d2 + d3 + d4;

	return ret;
}

Quaternion Quaternion::operator*(const Quaternion& q) const {
	return Multiplication(q);
}

double Dot(const Quaternion& qa, const Quaternion& qb) {
	return (qa.w * qb.w + qa.x * qb.x + qa.y * qb.y + qa.z * qb.z);
}

Quaternion AngleAxis(Vector3 axis, double rad) {
	Quaternion ret = {};

	double norm;
	double c, s;

	ret.w = 1.0;
	ret.x = ret.y = ret.z = 0.0;

	norm = (double)axis.x * (double)axis.x + (double)axis.y * (double)axis.y + (double)axis.z * (double)axis.z;
	if (norm <= 0.0f) return ret;

	norm = 1.0 / sqrt(norm);
	axis.x = (float)(axis.x * norm);
	axis.y = (float)(axis.y * norm);
	axis.z = (float)(axis.z * norm);

	c = cos(0.5f * rad);
	s = sin(0.5f * rad);

	ret.w = c;
	ret.x = s * axis.x;
	ret.y = s * axis.y;
	ret.z = s * axis.z;

	return ret;
}

Quaternion LookRotation(Vector3 dir) {
	Vector3 up = { 0.0f, 1.0f, 0.0f };
	return LookRotation(dir, up);
}

Quaternion LookRotation(Vector3 dir, Vector3 up) {
	dir.Normalize();
	Vector3 right = Cross(up, dir).Normalized();
	up = Cross(dir, right);
	auto m00 = right.x;
	auto m01 = right.y;
	auto m02 = right.z;
	auto m10 = up.x;
	auto m11 = up.y;
	auto m12 = up.z;
	auto m20 = dir.x;
	auto m21 = dir.y;
	auto m22 = dir.z;


	float num8 = (m00 + m11) + m22;
	Quaternion quaternion = {};

	if (num8 > 0.0f) {
		double num = sqrt(num8 + 1.0);
		quaternion.w = num * 0.5;
		num = 0.5 / num;
		quaternion.x = ((double)m12 - m21) * num;
		quaternion.y = ((double)m20 - m02) * num;
		quaternion.z = ((double)m01 - m10) * num;
		return quaternion.Normalized();
	}

	if ((m00 >= m11) && (m00 >= m22)) {
		auto num7 = sqrt(((1.0f + m00) - m11) - m22);
		auto num4 = 0.5f / num7;
		quaternion.x = ((double)m12 - m21) * num4;
		quaternion.y = ((double)m01 + m10) * num4;
		quaternion.z = ((double)m02 + m20) * num4;
		quaternion.w = 0.5 * num7;
		return quaternion.Normalized();
	}

	if (m11 > m22) {
		auto num6 = sqrt(((1.0f + m11) - m00) - m22);
		auto num3 = 0.5f / num6;
		quaternion.x = ((double)m10 + m01) * num3;
		quaternion.y = 0.5 * num6;
		quaternion.z = ((double)m21 + m12) * num3;
		quaternion.w = ((double)m20 - m02) * num3;
		return quaternion.Normalized();
	}

	auto num5 = sqrt(((1.0f + m22) - m00) - m11);
	auto num2 = 0.5f / num5;
	quaternion.x = ((double)m20 + m02) * num2;
	quaternion.y = ((double)m21 + m12) * num2;
	quaternion.z = 0.5 * num5;
	quaternion.w = ((double)m01 - m10) * num2;
	return quaternion.Normalized();
}

Quaternion GetRotation(Matrix4x4 mat) {
	Quaternion ret;

	float s;
	float tr = mat.m[0][0] + mat.m[1][1] + mat.m[2][2] + 1.0f;

	if (tr >= 1.0f) {
		s = 0.5f / sqrtf(tr);
		ret.w = 0.25f / s;
		ret.x = (mat.m[1][2] - mat.m[2][1]) * s;
		ret.y = (mat.m[2][0] - mat.m[0][2]) * s;
		ret.z = (mat.m[0][1] - mat.m[1][0]) * s;
	}
	else {
		float max;
		max = mat.m[1][1] > mat.m[2][2] ? mat.m[1][1] : mat.m[2][2];

		if (max < mat.m[0][0]) {
			s = sqrtf(mat.m[0][0] - (mat.m[1][1] + mat.m[2][2]) + 1.0f);

			float x = s * 0.5f;
			s = 0.5f / s;
			ret.x = x;
			ret.y = (mat.m[0][1] + mat.m[1][0]) * s;
			ret.z = (mat.m[2][0] + mat.m[0][2]) * s;
			ret.w = (mat.m[1][2] - mat.m[2][1]) * s;
		}
		else
			if (max == mat.m[1][1]) {
				s = sqrtf(mat.m[1][1] - (mat.m[2][2] + mat.m[0][0]) + 1.0f);

				float y = s * 0.5f;
				s = 0.5f / s;
				ret.x = (mat.m[0][1] + mat.m[1][0]) * s;
				ret.y = y;
				ret.z = (mat.m[1][2] + mat.m[2][1]) * s;
				ret.w = (mat.m[2][0] - mat.m[0][2]) * s;
			}
			else {
				s = sqrtf(mat.m[2][2] - (mat.m[0][0] + mat.m[1][1]) + 1.0f);

				float z = s * 0.5f;
				s = 0.5f / s;
				ret.x = (mat.m[2][0] + mat.m[0][2]) * s;
				ret.y = (mat.m[1][2] + mat.m[2][1]) * s;
				ret.z = z;
				ret.w = (mat.m[0][1] - mat.m[1][0]) * s;
			}
	}

	return ret;
}
#pragma endregion
