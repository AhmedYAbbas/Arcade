#include "CorePCH.h"
#include "Buttton.h"
#include "Window.h"

namespace Core
{
	Button::Button(const BitmapFont& bitmapFont, const Color& textColor, const Color& highlightColor)
		: m_BitmapFont(bitmapFont), m_Title(""), m_Highlighted(false), m_HighlightColor(highlightColor), m_TextColor(textColor), m_Action(nullptr), m_TextScale(1.f)
	{
	}

	void Button::Init(const std::string& text, Vec2D topLeft, unsigned int width, unsigned height)
	{
		m_BoundingBox = Rectangle(topLeft, width, height);
		m_DrawPosition = m_BitmapFont.GetDrawPosition(m_Title, m_BoundingBox, BitmapFontXAlignment::Center, BitmapFontYAlignment::Center);
	}

	void Button::Draw(Window& window)
	{
		DrawTransform transform;
		transform.Pos = m_DrawPosition;

		ColorParams colorParams;
		colorParams.Overlay = m_TextColor;

		UVParams uvParams;

		window.Draw(m_BitmapFont, m_Title, transform, colorParams, uvParams);
		DrawHighlight(window);
	}

	void Button::ExecuteAction()
	{
		m_Action();
	}

	void Button::DrawHighlight(Core::Window& window)
	{
		if (m_Highlighted)
			window.Draw(m_BoundingBox, m_HighlightColor);
	}
}