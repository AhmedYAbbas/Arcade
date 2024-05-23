#include "BitmapFont.h"
#include "Shapes/Rectangle.h"
#include "Utility/Vec2D.h"

namespace Core
{
	BitmapFont::BitmapFont()
	{
	}

	bool BitmapFont::Load(const std::string& name)
	{
		return m_FontSheet.Load(name);
	}

	Size BitmapFont::GetSizeOf(const std::string& str) const
	{
		Size textSize;
		size_t length = str.size();
		int i = 0;
		for (const char& c : str)
		{
			if (c == ' ')
			{
				textSize.Width += GetFontSpacingBetweenWords();
				++i;
				continue;
			}
			Sprite sprite = m_FontSheet.GetSprite(std::string(1, c));
			textSize.Height = textSize.Height < sprite.height ? sprite.height : textSize.Height;
			textSize.Width += sprite.width;

			if (i + 1 < length)
				textSize.Width += GetFontSpacingBetweenLetters();
			++i;
		}
		return textSize;
	}

	Vec2D BitmapFont::GetDrawPosition(const std::string& str, const Rectangle& box, BitmapFontXAlignment xAlign, BitmapFontYAlignment yAlign) const
	{
		Size textSize = GetSizeOf(str);
		uint32_t x = 0;
		uint32_t y = 0;

		if (xAlign == BitmapFontXAlignment::Center)
			x = box.GetWidth() / 2 - textSize.Width / 2;
		else if (xAlign == BitmapFontXAlignment::Right)
			x = box.GetWidth() - textSize.Width;

		x += box.GetTopLeftPoint().GetX();

		if (yAlign == BitmapFontYAlignment::Center)
			y = box.GetHeight() / 2 - textSize.Height / 2;
		else if (yAlign == BitmapFontYAlignment::Bottom)
			y = box.GetHeight() - textSize.Height;

		y += box.GetTopLeftPoint().GetY();
		
		return Vec2D(x, y);
	}
}