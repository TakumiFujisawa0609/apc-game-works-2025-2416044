#pragma once

// 弧度からラジアンに変換
float DegToRad(float n);
double DegToRad(double n);

// ラジアンから弧度に変換
float RadToDeg(float n);
double RadToDeg(double n);

// ２次元ベクトル
struct Vector2 {
	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}

	float x, y;

	// 平方和（各要素の２乗の和）の平方根
	const float Magnitude() const;

	// 正規化
	void Normalize();
	// 正規化済みベクトル
	Vector2 Normalized() const;

	// 角度（ラジアン）
	float Angle() const;
	// 角度（弧度）
	float AngleDegree() const;

	// ベクトル加算
	void operator+=(const Vector2& v);
	// ベクトル減算
	void operator-=(const Vector2& v);
	// スカラー倍
	void operator*=(float scale);
	// スカラー倍ベクトル
	Vector2 operator*(float scale) const;
	// 正負反転
	Vector2 operator-() const;
};

// ベクトル同士の加算
Vector2 operator+(const Vector2& va, const Vector2& vb);
// ベクトル同士の減算
Vector2 operator-(const Vector2& va, const Vector2& vb);

// 内積（ドット積）
float Dot(const Vector2& va, const Vector2& vb);
// 外積（クロス積）
float Cross(const Vector2& va, const Vector2& vb);

// 内積（ドット積）
float operator*(const Vector2& va, const Vector2& vb);
// 外積（クロス積）
float operator%(const Vector2& va, const Vector2& vb);

// 角度からベクトルを取得
Vector2 GetVector2FromAngle(float angle, float length);

// ３次元ベクトル
struct Vector3 {
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

	float x, y, z;

	// 平方和（各要素の２乗の和）の平方根
	float Magnitude() const;
	// 平方和（各要素の２乗の和）
	float SquareMagnitude() const;

	// 正規化
	void Normalize();
	// 正規化済みベクトル
	Vector3 Normalized() const;

	// ベクトル加算
	void operator+=(const Vector3& v);
	// ベクトル減算
	void operator-=(const Vector3& v);
	// スカラー倍
	void operator*=(float scale);
	// スカラー倍ベクトル
	Vector3 operator*(float scale) const;
	// 正負反転
	Vector3 operator-() const;
};

// ベクトル同士の加算
Vector3 operator+(const Vector3& va, const Vector3& vb);
// ベクトル同士の減算
Vector3 operator-(const Vector3& va, const Vector3& vb);

// 内積（ドット積）
float Dot(const Vector3& va, const Vector3& vb);
// 外積（クロス積）
Vector3 Cross(const Vector3& va, const Vector3& vb);

// 内積（ドット積）
float operator*(const Vector3& va, const Vector3& vb);
// 外積（クロス積）
Vector3 operator%(const Vector3& va, const Vector3& vb);

// ４×４行列
struct Matrix4x4 {
	float m[4][4] = {};

	// 行列の結合
	Matrix4x4 operator*(const Matrix4x4& rhs) const;
	// ３次元ベクトルとの結合
	Vector3 operator*(const Vector3& v) const;
};

// 単位行列
Matrix4x4 IdentityMatrix();
// 転置行列
Matrix4x4 TransposeMatrix(const Matrix4x4& m);

// Ｘ軸中心の回転行列
Matrix4x4 RotationMatrixX(float angle);
// Ｙ軸中心の回転行列
Matrix4x4 RotationMatrixY(float angle);
// Ｚ軸中心の回転行列
Matrix4x4 RotationMatrixZ(float angle);

// 平行移動行列
Matrix4x4 TranslationMatrix(const Vector3& v);
// 平行移動行列
Matrix4x4 TranslationMatrix(float x, float y, float z);
