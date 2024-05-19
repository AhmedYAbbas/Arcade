#pragma once

#include <stdint.h>
#include <SDL.h>

#include "Color.h"

namespace Core
{
	class ScreenBuffer
	{
	public:
		ScreenBuffer();
		ScreenBuffer(const ScreenBuffer& screenBuffer);
		~ScreenBuffer();

		ScreenBuffer& operator=(const ScreenBuffer& screenBuffer);

		void Init(uint32_t width, uint32_t height, uint32_t format);
		void Clear(const Color& color = Color::Black());

		void SetPixel(int x, int y, const Color& color);

		inline SDL_Surface* GetSurface() { return m_Surface; }

	private:
		uint32_t GetIndex(int r, int c);

	private:
		SDL_Surface* m_Surface;
	};
}