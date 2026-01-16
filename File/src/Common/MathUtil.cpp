#include <algorithm>
#include <numbers>
#include "MathUtil.h"

int MathUtil::Round(float n)
{
	return n - (int)n >= 0.5f ? (int)n + 1 : (int)n;
}

int MathUtil::Round(double n)
{
	return n - (int)n >= 0.5 ? (int)n + 1 : (int)n;
}

int MathUtil::RoundDown(float n)
{
	return (int)n;
}

int MathUtil::RoundDown(double n)
{
	return (int)n;
}

int MathUtil::RoundUp(float n)
{
	return n - (int)n > 0.0f ? (int)n + 1 : (int)n;
}

int MathUtil::RoundUp(double n)
{
	return n - (int)n > 0.0 ? (int)n + 1 : (int)n;
}

float MathUtil::DegToRad(float degree)
{
	return degree * (float)std::numbers::pi / 180.0f;
}

double MathUtil::DegToRad(double degree)
{
	return degree * std::numbers::pi / 180.0;
}

float MathUtil::RadToDeg(float radian)
{
	return radian / (float)std::numbers::pi * 180.0f;
}

double MathUtil::RadToDeg(double radian)
{
	return radian / std::numbers::pi * 180.0;
}

float MathUtil::RadIn2PI(float radian)
{
	while (radian >= (float)std::numbers::pi * 2.0f)
	{
		radian -= (float)std::numbers::pi * 2.0f;
	}
	while (radian < 0.0f)
	{
		radian += (float)std::numbers::pi * 2.0f;
	}

	return radian;
}

double MathUtil::RadIn2PI(double radian)
{
	while (radian >= std::numbers::pi * 2.0)
	{
		radian -= std::numbers::pi * 2.0;
	}
	while (radian < 0.0f)
	{
		radian += std::numbers::pi * 2.0;
	}

	return radian;
}

float MathUtil::Lerp(float start, float end, float lerp)
{
	lerp = std::clamp(lerp, 0.0f, 1.0f);

	return start + (end - start) * lerp;
}

double MathUtil::Lerp(double start, double end, double lerp)
{
	lerp = std::clamp(lerp, 0.0, 1.0);

	return start + (end - start) * lerp;
}

float MathUtil::LerpRad(float start, float end, float lerp)
{
	lerp = std::clamp(lerp, 0.0f, 1.0f);

	float ret = 0.0f;
	float diff = end - start;

	if (diff < -(float)std::numbers::pi)
	{
		end += (float)std::numbers::pi * 2.0f;
		ret = Lerp(start, end, lerp);
		while (ret >= (float)std::numbers::pi * 2.0f) ret -= (float)std::numbers::pi * 2.0f;
	}
	else if (diff > (float)std::numbers::pi)
	{
		end -= (float)std::numbers::pi * 2.0f;
		ret = Lerp(start, end, lerp);
		while (ret < 0.0f) ret += (float)std::numbers::pi * 2.0f;
	}
	else {
		ret = Lerp(start, end, lerp);
	}

	return ret;
}

double MathUtil::LerpRad(double start, double end, double lerp)
{
	lerp = std::clamp(lerp, 0.0, 1.0);

	double ret = 0.0;
	double diff = end - start;

	if (diff < -std::numbers::pi)
	{
		end += std::numbers::pi * 2.0;
		ret = Lerp(start, end, lerp);
		while (ret >= std::numbers::pi * 2.0) ret -= std::numbers::pi * 2.0;
	}
	else if (diff > std::numbers::pi)
	{
		end -= std::numbers::pi * 2.0;
		ret = Lerp(start, end, lerp);
		while (ret < 0.0) ret += std::numbers::pi * 2.0;
	}
	else {
		ret = Lerp(start, end, lerp);
	}

	return ret;
}
