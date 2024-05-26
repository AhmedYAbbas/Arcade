#include "CorePCH.h"

#include <SDL.h>

#include "BMPImage.h"

namespace Core
{
	BMPImage::BMPImage()
		: m_Width(0), m_Height(0)
	{
	}

	bool BMPImage::Load(const std::string& filePath)
	{
		if (SDL_Surface* bmpSurface = SDL_LoadBMP(filePath.c_str()))
		{
			m_Width = bmpSurface->w;
			m_Height = bmpSurface->h;

			uint32_t numOfPixels = m_Width * m_Height;
			m_Pixels.reserve(numOfPixels);

			SDL_LockSurface(bmpSurface);

			uint32_t* pixels = static_cast<uint32_t*>(bmpSurface->pixels);
			for (uint32_t i = 0; i < numOfPixels; ++i)
				m_Pixels.push_back(Color(pixels[i]));

			SDL_UnlockSurface(bmpSurface);
			SDL_FreeSurface(bmpSurface);

			return true;
		}
		return false;
	}
}