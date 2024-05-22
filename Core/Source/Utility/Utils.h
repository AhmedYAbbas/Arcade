#pragma once

namespace Core
{
	static const float EPSILON = 0.0001f;
	static const float PI = 3.14159f;
	static const float TWO_PI = 2.f * PI;

	bool IsEqual(float X, float y);
	bool IsGreaterThanOrEqual(float x, float y);
	bool IsLessThanOrEqual(float x, float y);
	float MillisecondsToSeconds(unsigned int milliseconds);
}