#pragma once

// 注意: 全ての関数を正常にコンパイル・動作させるには、C++20以上が必要です

#pragma region 汎用
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
#pragma endregion

#pragma region 色
struct Color {
	float r, g, b;

	Color() : r(0), g(0), b(0) {}
	Color(float r, float g, float b) : r(r), g(g), b(b) {}

	Color operator/(float f);

	Color Add(const Color& c, bool limit = true) const;

	// unsigned int型を返す
	unsigned int GetColorHex() const;
};
#pragma endregion

#pragma region ２次元ベクトル
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
#pragma endregion

#pragma region 長方形
// 長方形
struct Rect {
	Vector2 start, end;

	Rect() : start(0, 0), end(0, 0) {}
	Rect(Vector2 start, Vector2 end) : start(start), end(end) {}

	void Expand(float add_size);
	Rect Expanded(float add_size) const;
};
#pragma endregion

#pragma region ３次元ベクトル
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
#pragma endregion

#pragma region ４x４行列
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
#pragma endregion

#pragma region クォータニオン
// クォータニオン
// ※Y軸、X軸、Z軸の順に記述すること
struct Quaternion {
	static constexpr float kEpsilonNormalSqrt = 1.0e-15f;

	double w, x, y, z;

	// デフォルトコンストラクタ
	Quaternion() : w(1), x(0), y(0), z(0) {};
	// 関数用コンストラクタ
	Quaternion(double w, double x, double y, double z) : w(w), x(x), y(y), z(z) {};

	// オイラー角（３次元ベクトル）から、Quaternionに変換
	Quaternion Euler(const Vector3& rad);
	// オイラー角（単体の要素）から、Quaternionに変換
	Quaternion Euler(double rad_x, double rad_y, double rad_z);

	// 平方和（各要素の２乗の和）の平方根
	double Magnitude() const;
	// 平方和（各要素の２乗の和）
	double SquareMagnitude() const;

	// Quaternionから、オイラー角（３次元ベクトル）に変換
	Vector3 ToEuler() const;

	// Quaternionから、行列に変換
	Matrix4x4 ToMatrix() const;

	// Quaternion計算用
	Vector3 XYZ() const;

	// 正規化
	void Normalize();
	// 正規化済み
	Quaternion Normalized() const;

	// 逆Quaternion化済み
	Quaternion Inversed() const;

	void operator*=(float f);
	Quaternion operator*(float f) const;

	Quaternion operator+(const Quaternion& q) const;

	// Quaternionの合成
	Quaternion Multiplication(const Quaternion& q) const;
	// Quaternionの合成
	Quaternion operator*(const Quaternion& q) const;
};

// 内積（ドット積）
double Dot(const Quaternion& qa, const Quaternion& qb);

// 指定した軸を指定した角度回転させる、Quaternionを生成
// @param axis 軸（ゼロベクトルを元に、指定したい軸だけ 1.0F を入れる）
// @param rad ラジアン角
Quaternion AngleAxis(Vector3 axis, double rad);

// 移動方向（３次元ベクトル）から、Quaternionに変換
Quaternion LookRotation(Vector3 dir);
// 移動方向（３次元ベクトル）から、Quaternionに変換
Quaternion LookRotation(Vector3 dir, Vector3 up);

// 行列から、Quaternionに変換
Quaternion GetRotation(Matrix4x4 mat);
#pragma endregion
