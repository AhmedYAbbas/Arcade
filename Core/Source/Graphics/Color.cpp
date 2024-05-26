#include "CorePCH.h"

#include <SDL.h>

#include "Color.h"

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
		float destAlpha = 1.f - sourceAlpha;

		Color outColor;
		outColor.SetAlpha(255);
		outColor.SetRed(static_cast<float>(source.GetRed()) * sourceAlpha + destination.GetRed() * destAlpha);
		outColor.SetGreen(static_cast<float>(source.GetGreen()) * sourceAlpha + destination.GetGreen() * destAlpha);
		outColor.SetBlue(static_cast<float>(source.GetBlue()) * sourceAlpha + destination.GetBlue() * destAlpha);

		return outColor;
	}

	Color::Color()
		: Color(0)
	{
	}

	Color::Color(uint32_t color)
		: m_Color(color)
	{
	}

	Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		SetRGBA(r, g, b, a);
	}

	void Color::SetRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		m_Color = SDL_MapRGBA(m_Format, r, g, b, a);
	}

	void Color::SetRed(uint8_t red)
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		SDL_GetRGBA(m_Color, m_Format, &r, &g, &b, &a);
		SetRGBA(red, g, b, a);
	}

	void Color::SetGreen(uint8_t green)
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		SDL_GetRGBA(m_Color, m_Format, &r, &g, &b, &a);
		SetRGBA(r, green, b, a);
	}

	void Color::SetBlue(uint8_t blue)
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		SDL_GetRGBA(m_Color, m_Format, &r, &g, &b, &a);
		SetRGBA(r, g, blue, a);
	}

	void Color::SetAlpha(uint8_t alpha)
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		SDL_GetRGBA(m_Color, m_Format, &r, &g, &b, &a);
		SetRGBA(r, g, b, alpha);
	}

	uint8_t Color::GetRed() const
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		SDL_GetRGBA(m_Color, m_Format, &r, &g, &b, &a);
		return r;
	}

	uint8_t Color::GetGreen() const
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		SDL_GetRGBA(m_Color, m_Format, &r, &g, &b, &a);
		return g;
	}

	uint8_t Color::GetBlue() const
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		SDL_GetRGBA(m_Color, m_Format, &r, &g, &b, &a);
		return b;
	}

	uint8_t Color::GetAlpha() const
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		SDL_GetRGBA(m_Color, m_Format, &r, &g, &b, &a);
		return a;
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