#include "Buttton.h"
#include "Window.h"

namespace Core
{
	Button::Button(const BitmapFont& bitmapFont, const Color& textColor, const Color& highlightColor)
		: m_BitmapFont(bitmapFont), m_Title(""), m_Highlighted(false), m_HighlightColor(highlightColor), m_TextColor(textColor)
	{
	}

	void Button::Init(Vec2D topLeft, unsigned int width, unsigned int height)
	{
		m_BoundingBox = Rectangle(topLeft, width, height);
	}

	void Button::Draw(Window& window)
	{
		window.Draw(m_BitmapFont, m_Title, m_BitmapFont.GetDrawPosition(m_Title, m_BoundingBox, BitmapFontXAlignment::Center, BitmapFontYAlignment::Center));

		if (m_Highlighted)
			window.Draw(m_BoundingBox, m_HighlightColor);
	}

	void Button::ExecuteAction()
	{
		m_Action();
	}
}