#pragma once

class MathUtil {
public:
	// 四捨五入
	static int Round(float n);
	// 四捨五入
	static int Round(double n);

	// 端数切り捨て
	static int RoundDown(float n);
	// 端数切り捨て
	static int RoundDown(double n);

	// 端数切り上げ
	static int RoundUp(float n);
	// 端数切り上げ
	static int RoundUp(double n);

	// 弧度からラジアンに変換
	static float DegToRad(float degree);
	// 弧度からラジアンに変換
	static double DegToRad(double degree);

	// ラジアンから弧度に変換
	static float RadToDeg(float radian);
	// ラジアンから弧度に変換
	static double RadToDeg(double radian);

	// ラジアンを０～２πに収める
	static float RadIn2PI(float radian);
	// ラジアンを０～２πに収める
	static double RadIn2PI(double radian);

	// 線形補間
	static float Lerp(float start, float end, float lerp = 0.2f);
	// 線形補間（ラジアン）
	static float LerpRad(float start, float end, float lerp = 0.2f);
};
