#pragma once

#include <stdint.h>

#include "Utility/Easings.h"

struct SDL_PixelFormat;

namespace Core
{
	class Color
	{
	public:
		static void InitColorFormat(const SDL_PixelFormat* format);
		static Color EvaluateOneMinusSourceAlpha(const Color& source, const Color& destination);

		static Color Black() 	{ return {0, 0, 0, 255}; }
		static Color White() 	{ return {255, 255, 255, 255}; }
		static Color Blue() 	{ return {0, 0, 255, 255}; }
		static Color Red() 		{ return {255, 0, 0, 255}; }
		static Color Green() 	{ return {0, 255, 0, 255}; }
		static Color Yellow() 	{ return {255, 255, 0, 255}; }
		static Color Magenta() 	{ return {255, 0, 255, 255}; }
		static Color Cyan() 	{ return {37, 240, 217, 255}; }
		static Color Pink() 	{ return {252, 197, 224, 255}; }
		static Color Orange() 	{ return {245, 190, 100, 255}; }

		Color();
		Color(uint32_t color);
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		inline uint32_t GetPixelColor() const { return m_Color; }

		void Generate32BitColor();

		void SetRed(uint8_t red);
		void SetGreen(uint8_t green);
		void SetBlue(uint8_t blue);
		void SetAlpha(uint8_t alpha);

		inline uint8_t GetRed() const { return m_R; }
		inline uint8_t GetGreen() const { return m_G; }
		inline uint8_t GetBlue() const { return m_B; }
		inline uint8_t GetAlpha() const { return m_A; }

		static Color Lerp(const Color& c1, const Color& c2, float t);
		static Color Lerp(const Color& c1, const Color& c2, float t, Ease::EasingFunc func);
		
	public:
		static const SDL_PixelFormat* m_Format;

	private:
		uint32_t m_Color;
		uint8_t m_R;
		uint8_t m_G;
		uint8_t m_B;
		uint8_t m_A;
	};

	bool operator==(const Color& c1, const Color& c2);
	bool operator!=(const Color& c1, const Color& c2);
}