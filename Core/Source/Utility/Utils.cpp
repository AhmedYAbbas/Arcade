#include "CorePCH.h"
#include "Utils.h"

namespace Core
{
	bool IsEqual(float x, float y)
	{
		return fabsf(x - y) < EPSILON;
	}

	bool IsGreaterThanOrEqual(float x, float y)
	{
		return x > y || IsEqual(x, y);
	}

	bool IsLessThanOrEqual(float x, float y)
	{
		return x < y || IsEqual(x, y);
	}

	float Clamp(float value, float min, float max)
	{
		if (value > max)
			return max;
		else if (value < min)
			return min;

		return value;
	}

	float MillisecondsToSeconds(unsigned int milliseconds)
	{
		return static_cast<float>(milliseconds / 1000.f);
	}

	unsigned int GetIndex(unsigned int width, unsigned int r, unsigned int c)
	{
		return r * width + c;
	}

	bool StringCompare(const std::string& a, const std::string& b)
	{
		if (a.size() == b.length())
		{
			return std::equal(b.begin(), b.end(), a.begin(), [](unsigned char a, unsigned char b)
			{
				return std::tolower(a) == std::tolower(b);
			});
		}
		return false;
	}
}