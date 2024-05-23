#pragma once

#include <vector>
#include<string>

#include "Color.h"

namespace Core
{
	class BMPImage
	{
	public:
		BMPImage();

		bool Load(const std::string& filePath);

		inline const std::vector<Color>& GetPixels() const { return m_Pixels; }
		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

	private:
		std::vector<Color> m_Pixels;
		uint32_t m_Width;
		uint32_t m_Height;
	};
}