#pragma once

#include "SpriteSheet.h"
#include "Utility/Utils.h"

namespace Core
{
	enum class BitmapFontXAlignment
	{
		Left = 0,
		Center,
		Right
	};

	enum class BitmapFontYAlignment
	{
		Top = 0,
		Center,
		Bottom
	};

	class Vec2D;
	class Rectangle;

	class BitmapFont
	{
	public:
		BitmapFont();

		bool Load(const std::string& name);

		Size GetSizeOf(const std::string& str) const;
		Vec2D GetDrawPosition(const std::string& str, const Rectangle& box, BitmapFontXAlignment xAlign = BitmapFontXAlignment::Left, BitmapFontYAlignment yAlign = BitmapFontYAlignment::Top) const;

		inline const SpriteSheet& GetSpriteSheet() const { return m_FontSheet; }
		inline const uint32_t GetFontSpacingBetweenLetters() const { return 2; }
		inline const uint32_t GetFontSpacingBetweenWords() const { return 5; }

	private:
		SpriteSheet m_FontSheet;
	};
}