#include "CorePCH.h"
#include "Easings.h"
#include "Utils.h"

namespace Ease
{
	float EaseLinear(float t)
	{
		return t;
	}

	float EaseInQuad(float t)
	{
		return t * t;
	}

	float EaseOutQuad(float t)
	{
		return  1.0f - (1.0f - t) * (1.0f - t);
	}

	float EaseInOutQuad(float t)
	{
		return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.f, 2.f) / 2.f;

	}

	float EaseInCubic(float t)
	{
		return t * t * t;
	}

	float EaseOutCubic(float t)
	{
		return 1.0f - std::pow(1.0f - t, 3.0f);
	}

	float EaseInOutCubic(float t)
	{
		return t < 0.5f ? 4 * t * t * t : 1.0f - static_cast<float>(std::pow(-2.0f * t + 2.0f, 3)) / 2.f;
	}

	float EaseInQuart(float t)
	{
		return t * t * t * t;
	}

	float EaseOutQuart(float t)
	{
		return 1.0f - std::pow(1.0f - t, 4.f);
	}

	float EaseInOutQuart(float t)
	{
		return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - static_cast<float>(std::pow(-2.0f * t + 2.0f, 4)) / 2.0f;

	}

	float EaseInBack(float t)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.f;

		return c3 * t * t * t - c1 * t * t;
	}

	float EaseOutBack(float t)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1;

		return 1.0f + c3 * static_cast<float>(std::pow(t - 1.0f, 3.0)) + c1 * static_cast<float>(std::pow(t - 1.0f, 2));
	}
	float EaseInOutBack(float t)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 * 1.525f;

		return t < 0.5f
			? (static_cast<float>(std::pow(2.0f * t, 2)) * ((c3 + 1.0f) * 2.0f * t - c3)) / 2.0f
			: (static_cast<float>(std::pow(2.0f * t - 2, 2.0f)) * ((c3 + 1) * (t * 2.0f - 2.0f) + c3) + 2.0f) / 2.0f;
	}

	float EaseInElastic(float t)
	{
		const float c4 = (2.0f * Core::PI) / 3.0f;
		return t == 0.0f ? 0.0f : t == 1.0f ? 1.0f : -std::pow(2.f, 10.0f * t - 10.0f) * sinf(t * 10.0f - 10.75f) * c4;
	}

	float EaseOutElastic(float t)
	{
		const float c4 = (2.0f * Core::PI) / 3.0f;

		return t == 0
			? 0
			: t == 1
			? 1.0f
			: static_cast<float>(std::pow(2, -10.0f * t)) * sin((t * 10.f - 0.75f) * c4) + 1.0f;
	}

	float EaseInOutElastic(float t)
	{
		const float c5 = (2.0f * Core::PI) / 4.5f;

		return t == 0.0f
			? 0.0f
			: t == 1.0f
			? 1.0f
			: t < 0.5f
			? -(static_cast<float>(std::pow(2, 20.0f * t - 10.0f)) * sinf(20.f * t - 11.125f) * c5) / 2.0f
			: (static_cast<float>(std::pow(2, -20.f * t - 10.0f)) * sinf(20.f * t - 11.125f) * c5) / 2.0f + 1;
	}

	float EaseInBounce(float t)
	{
		return 1.0f - EaseOutBounce(1 - t);
	}

	float EaseOutBounce(float t)
	{
		const float n1 = 7.5625f;
		const float d1 = 2.75f;

		if (t < 1.0f / d1)
		{
			return n1 * t * t;
		}
		else if (t < 2.0f / d1)
		{
			return n1 * (t -= 1.5f / d1) * t + 0.75f;
		}
		else if (t < 2.5f / d1)
		{
			return n1 * (t -= 2.25f / d1) * t + 0.9375f;
		}
		else
		{
			return n1 * (t -= 2.625f / d1) * t + 0.984375f;
		}
	}

	float EaseInOutBounce(float t)
	{
		return t < 0.5f
			? (1.f - EaseOutBounce(1.f - 2.f * t)) / 2.0f
			: (1.f + EaseOutBounce(2.f * t - 1.f)) / 2.0f;
	}

	float SmoothStep(float t)
	{
		return t < 0.5f ? 2.f * t * t : 1.f - std::powf(-2.f * t + 2.f, 2.f) / 2.f;
	}
}