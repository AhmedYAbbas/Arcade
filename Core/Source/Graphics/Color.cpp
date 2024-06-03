#include "CorePCH.h"
#include "Color.h"
#include "Utility/Easings.h"
#include "Utility/Utils.h"

#include <SDL.h>

namespace Core
{
	const SDL_PixelFormat* Color::m_Format = nullptr;

	void Color::InitColorFormat(const SDL_PixelFormat* format)
	{
		m_Format = format;
	}

	Color Color::EvaluateOneMinusSourceAlpha(const Color& source, const Color& destination)
	{
		uint8_t alpha = source.GetAlpha();
		float sourceAlpha = static_cast<float>(alpha) / 255.f;

		Color newColor = Color::Lerp(destination, source, sourceAlpha, Ease::EaseLinear);
		newColor.SetAlpha(255);

		return newColor;
	}

	Color::Color()
		: m_Color(0), m_R(0), m_G(0), m_B(0), m_A(0)
	{
	}

	Color::Color(uint32_t color)
		: m_Color(color)
	{
		SDL_GetRGBA(color, m_Format, &m_R, &m_G, &m_B, &m_A);
	}

	Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: m_Color(0), m_R(r), m_G(g), m_B(b), m_A(a)
	{
		Generate32BitColor();
	}

	void Color::Generate32BitColor()
	{
		m_Color = SDL_MapRGBA(m_Format, m_R, m_G, m_B, m_A);
	}

	void Color::SetRed(uint8_t red)
	{
		m_R = red;
		Generate32BitColor();
	}

	void Color::SetGreen(uint8_t green)
	{
		m_G = green;
		Generate32BitColor();
	}

	void Color::SetBlue(uint8_t blue)
	{
		m_B = blue;
		Generate32BitColor();
	}

	void Color::SetAlpha(uint8_t alpha)
	{
		m_A = alpha;
		Generate32BitColor();
	}

	Color Color::Lerp(const Color& c1, const Color& c2, float t)
	{
		return Lerp(c1, c2, t, Ease::EaseLinear);
	}

	Color Color::Lerp(const Color& c1, const Color& c2, float t, const Ease::EasingFunc& func)
	{
		t = func(t);

		uint8_t r = Clamp8(static_cast<uint8_t>(Lerpf(c1.GetRed(), c2.GetRed(), t)), 0, 255);
		uint8_t g = Clamp8(static_cast<uint8_t>(Lerpf(c1.GetGreen(), c2.GetGreen(), t)), 0, 255);
		uint8_t b = Clamp8(static_cast<uint8_t>(Lerpf(c1.GetBlue(), c2.GetBlue(), t)), 0, 255);
		uint8_t a = Clamp8(static_cast<uint8_t>(Lerpf(c1.GetAlpha(), c2.GetAlpha(), t)), 0, 255);

		return Color(r, g, b, a);
	}

	bool operator==(const Color& c1, const Color& c2)
	{
		return c1.GetPixelColor() == c2.GetPixelColor();
	}

	bool operator!=(const Color& c1, const Color& c2)
	{
		return !(c1 == c2);
	}
}