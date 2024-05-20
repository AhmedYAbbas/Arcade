#pragma once

#include <stdint.h>

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

		void SetRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		void SetRed(uint8_t red);
		void SetGreen(uint8_t green);
		void SetBlue(uint8_t blue);
		void SetAlpha(uint8_t alpha);

		uint8_t GetRed() const;
		uint8_t GetGreen() const;
		uint8_t GetBlue() const;
		uint8_t GetAlpha() const;
		
	public:
		static const SDL_PixelFormat* m_Format;

	private:
		uint32_t m_Color;
	};

	bool operator==(const Color& c1, const Color& c2);
	bool operator!=(const Color& c1, const Color& c2);
}