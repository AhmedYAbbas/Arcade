#pragma once

#include <string>

namespace Core
{
	static const float EPSILON = 0.0001f;
	static const float PI = 3.14159f;
	static const float TWO_PI = 2.f * PI;

	struct Size
	{
		unsigned int Width = 0, Height = 0;
	};

	bool IsEqual(float X, float y);
	bool IsGreaterThanOrEqual(float x, float y);
	bool IsLessThanOrEqual(float x, float y);
	float Clamp(float value, float min, float max);
	float MillisecondsToSeconds(unsigned int milliseconds);
	unsigned int GetIndex(unsigned int width, unsigned int r, unsigned int c);
	bool StringCompare(const std::string& a, const std::string& b);
}