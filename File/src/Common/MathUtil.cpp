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

float MathUtil::Lerp(float start, float end, float rate)
{
	rate = std::clamp(rate, 0.0f, 1.0f);

	return start + (end - start) * rate;
}

double MathUtil::Lerp(double start, double end, double rate)
{
	rate = std::clamp(rate, 0.0, 1.0);

	return start + (end - start) * rate;
}

float MathUtil::LerpRad(float start, float end, float rate)
{
	rate = std::clamp(rate, 0.0f, 1.0f);

	float ret = 0.0f;
	float diff = end - start;

	if (diff < -(float)std::numbers::pi)
	{
		end += (float)std::numbers::pi * 2.0f;
		ret = Lerp(start, end, rate);
		while (ret >= (float)std::numbers::pi * 2.0f) ret -= (float)std::numbers::pi * 2.0f;
	}
	else if (diff > (float)std::numbers::pi)
	{
		end -= (float)std::numbers::pi * 2.0f;
		ret = Lerp(start, end, rate);
		while (ret < 0.0f) ret += (float)std::numbers::pi * 2.0f;
	}
	else {
		ret = Lerp(start, end, rate);
	}

	return ret;
}

double MathUtil::LerpRad(double start, double end, double rate)
{
	rate = std::clamp(rate, 0.0, 1.0);

	double ret = 0.0;
	double diff = end - start;

	if (diff < -std::numbers::pi)
	{
		end += std::numbers::pi * 2.0;
		ret = Lerp(start, end, rate);
		while (ret >= std::numbers::pi * 2.0) ret -= std::numbers::pi * 2.0;
	}
	else if (diff > std::numbers::pi)
	{
		end -= std::numbers::pi * 2.0;
		ret = Lerp(start, end, rate);
		while (ret < 0.0) ret += std::numbers::pi * 2.0;
	}
	else {
		ret = Lerp(start, end, rate);
	}

	return ret;
}

unsigned int MathUtil::LerpColor(unsigned int start, unsigned int end, float rate)
{
	rate = std::clamp(rate, 0.0f, 1.0f);

	int ret_r = start >> 16 & 0xff, ret_g = start >> 8 & 0xff, ret_b = start & 0xff;
	ret_r += Round(rate * ((end >> 16 & 0xff) - (start >> 16 & 0xff)));
	ret_g += Round(rate * ((end >> 8 & 0xff) - (start >> 8 & 0xff)));
	ret_b += Round(rate * ((end & 0xff) - (start & 0xff)));
	
	return unsigned int(ret_r + ret_g + ret_b);
}
