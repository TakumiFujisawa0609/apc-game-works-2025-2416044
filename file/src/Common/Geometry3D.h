#pragma once

// -------------
// -- Vector3 --
// -------------
struct Vector3 {
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}

	float x, y, z;

	float Magnitude() const;
	float SQMagnitude() const;

	void Normalize();
	Vector3 Normalized() const;

	void operator+=(const Vector3& v);
	void operator-=(const Vector3& v);
	void operator*=(float scale);
	Vector3 operator-() const;
	Vector3 operator*(float scale) const;
};

float Dot(const Vector3& va, const Vector3& vb);
Vector3 Cross(const Vector3& va, const Vector3& vb);

float operator*(const Vector3& va, const Vector3& vb);
Vector3 operator%(const Vector3& va, const Vector3& vb);

// ---------------
// -- Matrix4x4 --
// ---------------
struct Matrix4x4 {
	float m[4][4] = {};

	Matrix4x4 operator*(const Matrix4x4& rhs) const;
	Vector3 operator*(const Vector3& v) const;
};

Matrix4x4 IdentityMatrix();
Matrix4x4 TransposeMatrix(const Matrix4x4& m);

Matrix4x4 RotationMatrixX(float angle);
Matrix4x4 RotationMatrixY(float angle);
Matrix4x4 RotationMatrixZ(float angle);

Matrix4x4 TranslationMatrix(const Vector3& v);
Matrix4x4 TranslationMatrix(float x, float y, float z);
