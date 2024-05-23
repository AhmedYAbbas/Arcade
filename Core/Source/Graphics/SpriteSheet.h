#pragma once

#include <string>
#include <vector>

#include "BMPImage.h"

namespace Core
{
	struct Sprite
	{
		uint32_t xPos = 0;
		uint32_t yPos = 0;
		uint32_t width = 0;
		uint32_t height = 0;
	};

	class SpriteSheet
	{
	public:
		SpriteSheet();

		bool Load(const std::string& name);
		Sprite GetSprite(const std::string& spriteName) const;

		std::vector<std::string> SpriteNames() const;

		inline const BMPImage& GetBMPImage() const { return m_BMPImage; }
		inline uint32_t GetWidth() const { return m_BMPImage.GetWidth(); }
		inline uint32_t GetHeight() const { return m_BMPImage.GetHeight(); }

	private:
		bool LoadSpriteSections(const std::string& filePath);

	private:
		struct BMPImageSection
		{
			std::string key = "";
			Sprite sprite;
		};

		BMPImage m_BMPImage;
		std::vector<BMPImageSection> m_Sections;
	};
}