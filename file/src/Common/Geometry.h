#pragma once

// 注意: 全ての関数を正常にコンパイル・動作させるには、C++20以上が必要です

// 四捨五入
int Round(float n);
// 四捨五入
int Round(double n);

// 端数切り捨て
int RoundDown(float n);
// 端数切り捨て
int RoundDown(double n);

// 端数切り上げ
int RoundUp(float n);
// 端数切り上げ
int RoundUp(double n);

// 弧度からラジアンに変換
float DegToRad(float degree);
// 弧度からラジアンに変換
double DegToRad(double degree);

// ラジアンから弧度に変換
float RadToDeg(float radian);
// ラジアンから弧度に変換
double RadToDeg(double radian);

float RadIn2PI(float radian);
double RadIn2PI(double radian);

// 線形補間
float Lerp(float start, float end, float lerp = 0.2f);
// 線形補間（ラジアン）
float LerpRad(float start, float end, float lerp = 0.2f);

struct Color {
	float r, g, b;

	Color() : r(0), g(0), b(0) {}
	Color(float r, float g, float b) : r(r), g(g), b(b) {}

	// unsigned int型を返す
	unsigned int GetColorHex();
};

// ２次元ベクトル
struct Vector2 {
	float x, y;

	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}

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

using Float2 = Vector2;

// ベクトル同士の加算
Vector2 operator+(const Vector2& va, const Vector2& vb);
// ベクトル同士の減算
Vector2 operator-(const Vector2& va, const Vector2& vb);
// ベクトル同士の比較
bool operator==(const Vector2& va, const Vector2& vb);

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
	float x, y, z;

	Vector3() : x(0), y(0), z(0) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

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

using Float3 = Vector3;

// ベクトル同士の加算
Vector3 operator+(const Vector3& va, const Vector3& vb);
// ベクトル同士の減算
Vector3 operator-(const Vector3& va, const Vector3& vb);
// ベクトル同士の比較
bool operator==(const Vector3& va, const Vector3& vb);

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
	Matrix4x4 operator*(const Matrix4x4& mat) const;
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

struct Quaternion {
	static constexpr float kEpsilonNormalSqrt = 1.0e-15f;

	double w, x, y, z;

	Quaternion() : w(1), x(0), y(0), z(0) {};
	Quaternion(double w, double x, double y, double z) : w(w), x(x), y(y), z(z) {};

	// オイラー角（ベクトル）から変換
	Quaternion Euler(const Vector3& radian);
	// オイラー角（単体の要素）から変換
	Quaternion Euler(double radian_x, double radian_y, double radian_z);

	// 平方和（各要素の２乗の和）の平方根
	double Magnitude() const;
	// 平方和（各要素の２乗の和）
	double SquareMagnitude() const;

	Matrix4x4 ToMatrix() const;

	Vector3 XYZ() const;

	// 正規化
	void Normalize();
	// 正規化済み
	Quaternion Normalized() const;

	void operator*=(float f);
	Quaternion operator*(float f) const;

	Quaternion operator+(const Quaternion& q) const;
	Quaternion operator*(const Quaternion& q) const;
};

// 内積（ドット積）
double Dot(const Quaternion& qa, const Quaternion& qb);

Quaternion LookRotation(Vector3 dir);
Quaternion LookRotation(Vector3 dir, Vector3 up);

Quaternion GetRotation(Matrix4x4 mat);
