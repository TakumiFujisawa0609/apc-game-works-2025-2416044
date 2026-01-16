#pragma once

class MathUtil
{
public:
	// 少数を四捨五入
	static int Round(float n);
	// 少数を四捨五入
	static int Round(double n);

	// 少数を切り捨て
	static int RoundDown(float n);
	// 少数を切り捨て
	static int RoundDown(double n);

	// 少数を切り上げ
	static int RoundUp(float n);
	// 少数を切り上げ
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
	// 線形補間
	static double Lerp(double start, double end, double lerp = 0.2);
	// 線形補間（ラジアン）
	static float LerpRad(float start, float end, float lerp = 0.2f);
	// 線形補間（ラジアン）
	static double LerpRad(double start, double end, double lerp = 0.2);

	// 配列内の平均値
	template <class Number>
	static inline Number Average(Number* arr)
	{
		Number total = 0;

		for (size_t i = 0; i < sizeof(arr); ++i)
		{
			total += arr[i];
		}

		return total / sizeof(arr);
	}
	
	// 数値が範囲内に含まれているか
	template <class Number>
	static inline bool IsInRange(Number value, Number start, Number end)
	{
		return start <= end && start <= value && value <= end;
	}
};
